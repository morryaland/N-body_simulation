/* AUTOR: muly20
 * See file LICENSE for full license details.*/

#ifndef PARTICLE_H
#define PARTICLE_H

typedef struct {
  float x;
  float y;
  float speedx;
  float speedy;
} PARTICLE;

int  particle_add(int x, int y);
void particle_clean();

extern PARTICLE *particles;
extern int particle_c;

#endif
