/* AUTHOR: muly / morryaland
 * See file LICENSE for full license details.*/

#ifndef _QUAD_TREE_H_
#define _QUAD_TREE_H_

#include "talloc.h"
#include "particle.h"

#define THREAD_DEPTH 4

typedef struct __qtree {
  float massx, massy;
  int mass;
  int nx, ny, ex, ey;
  particle_t **contpart;
  struct __qtree *a, *b, *c, *d;
} qtree_node_t;

extern qtree_node_t *qtree;
extern float theta;

int quad_tree_rebuild();

int quad_tree_init();

void quad_tree_free();

#endif
