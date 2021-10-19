// #include "Pch.h"

// #include "ImGuiLayer.hpp"

// #include <imgui/backends/imgui_impl_opengl3.h>
// #include <imgui/backends/imgui_impl_glfw.h>

// #include "Render/Renderer.hpp"
// #include "Core/Application.hpp"

// #include "Gl.h"

// #include <GLFW/glfw3.h>

// namespace df
// {
//     bool ImGuiLayer::s_blockEvents = false;

//     void ImGuiLayer::OnAttach()
//     {

//         ImGui::CreateContext();
//         ImGui::StyleColorsDark();
//         ImGuiIO &io = ImGui::GetIO();

//         io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
//         io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
//         // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

//         io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
//         io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

//         io.KeyMap[ImGuiKey_Tab] = int(df::KeyCode::KEY_TAB);
//         io.KeyMap[ImGuiKey_LeftArrow] = int(df::KeyCode::KEY_LEFT);
//         io.KeyMap[ImGuiKey_RightArrow] = int(df::KeyCode::KEY_RIGHT);
//         io.KeyMap[ImGuiKey_UpArrow] = int(df::KeyCode::KEY_UP);
//         io.KeyMap[ImGuiKey_DownArrow] = int(df::KeyCode::KEY_DOWN);
//         io.KeyMap[ImGuiKey_PageUp] = int(df::KeyCode::KEY_PAGE_UP);
//         io.KeyMap[ImGuiKey_PageDown] = int(df::KeyCode::KEY_PAGE_DOWN);
//         io.KeyMap[ImGuiKey_Home] = int(df::KeyCode::KEY_HOME);
//         io.KeyMap[ImGuiKey_End] = int(df::KeyCode::KEY_END);
//         io.KeyMap[ImGuiKey_Insert] = int(df::KeyCode::KEY_INSERT);
//         io.KeyMap[ImGuiKey_Delete] = int(df::KeyCode::KEY_DELETE);
//         io.KeyMap[ImGuiKey_Backspace] = int(df::KeyCode::KEY_BACKSPACE);
//         io.KeyMap[ImGuiKey_Space] = int(df::KeyCode::KEY_SPACE);
//         io.KeyMap[ImGuiKey_Enter] = int(df::KeyCode::KEY_ENTER);
//         io.KeyMap[ImGuiKey_Escape] = int(df::KeyCode::KEY_ESCAPE);
//         io.KeyMap[ImGuiKey_KeyPadEnter] = int(df::KeyCode::KEY_KP_ENTER);
//         io.KeyMap[ImGuiKey_A] = int(df::KeyCode::KEY_A);
//         io.KeyMap[ImGuiKey_C] = int(df::KeyCode::KEY_C);
//         io.KeyMap[ImGuiKey_V] = int(df::KeyCode::KEY_V);
//         io.KeyMap[ImGuiKey_X] = int(df::KeyCode::KEY_X);
//         io.KeyMap[ImGuiKey_Y] = int(df::KeyCode::KEY_Y);
//         io.KeyMap[ImGuiKey_Z] = int(df::KeyCode::KEY_Z);

//         auto window = Application::GetInstance()->GetWindow().GetNativeWindow();
//         ImGui_ImplGlfw_InitForOpenGL(window, true);
//         ImGui_ImplOpenGL3_Init("#version 450");
//     }

//     void ImGuiLayer::OnUpdate()
//     {
//         ImGuiIO &io = ImGui::GetIO();
//         auto size = Application::GetInstance()->GetWindow().GetSize();
//         io.DisplaySize = ImVec2(size.x, size.y);
//         ImGui_ImplOpenGL3_NewFrame();
//         ImGui_ImplGlfw_NewFrame();
//         ImGui::NewFrame();
//     }

//     void ImGuiLayer::OnDraw()
//     {
//         ImGui::EndFrame();
//         ImGui::Render();
//         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

//         ImGuiIO &io = ImGui::GetIO();

//         if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//         {
//             GLFWwindow *backup_current_context = glfwGetCurrentContext();
//             ImGui::UpdatePlatformWindows();
//             ImGui::RenderPlatformWindowsDefault();
//             glfwMakeContextCurrent(backup_current_context);
//         }
//     }

//     void ImGuiLayer::OnDetach()
//     {
//         ImGui_ImplOpenGL3_Shutdown();
//         ImGui_ImplGlfw_Shutdown();
//         ImGui::DestroyContext();
//     }

// #define BlockEvent(expr)      \
//     if (expr)                 \
//     {                         \
//         event->MarkHandled(); \
//         return;               \
//     }

//     void ImGuiLayer::OnEvent(Event *event)
//     {
//         if (s_blockEvents)
//         {
//             auto &io = ImGui::GetIO();
//             BlockEvent(event->IsInCategory(EventCategory::Mouse) & io.WantCaptureMouse);
//             BlockEvent(event->IsInCategory(EventCategory::Keyboard) & io.WantCaptureKeyboard);
//         }
//     }

//     /*  void ImGuiLayer::OnMouseMoved(Event *e)
//     {
//         auto ev = (MouseMovedEvent *)e;
//         auto &io = ImGui::GetIO();
//         io.MousePos = ImVec2(ev->GetMousePos().x, ev->GetMousePos().y);
//     }

//     void ImGuiLayer::OnMouseScrolled(Event *e)
//     {
//         auto ev = (MouseScrolledEvent *)e;
//         auto &io = ImGui::GetIO();
//         io.MouseWheel = ev->GetScrollOffset().x;
//         io.MouseWheelH = ev->GetScrollOffset().y;
//     }

//     void ImGuiLayer::OnMouseClicked(Event *e)
//     {
//         auto ev = (MouseButtonPressedEvent *)e;
//         auto &io = ImGui::GetIO();
//         io.MouseDown[int(ev->GetButtonCode())] = true;
//     }

//     void ImGuiLayer::OnMouseReleased(Event *e)
//     {
//         auto ev = (MouseButtonPressedEvent *)e;
//         auto &io = ImGui::GetIO();
//         io.MouseDown[int(ev->GetButtonCode())] = false;
//     }

//     void ImGuiLayer::OnKeyPressed(Event *e)
//     {
//         auto ev = (KeyPressedEvent *)e;
//         auto &io = ImGui::GetIO();
//         io.KeysDown[int(ev->GetKeyCode())] = true;
//         io.AddInputCharacter(int(ev->GetKeyCode()));
//     }

//     void ImGuiLayer::OnKeyReleased(Event *e)
//     {
//         auto ev = (KeyReleasedEvent *)e;
//         auto &io = ImGui::GetIO();
//         io.KeysDown[int(ev->GetKeyCode())] = false;
//     }

//     void ImGuiLayer::OnWindowResized(Event *e)
//     {
//         auto ev = (WindowRezisedEvent *)e;
//         auto &io = ImGui::GetIO();
//         io.DisplaySize = ImVec2(ev->GetWindowSize().x, ev->GetWindowSize().y);
//     } */

// }