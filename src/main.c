/*See file LICENSE for full license details.*/

#include <stdio.h>
#include <stdlib.h>

#include "include/quad_tree.h"

int main(int argc, char **argv)
{
  particle_c = 10;
  particles = malloc(sizeof(PARTICLE) * particle_c);
  for (int i = 0; i < particle_c; i++) {
    particles[i].x = rand()%100 - 50;
    particles[i].y = rand()%100 - 50;
  }
  quad_tree_init();
  quad_tree_free();
}
