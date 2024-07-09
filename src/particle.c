/* AUTHOR: muly / morryaland
 * See file LICENSE for full license details.*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "quad_tree.h"
#include "particle.h"

particle_t *particles;
int particle_c;
float gravity = 0;
float time_ms = 20;

static int count;
static float *distance;
static qtree_node_t **out;
static particle_t **particle;

static void distancef(qtree_node_t *node);

static void distancef(qtree_node_t *node)
{
  float x = (node->massx - (**particle).x);
  float y = (node->massy - (**particle).y);
  float d = sqrtf(x * x + y * y);
  if (isnan(d))
    return;
  if (d < 10)
    d = 10;
  if(abs(node->ex - node->nx) / d > theta &&
         node->a && node->b && node->c && node->d) {
      distancef(node->a);
      distancef(node->b);
      distancef(node->c);
      distancef(node->d);
  }
  else {
    distance = realloc(distance, sizeof(float) * (count + 1));
    distance[count] = d;
    out = realloc(out, sizeof(qtree_node_t*) * (count + 1));
    out[count] = node;
    count++;
  }
}

int particle_add(int x, int y)
{
  pthread_mutex_lock(&sim_mutex);
  particles = realloc(particles, sizeof(particle_t) * (particle_c + 1));
  if (!particles)
    return -1;
  particles[particle_c].x = x;
  particles[particle_c].y = y;
  particles[particle_c].speedx = 0;
  particles[particle_c].speedy = 0;
  particle_c++;
  quad_tree_rebuild();
  pthread_mutex_unlock(&sim_mutex);
  return 0;
}

void particle_move()
{
  if (particle_c < 1)
    return;
  for (int i = 0; i < particle_c; i++) {
    count = 0;
    particle = malloc(sizeof(particle_t*));
    *particle = particles + i;
    distancef(qtree);
    float acx = 0;
    float acy = 0;
    if (gravity) {
      float d = sqrtf((**particle).x * (**particle).x + (**particle).y * (**particle).y);
      if (!isnan(d) && d > 10) {
        float aot = particle_c * gravity / (d * d);
        acx = -(**particle).x * aot;
        acy = -(**particle).y * aot;
      }
    }
    for (int j = 0; j < count; j++) {
      float aot = (out[j]->mass) / (distance[j] * distance[j]);
      acx += aot * (out[j]->massx - (**particle).x);
      acy += aot * (out[j]->massy - (**particle).y);
    }
    (**particle).speedx += acx;
    (**particle).speedy += acy;
    (**particle).x += (**particle).speedx;
    (**particle).y += (**particle).speedy;
    free(particle);
    free(distance);
    free(out);
    distance = NULL;
    out = NULL;
  }
}

void particle_clean()
{
  pthread_mutex_lock(&sim_mutex);
  free(particles);
  particles = NULL;
  particle_c = 0;
  pthread_mutex_unlock(&sim_mutex);
}
