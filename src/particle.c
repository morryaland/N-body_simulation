/* AUTHOR: muly / morryaland
 * See file LICENSE for full license details.*/

#include <stdlib.h>

#include "include/quad_tree.h"
#include "include/particle.h"

PARTICLE *particles;
int particle_c;

int particle_add(int x, int y)
{
  particles = realloc(particles, sizeof(PARTICLE) * (particle_c + 1));
  if (!particles)
    return -1;
  particles[particle_c].x = x;
  particles[particle_c].y = y;
  particle_c++;
  quad_tree_rebuild();
  return 0;
}

void particle_clean()
{
  free(particles);
  particles = NULL;
  particle_c = 0;
  quad_tree_rebuild();
}
