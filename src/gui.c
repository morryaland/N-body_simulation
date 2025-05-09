/* AUTHOR: muly / morryaland
 * See file LICENSE for full license details.*/

#include <stdbool.h>

#include "../config.h"
#include "gui.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

unsigned int sim_speed;

static unsigned int particle_spawn_c = 1;
static bool hovered;
static bool draw_force;
static bool draw_qtree;
static bool draw_upper;
static bool isdraw_mass;

static Camera2D cam = {
  .offset = (Vector2){ SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f},
  .rotation = 0.0f,
  .zoom = 1.0f
};

static void window_is_hovered();

void init_raylib()
{
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, PACKAGE_STRING);
}

void init_cimgui()
{
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
}

void draw_imgui()
{
  ImGui_ImplRaylib_ProcessEvents();
  ImGui_ImplRaylib_NewFrame();
  igNewFrame();

  igSetNextWindowSize((ImVec2){ 400, 180 }, ImGuiCond_FirstUseEver);
  if(!igBegin("Tool Bar", NULL, 0)) {
    igEnd();
    return;
  }

  igBeginTabBar("Tab Bar", 0);
  if(igBeginTabItem("Simulation", NULL, 0)) {
    igSliderFloat("theta", &theta, 0, 5, "%.3f", 0);
    igSliderFloat("time ms", &time_ms, 0, 100, "%.3f", 0);
    igDragFloat("gravity", &gravity, 0.01, 0, 100, "%.3f", 0);
    igCheckbox("Draw force", &draw_force);
    igCheckbox("Draw quad tree ", &draw_qtree);
    if (draw_qtree) {
      igSameLine(0, 0);
      igCheckbox("Draw upper", &draw_upper);
      igSameLine(0, 0);
      igCheckbox("Draw mass", &isdraw_mass);
    }
    igText("Application average %.3f ms", 1000.0f / igGetIO()->Framerate);
    igEndTabItem();
  }
  if(igBeginTabItem("Particle Editor", NULL, 0)) {
    igSliderInt("Spawn", &particle_spawn_c, 1, 200, "%d", 0);
    if (igButton("Clean", (ImVec2){ 0 }))
      particle_clean();

    igText("Particle count %d", particle_c);
    igEndTabItem();
  }
  igEndTabBar();

  window_is_hovered();
  igEnd();

  igRender();
}

void window_is_hovered()
{
  ImVec2 pos, size, mouse;
  mouse = igGetIO()->MousePos;
  igGetWindowSize(&size);
  igGetWindowPos(&pos);
  size.x += pos.x;
  size.y += pos.y;
  if (mouse.x >= pos.x && mouse.y >= pos.y &&
    mouse.x <= size.x && mouse.y <= size.y) {
    hovered = true;
  } else {
    hovered = false;
  }
}

void draw_window()
{
  BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(cam);
      for (int i = 0; i < particle_c; i++) {
	if (draw_force)
	  DrawLine(particles[i].x, particles[i].y, particles[i].x + particles[i].speedx * 10, particles[i].y + particles[i].speedy * 10, PURPLE);
        DrawCircle(particles[i].x, particles[i].y, 4, WHITE);
      }
      if (draw_qtree) {
        pthread_mutex_lock(&sim_mutex);
        draw_quad_tree(qtree);
	if (isdraw_mass)
	  draw_mass(qtree);
        pthread_mutex_unlock(&sim_mutex);
      }
    EndMode2D();
    ImGui_ImplRaylib_RenderDrawData(igGetDrawData());
  EndDrawing();
}

void destroy_gui()
{
  ImGui_ImplRaylib_Shutdown();
  igDestroyContext(NULL);
  CloseWindow();
}

void get_input()
{
  if (hovered)
    return;

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    Vector2 pos = GetScreenToWorld2D(GetMousePosition(), cam);
    for (int i = 0; i < particle_spawn_c; i++)
      particle_add(cosf(i * 180.0f / PI) * particle_spawn_c + pos.x, sinf(i * 180.0f / PI) * particle_spawn_c + pos.y);
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
  if (cam.zoom > 3)
    cam.zoom = 3;
  else if (cam.zoom < 0.2)
    cam.zoom = 0.2;
}

void draw_quad_tree(qtree_node_t *node)
{
  if (!node)
    return;
  if (node->a || node->b || node->c || node->d) {
    draw_quad_tree(node->a);
    draw_quad_tree(node->b);
    draw_quad_tree(node->c);
    draw_quad_tree(node->d);
  }
  else {
    if (draw_upper && !node->mass)
      return;
    DrawLine(node->nx, node->ny, node->ex, node->ny, DARKGREEN);
    DrawLine(node->nx, node->ny, node->nx, node->ey, DARKGREEN);
    DrawLine(node->nx, node->ey, node->ex, node->ey, DARKGREEN);
    DrawLine(node->ex, node->ny, node->ex, node->ey, DARKGREEN);
  }
}

void draw_mass(qtree_node_t *node)
{
  if (!node)
    return;
  DrawCircle(node->massx, node->massy, node->mass > 10 ? 10 : node->mass, RED);
  if (node->a || node->b || node->c || node->d) {
  draw_mass(node->a);
  draw_mass(node->b);
  draw_mass(node->c);
  draw_mass(node->d);
  }
}
