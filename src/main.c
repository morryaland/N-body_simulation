/* AUTHOR: muly / morryaland
 * See file LICENSE for full license details.*/

#include "gui.h"
#include "quad_tree.h"

int main(int argc, char **argv)
{
  init_raylib();

  init_cimgui();

  while(!WindowShouldClose())
  {
    quad_tree_rebuild();
    particle_move();

    draw_imgui();

    get_input();

    draw_window();
  }

  quad_tree_free();
  particle_clean();

  destroy_gui();
}
