#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl2.h"

#if defined(_WIN32) || defined(_WIN64)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    
    int main(void);

    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
        (void)hInstance; (void)hPrevInstance; (void)lpCmdLine; (void)nCmdShow;
        return main();
    }
#endif

int main(void) {
    return 0;
}