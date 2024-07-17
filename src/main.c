/* AUTHOR: muly / morryaland
 * See file LICENSE for full license details.*/

#include <pthread.h>
#include <unistd.h>

#include "quad_tree.h"
#include "gui.h"

pthread_mutex_t sim_mutex;

static bool window_open = true;

static void *thread_func(void*);

static void *thread_func(void* arg)
{
  while(window_open) {
    pthread_mutex_lock(&sim_mutex);
    clock_t bef = clock();
    quad_tree_rebuild();
    particle_move();
    pthread_mutex_unlock(&sim_mutex);
    usleep(1);
    sim_speed = (clock() - bef);
    if (sim_speed < time_ms * 1000)
      usleep(time_ms * 1000 - sim_speed);
  }
  return NULL;
}

int main(int argc, char **argv)
{
  init_raylib();

  init_cimgui();

  pthread_mutex_init(&sim_mutex, NULL);
  pthread_t thread;
  pthread_create(&thread, NULL, thread_func, NULL);

  while(window_open)
  {
    draw_imgui();

    get_input();

    draw_window();

    window_open = !WindowShouldClose();
  }
  pthread_join(thread, NULL);
  pthread_mutex_destroy(&sim_mutex);

  quad_tree_free();
  particle_clean();

  destroy_gui();
}
