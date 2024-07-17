/* AUTHOR: muly / morryaland
 * See file LICENSE for full license details.*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "quad_tree.h"

qtree_node_t *qtree;

static qtree_node_t *quad_tree(qtree_node_t *parent, int nx, int ny, int ex, int ey);
static int particle_ln();

static qtree_node_t *quad_tree(qtree_node_t *parent, int nx, int ny, int ex, int ey)
{
  if (ex - nx == 1)
    return NULL;

  qtree_node_t *node = tzalloc(sizeof(qtree_node_t), parent);

  node->nx = nx; node->ny = ny; node->ex = ex; node->ey = ey;
  node->contpart = talloc(sizeof(particle_t*), node);

  int cont_c;
  particle_t **part;

  if (!parent) {
    cont_c = particle_c;
    part = talloc(sizeof(particle_t*) * cont_c, node);
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
      node->contpart = trealloc(node->contpart, sizeof(particle_t*) * (node->mass + 1));
      node->contpart[node->mass] = part[i];
      node->massx += particles[i].x;
      node->massy += particles[i].y;
      node->mass++;
    }
  }
  if (node->mass) {
    node->massx /= node->mass;
    node->massy /= node->mass;
  }
  if(node->mass <= 1) {
    return node;
  }
  int midx = (nx + ex) >> 1;
  int midy = (ny + ey) >> 1;
  node->a = quad_tree(node, nx, ny, midx, midy);
  node->b = quad_tree(node, midx, ny, ex, midy);
  node->c = quad_tree(node, midx, midy, ex, ey);
  node->d = quad_tree(node, nx, midy, midx, ey);
  return node;
}

static int particle_ln()
{
  float max = 2;
  for (int i = 0; i < particle_c; i++) {
    int x = fabsf(particles[i].x), y = fabsf(particles[i].y);
    if (x > max)
      max = x;
    if (y > max)
      max = y;
  }
  if (!(int)max)
    return -1;
  return (int)log2f(max) + 1;
}

int quad_tree_rebuild()
{
  quad_tree_free();
  if (quad_tree_init() < 0)
    return -1;
  return 0;
}

int quad_tree_init()
{
  if (particle_c < 1)
    return 0;
  if (!particles) {
    puts("ERROR: particles == NULL");
    return -1;
  }

  int depth = particle_ln();
  if (depth < 0) {
    puts("ERROR: depth ln");
    return -1;
  }

  int bord = 1 << depth;

  qtree = quad_tree(NULL, -bord, -bord, bord, bord);
  return 0;
}

void quad_tree_free()
{
  if (qtree) {
    tfree(qtree);
    qtree = NULL;
  }
}
