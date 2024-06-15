/*See file LICENSE for full license details.*/

#ifndef PARTICLE_H
#define PARTICLE_H

typedef struct {
  float x;
  float y;
  float speedx;
  float speedy;
} PARTICLE;

extern PARTICLE *particles;
extern int particle_c;

#endif
