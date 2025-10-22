#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdio>


static void glfw_error_callback(int error, const char* description) {
    std::fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main() {
    // --- Init GLFW + window ---
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return 1;

    // OpenGL 3.0+ is fine for ImGui OpenGL3 backend
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(900, 600, "aegis-spectra", nullptr, nullptr);
    if (!window) { glfwTerminate(); return 1; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    // --- Init ImGui ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // --- Demo state ---
    float color[3] = {1.0f, 0.5f, 0.25f};
    int r = 255, g = 128, b = 64;
    bool fanOn = false;

    // --- Main loop ---
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start a new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // UI
        ImGui::Begin("Aegis Spectra");

        // Color picker + integer sliders to show both styles
        ImGui::ColorEdit3("Color", color, ImGuiColorEditFlags_NoInputs);
        r = std::lround(color[0] * 255.0f + 0.5f);
        g = std::lround(color[1] * 255.0f + 0.5f);
        b = std::lround(color[2] * 255.0f + 0.5f);

        ImGui::SliderInt("R", &r, 0, 255);
        ImGui::SliderInt("G", &g, 0, 255);
        ImGui::SliderInt("B", &b, 0, 255);
        if (ImGui::Button("Print RGB to console")) {
            std::printf("RGB = %d %d %d\n", r, g, b);
            color[0] = static_cast<float>(r)/255.0f;
            color[1] = static_cast<float>(g)/255.0f;
            color[2] = static_cast<float>(b)/255.0f;
        }

        ImGui::Separator();
        ImGui::Checkbox("Fan On", &fanOn);
        if (ImGui::Button("Apply Fan")) {
            std::printf("Fan: %s\n", fanOn ? "ON" : "OFF");
        }

        ImGui::End();

        // Render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
