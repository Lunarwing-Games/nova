#define GLFW_INCLUDE_NONE

#if defined(_WIN32) || defined(_WIN64)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    
    int main(void);

    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
        (void)hInstance; (void)hPrevInstance; (void)lpCmdLine; (void)nCmdShow;
        return main();
    }
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl2.h"
#include "shader.h"
#include "camera.h"

#include "steam_mgr.h"

int main(void) {
    // GLFW + GLEW init steps
    if(!glfwInit()) {
        std::cerr << "GLFW init fail!\n";
    }

    GLFWwindow *window = glfwCreateWindow(800, 600, "Nova", nullptr, nullptr);
    if(!window) {
        std::cerr << "Failed to create GLFW window!\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if(glewInit() != GLEW_OK) {
        std::cerr << "Failed to init GLEW!\n";
        return -1;
    }

    // Imgui setup steps
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "GLFW Version: " << glfwGetVersionString() << "\n";
    std::cout << "GLEW Version: " << glewGetString(GLEW_VERSION) << "\n";
    std::cout << "Imgui Version: " << ImGui::GetVersion() << "\n";
    std::cout << "Steamworks: 1.62\n";
    
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Actual event loop / main loop
    while(!glfwWindowShouldClose(window)) {
        float current_frame = (float)glfwGetTime();
        deltaTime = current_frame - lastFrame;
        lastFrame = current_frame;

        // render
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui::Begin("Debug Info");
        ImGui::Text("FPS: %.1f", 1.0f / deltaTime);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleaning up the mess we made in RAM
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}