/* AUTHOR: muly / morryaland
 * See file LICENSE for full license details.*/

#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "../../lib/talloc/talloc.h"
#include "particle.h"

typedef struct __qtree {
  float massx, massy;
  int mass;
  int nx, ny, ex, ey;
  struct __qtree *p, *a, *b, *c, *d;
} QTREE_NODE;

extern QTREE_NODE *qtree;

int quad_tree_update();

int quad_tree_init();

void quad_tree_free();

#endif
