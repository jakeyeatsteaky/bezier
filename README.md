# TODO

- [ ] create a small frame buffer in imgui and render to it
- [ ] creat a small frame buffer in sdl and render to it


# Notes

## Cpp Group Project Stuff
- [ ] Fast vector deletion is cool. I can store my points in a vector and be able to easily delete them. fast vector delete

## ImGUI
- Periodically during runtime, ImGUI outputs to imgui.ini which stores some state about the app on close

### ImGUI init
pulled from the setup page.  This would be for [SDL and OpenGL](https://github.com/ocornut/imgui/wiki/Getting-Started#once-you-are-setup)

```cpp
IMGUI_CHECKVERSION();                                        
ImGui::CreateContext();                                   // Setup Context  
ImGuiIO& io = ImGui::GetIO();
io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

// Setup Platform/Renderer backends
ImGui_ImplSDL2_InitForOpenGL(window, YOUR_SDL_GL_CONTEXT);
ImGui_ImplOpenGL3_Init();
// Note currently just using SDL2_Renderer so the renderer init is:
ImGui_ImplSDLRenderer2_Init(renderer);

// Add Imgui to the event loop:
ImGui_ImplSDL2_ProcessEvent(&event); // Forward your event to backend

// Start the Dear ImGui frame
ImGui_ImplOpenGL3_NewFrame();
ImGui_ImplSDL2_NewFrame();
ImGui::NewFrame();

// Rendering
// (Your code clears your framebuffer, renders your other stuff etc.)
ImGui::Render(); // just prepares data to render
ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
// (Your code calls SDL_RenderPresent() etc.)
```