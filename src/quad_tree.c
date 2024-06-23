/* AUTHOR: muly / morryaland
 * See file LICENSE for full license details.*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/quad_tree.h"

PARTICLE *particles;
int particle_c;
QTREE_NODE *qtree;

static void quad_tree(QTREE_NODE *parent, QTREE_NODE **node, int depth, int nx, int ny, int ex, int ey);
static int particle_ln();

static void quad_tree(QTREE_NODE *parent, QTREE_NODE **node, int depth, int nx, int ny, int ex, int ey)
{
  if (!depth)
    return;
  if (!(*node))
    *node = tzalloc(sizeof(QTREE_NODE), parent);

  (**node).p = parent;
  (**node).nx = nx; (**node).ny = ny; (**node).ex = ex; (**node).ey = ey;
  (**node).contpart = talloc(sizeof(PARTICLE*), *node);

  int cont_c;
  PARTICLE **part;

  if (!parent) {
    cont_c = particle_c;
    part = talloc(sizeof(PARTICLE*) * cont_c, *node);
    for (int i = 0; i < cont_c; i++) {
      part[i] = particles + i;
    }
  }
  else {
    cont_c = parent->mass;
    part = parent->contpart;
  }

  for (int i = 0; i < cont_c; i++) {
    if (part[i]->x < ex && part[i]->y < ey &&
        part[i]->x > nx && part[i]->y > ny) {
      (**node).contpart = trealloc((**node).contpart, sizeof(PARTICLE*) * ((**node).mass + 1));
      (**node).contpart[(**node).mass] = part[i];
      (**node).massx += particles[i].x;
      (**node).massy += particles[i].y;
      (**node).mass++;
    }
  }
  if (!((**node).mass)) {
    (**node).massx /= (**node).mass;
    (**node).massy /= (**node).mass;
  }
  if((**node).mass <= 1){
    //TODO
    //printf("%d %d %d %d\n", nx, ny, ex, ey);
    return;
  }
  int midx = (nx + ex) >> 1;
  int midy = (ny + ey) >> 1;
  depth--;
  quad_tree(*node, &(**node).a, depth, nx, ny, midx, midy);
  quad_tree(*node, &(**node).b, depth, midx, ny, ex, midy);
  quad_tree(*node, &(**node).c, depth, midx, midy, ex, ey);
  quad_tree(*node, &(**node).d, depth, nx, midy, midx, ey);
}

static int particle_ln()
{
  int res = -1;
  double max = fabs(particles[0].x);
  for (int i = 0; i < particle_c; i++) {
    if (fabs(particles[i].x) > max) {
      max = fabs(particles[i].x);
    }
    if (fabs(particles[i].y) > max) {
      max = fabs(particles[i].y);
    }
  }
  if (!(int)max)
    return -1;
  res = (int)log2(max) + 1;
  return res;
}

void quad_tree_update()
{
  quad_tree_free();
  quad_tree_init();
}

int quad_tree_init()
{
  if (!particles || !particle_c) {
    puts("ERROR: particles == NULL");
    return -1;
  }

  int depth = particle_ln();
  if (depth < 0) {
    puts("ERROR: depth ln");
    return -1;
  }

  int bord = (int)(powf(2, depth) + 0.5f);

  quad_tree(NULL, &qtree, depth, -bord, -bord, bord, bord);
  return 0;
}

void quad_tree_free()
{
  if (qtree) {
    tfree(qtree);
    qtree = NULL;
  }
}
