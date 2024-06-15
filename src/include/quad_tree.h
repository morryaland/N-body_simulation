/*See file LICENSE for full license details.*/

#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include <talloc.h>

#include "particle.h"

typedef struct __qtree {
  float massx, massy;
  int mass;
  struct __qtree *a, *b, *c, *d;
} QTREE_NODE;

typedef struct {
  TALLOC_CTX *ctx;
  QTREE_NODE *child;
} QTREE;

extern QTREE *qtree;

int quad_tree_update();

int quad_tree_init();

void quad_tree_free();

#endif
