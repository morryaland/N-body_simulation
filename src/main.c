/* AUTHOR: muly / morryaland
 * See file LICENSE for full license details.*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../lib/raylib/raylib/src/raylib.h"
#include "../lib/raylib/raylib/src/raymath.h"
#include "../lib/raylib/imgui_impl_raylib.h"
#include "../lib/raylib/rlcimgui.h"
#include "include/quad_tree.h"

static bool draw_upper;

void Draw_quad_tree(QTREE_NODE *node)
{
  if (!node)
    return;
  if (node->a || node->b || node->c || node->d) {
    Draw_quad_tree(node->a);
    Draw_quad_tree(node->b);
    Draw_quad_tree(node->c);
    Draw_quad_tree(node->d);
  }
  else {
    if (draw_upper && !node->mass)
      return;
    DrawRectangleLines(node->nx, node->ny,
                       node->ex - node->nx,
                       node->ey - node->ny, DARKGREEN);
  }
}

int main(int argc, char **argv)
{
  const int screenWidth = 1280;
  const int screenHeight = 720;
  SetConfigFlags(FLAG_WINDOW_RESIZABLE );
  InitWindow(screenWidth, screenHeight, "celestial mechanic");
  SetTargetFPS(60);

  igCreateContext(NULL);
  ImGuiIO *ioptr = igGetIO();
  ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
#ifdef IMGUI_HAS_DOCK
  ioptr->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  ioptr->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
#endif
  igStyleColorsDark(NULL);
  ImGui_ImplRaylib_Init();
  ImFontAtlas_AddFontDefault(ioptr->Fonts, NULL);
  rligSetupFontAwesome();
  ImGui_ImplRaylib_BuildFontAtlas();

  bool hovered = false;
  bool draw_quad_tree = false;

  Camera2D cam = { 0 };
  cam.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f};
  cam.rotation = 0.0f;
  cam.zoom = 1.0f;

  quad_tree_init();
  while(!WindowShouldClose())
  {
    quad_tree_rebuild();
    particle_move();

    ImGui_ImplRaylib_ProcessEvents();
    ImGui_ImplRaylib_NewFrame();
    igNewFrame();

    igBegin("Tool bar", NULL, 0); // Create a window called "Hello, world!" and append into it.
      if (igButton("Clean", (ImVec2){ 0 }))
        particle_clean();

      igCheckbox("Draw quad tree", &draw_quad_tree);
      if (draw_quad_tree)
        igCheckbox("Draw upper", &draw_upper);
      igSliderFloat("theta", &theta, 0, 5, "%.3f", 0);
      igText("Particle count %d", particle_c);
      igText("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);

      ImVec2 pos, size, mouse;
      mouse = igGetIO()->MousePos;
      igGetWindowSize(&size);
      igGetWindowPos(&pos);
      size.x += pos.x;
      size.y += pos.y;
      if (mouse.x >= pos.x && mouse.y >= pos.y &&
          mouse.x <= size.x &&
          mouse.y <= size.y) {
        hovered = true;
    } else {
        hovered = false;
    }
    igEnd();

    igRender();

    if (!hovered) {

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      Vector2 pos = GetScreenToWorld2D(GetMousePosition(), cam);
      particle_add(pos.x, pos.y);
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      Vector2 delta = GetMouseDelta();
      delta = Vector2Scale(delta, -1.0f/cam.zoom);
      cam.target = Vector2Add(cam.target, delta);
    }

    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
      Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), cam);
      cam.offset = GetMousePosition();
      cam.target = mouseWorldPos;
      float scaleFactor = 1.0f + (0.25f*fabsf(wheel));
      if (wheel < 0) scaleFactor = 1.0f/scaleFactor;
      cam.zoom = Clamp(cam.zoom*scaleFactor, 0.125f, 64.0f);
    }

    }

    BeginDrawing();
      ClearBackground(BLACK);
      BeginMode2D(cam);
        for (int i = 0; i < particle_c; i++) {
          DrawCircle(particles[i].x, particles[i].y, 4, WHITE);
        }
        if (draw_quad_tree) {
          Draw_quad_tree(qtree);
        }
      EndMode2D();
      ImGui_ImplRaylib_RenderDrawData(igGetDrawData());
    EndDrawing();
  }
  quad_tree_free();
  free(particles);

  ImGui_ImplRaylib_Shutdown();
  igDestroyContext(NULL);
  CloseWindow();
}
