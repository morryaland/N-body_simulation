/* AUTHOR: muly / morryaland
 * See file LICENSE for full license details.*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../lib/raylib/raylib/src/raylib.h"
#include "../lib/raylib/imgui_impl_raylib.h"
#include "../lib/raylib/rlcimgui.h"
#include "include/quad_tree.h"

int main(int argc, char **argv)
{
  SetConfigFlags(FLAG_WINDOW_RESIZABLE );
  InitWindow(1280, 720, "celestial mechanic");
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
    particles[i].x = rand()%100 - 50;
    particles[i].y = rand()%100 - 50;
  }
  ImVec4 clearColor = {0.45f, 0.55f, 0.60f, 1.00f};
  int open = 1;
  quad_tree_init();
  while(open)
  {
    quad_tree_update();

    ImGui_ImplRaylib_ProcessEvents();
    ImGui_ImplRaylib_NewFrame();
    igNewFrame();
    igBegin("Hello, world!", NULL, 0); // Create a window called "Hello, world!" and append into it.
    igText("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);
    igEnd();
    igRender();
    BeginDrawing();
    ClearBackground((Color){ (unsigned char)(clearColor.x * 255), (unsigned char)(clearColor.y * 255),(unsigned char)(clearColor.z * 255),(unsigned char)(clearColor.w * 255) });
    ImGui_ImplRaylib_RenderDrawData(igGetDrawData());
    EndDrawing();
    open = !WindowShouldClose();

  }
  quad_tree_free();

  ImGui_ImplRaylib_Shutdown();
  igDestroyContext(NULL);
  CloseWindow();
}
