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
  InitWindow(1280, 720, "raylib CIMGUI PURE C");
  SetTargetFPS(144);

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
  bool showDemoWindow = true;
  bool showAnotherWindow = false;
  ImVec4 clearColor = {0.45f, 0.55f, 0.60f, 1.00f};
  bool myToolActive = false;
  bool open = 1;
  quad_tree_init();
  while(open)
  {
    quad_tree_update();

    ImGui_ImplRaylib_ProcessEvents();
    ImGui_ImplRaylib_NewFrame();
    igNewFrame();
    if (showDemoWindow)
            igShowDemoWindow(&showDemoWindow);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            igBegin("Hello, world!", NULL, 0); // Create a window called "Hello, world!" and append into it.

            igText("This is some useful text."); // Display some text (you can use a format strings too)
            igCheckbox("Demo Window", &showDemoWindow); // Edit bools storing our window open/close state
            igCheckbox("Another Window", &showAnotherWindow);

            igSliderFloat("Float", &f, 0.0f, 1.0f, "%.3f", 0); // Edit 1 float using a slider from 0.0f to 1.0f
            igColorEdit3("clear color", (float*)&clearColor, 0); // Edit 3 floats representing a color

            ImVec2 buttonSize;
            buttonSize.x = 0;
            buttonSize.y = 0;
            if (igButton("Button", buttonSize))  // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            igSameLine(0.0f, -1.0f);
            igText("counter = %d", counter);

            igText("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);
            igEnd();
        }

        // 3. Show another simple window.
        if (showAnotherWindow)
        {
            igBegin("Another Window", &showAnotherWindow, 0);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            igText("Hello from another window!");
            ImVec2 buttonSize;
            buttonSize.x = 0;
            buttonSize.y = 0;
            if (igButton("Close me", buttonSize)) {
                showAnotherWindow = false;
            }
            igEnd();
        }

        // Create a window called "My First Tool", with a menu bar.
        igBegin("My First Tool", &myToolActive, ImGuiWindowFlags_MenuBar);
        if (igBeginMenuBar())
        {
            if (igBeginMenu("File", true))
            {
                if (igMenuItem_Bool("Open..", "Ctrl+Q", false, true)) {  }
                if (igMenuItem_Bool("Save", "Ctrl+S", false, false)) { /* Do stuff */ }
                if (igMenuItem_Bool("Close", "Ctrl+W", false, true)) {
                    myToolActive = false;
                }
                igEndMenu();
            }
            igEndMenuBar();
        }

        // Edit a color stored as 4 floats
        igColorEdit4("Color", (float*)&clearColor, 0);

        // Mix icon with text
        igText("%s among %d items", ICON_FA_BOOK, 10);

        igSetNextItemShortcut(ImGuiKey_C, ImGuiMod_Ctrl);
        if(igButton(ICON_FA_MUSIC " Search", (ImVec2){0, 0})) {
            printf("Music button!");
        }

        if(igButton("Add Bunnies", (ImVec2){0, 0})) {
           printf("Bunnies!");
        }

        // Generate samples and plot them
        float samples[100];
        for (int n = 0; n < 100; n++)
            samples[n] = sinf(n * 0.2f + igGetTime() * 1.5f);
        igPlotLines_FloatPtr("Samples", samples, 100, 10, "Overlay", 1.0, 2.0, (ImVec2){100, 100}, 0);

        // Display contents in a scrolling region
        igTextColored((ImVec4){1,1,0,1}, "Important Stuff");
        igBeginChild_Str("Scrolling", (ImVec2){200, 200}, 0, 0);
        for (int n = 0; n < 50; n++)
            igText("%04d: Some text", n);
        igEndChild();
        igEnd();

        // Rendering
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
