/* AUTHOR: muly / morryaland
 * See file LICENSE for full license details.*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../lib/raylib/raylib/src/raylib.h"
#include "../lib/raylib/raylib/src/raymath.h"
#include "../lib/raylib/imgui_impl_raylib.h"
#include "../lib/raylib/rlcimgui.h"
#include "include/quad_tree.h"

void Draw_quad_tree(QTREE_NODE **node)
{
  if (!*node)
    return;
  DrawRectangleLines((**node).nx, (**node).ny, (**node).ex -(**node).nx, (**node).ey -(**node).ny, DARKGREEN);
  Draw_quad_tree(&(**node).a);
  Draw_quad_tree(&(**node).b);
  Draw_quad_tree(&(**node).c);
  Draw_quad_tree(&(**node).d);
}

int main(int argc, char **argv)
{
  const int screenWidth = 1280;
  const int screenHeight = 720;
  SetConfigFlags(FLAG_WINDOW_RESIZABLE );
  InitWindow(screenWidth, screenHeight, "celestial mechanic");
  //SetTargetFPS(144);

  igCreateContext(NULL);
  ImGuiIO *ioptr = igGetIO();
  ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
  ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
#ifdef IMGUI_HAS_DOCK
  ioptr->ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
  ioptr->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
#endif
  igStyleColorsDark(NULL);
  ImGui_ImplRaylib_Init();
  ImFontAtlas_AddFontDefault(ioptr->Fonts, NULL);
  rligSetupFontAwesome();
  ImGui_ImplRaylib_BuildFontAtlas();

  particle_c = 1000;
  particles = malloc(sizeof(PARTICLE) * particle_c);
  for (int i = 0; i < particle_c; i++) {
    particles[i].x = rand()%1000 - 500;
    particles[i].y = rand()%1000 - 500;
  }

  Camera2D cam = { 0 };
  cam.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f};
  cam.rotation = 0.0f;
  cam.zoom = 1.0f;

  quad_tree_init();
  while(!WindowShouldClose())
  {
    quad_tree_update();

    ImGui_ImplRaylib_ProcessEvents();
    ImGui_ImplRaylib_NewFrame();
    igNewFrame();

    igBegin("lol", NULL, 0); // Create a window called "Hello, world!" and append into it.
      igText("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);
    igEnd();

    igRender();

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

    BeginDrawing();
      ClearBackground(BLACK);
      BeginMode2D(cam);
        for (int i = 0; i < particle_c; i++) {
          DrawCircle(particles[i].x, particles[i].y, 2, WHITE);
        }
        Draw_quad_tree(&qtree);
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
