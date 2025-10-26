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
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl2.h"
#include <iostream>
#include <cmath>
#include "camera.h"

#include "steam_mgr.h"

// Globals for camera demo
static Camera cam;
static float lastX = 400.0f, lastY = 300.0f;
static bool firstMouse = true;
static float deltaTime = 0.0f, lastFrame = 0.0f;
static bool cursorCaptured = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) { lastX = (float)xpos; lastY = (float)ypos; firstMouse = false; }
    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos; // reversed Y
    lastX = (float)xpos;
    lastY = (float)ypos;
    camera_process_mouse(&cam, xoffset, yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        cursorCaptured = !cursorCaptured;

        if (cursorCaptured) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = true; // reset so we don't jump when re-capturing
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

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
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "GLFW Version: " << glfwGetVersionString() << "\n";
    std::cout << "GLEW Version: " << glewGetString(GLEW_VERSION) << "\n";
    std::cout << "Imgui Version: " << ImGui::GetVersion() << "\n";
    std::cout << "Steamworks: 1.62\n";

    // Camera test setup
    vec3 pos = {0.0f, 0.0f, 3.0f};
    camera_init(&cam, pos);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    // Actual event loop / main loop
    while(!glfwWindowShouldClose(window)) {
        float current_frame = (float)glfwGetTime();
        deltaTime = current_frame - lastFrame;
        lastFrame = current_frame;
        if(cursorCaptured) {
            int forward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
            int backward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
            int left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
            int right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
            int up = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
            int down = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
            camera_process_movement(&cam, forward, backward, left, right, deltaTime);
            camera_process_vertical(&cam, up, down, deltaTime);
        }

        // Dispose of NoMouse flag from imgui for interactive UI
        io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        if (cursorCaptured) {
            // Lock if mouse is busy/captured
            io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
        }

        // render
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // camera matrices
        mat4 view, proj;
        camera_get_view(&cam, view);
        camera_get_projection(&cam, proj, (float)display_w / display_h, 0.1f, 100.0f);

        // fixed-function setup
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf((const GLfloat*)proj);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf((const GLfloat*)view);

        // spinning cube
        glPushMatrix();
        glRotatef((float)glfwGetTime() * 30.0f, 0.5f, 1.0f, 0.0f);
        glBegin(GL_QUADS);
            glColor3f(1,0,0); glVertex3f(-1,-1, 1); glVertex3f( 1,-1, 1); glVertex3f( 1, 1, 1); glVertex3f(-1, 1, 1);
            glColor3f(0,1,0); glVertex3f(-1,-1,-1); glVertex3f(-1, 1,-1); glVertex3f( 1, 1,-1); glVertex3f( 1,-1,-1);
            glColor3f(0,0,1); glVertex3f(-1, 1,-1); glVertex3f(-1, 1, 1); glVertex3f( 1, 1, 1); glVertex3f( 1, 1,-1);
            glColor3f(1,1,0); glVertex3f(-1,-1,-1); glVertex3f( 1,-1,-1); glVertex3f( 1,-1, 1); glVertex3f(-1,-1, 1);
            glColor3f(0,1,1); glVertex3f( 1,-1,-1); glVertex3f( 1, 1,-1); glVertex3f( 1, 1, 1); glVertex3f( 1,-1, 1);
            glColor3f(1,0,1); glVertex3f(-1,-1,-1); glVertex3f(-1,-1, 1); glVertex3f(-1, 1, 1); glVertex3f(-1, 1,-1);
        glEnd();
        glPopMatrix();

        // imgui overlay
        ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiCond_FirstUseEver);
        ImGui::Begin("Debug Info");
        ImGui::Text("FPS: %.1f", 1.0f / deltaTime);
        ImGui::Text("Position: %.2f, %.2f, %.2f", cam.position[0], cam.position[1], cam.position[2]);
        ImGui::Text("Mouse: %s (esc to toggle)", cursorCaptured ? "Captured" : "Free");
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