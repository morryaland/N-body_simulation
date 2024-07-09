/* AUTHOR: muly / morryaland
 * See file LICENSE for full license details.*/

#ifndef _GUI_H_
#define _GUI_H_

#include "../lib/include/raylib.h"
#include "../lib/include/raymath.h"
#include "../lib/include/imgui_impl_raylib.h"
#include "../lib/include/rlcimgui.h"
#include "quad_tree.h"

extern unsigned int sim_speed;

void init_raylib();

void init_cimgui();

void draw_imgui();

void get_input();

void draw_window();

void destroy_gui();

void draw_quad_tree(qtree_node_t *node);

#endif
