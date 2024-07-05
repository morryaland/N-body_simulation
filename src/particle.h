/* AUTOR: muly20
 * See file LICENSE for full license details.*/

#ifndef _PARTICLE_H_
#define _PARTICLE_H_

typedef struct {
  float x;
  float y;
  float speedx;
  float speedy;
} particle_t;

int  particle_add(int x, int y);

void particle_move();

void particle_clean();

extern particle_t *particles;
extern int particle_c;

#endif
