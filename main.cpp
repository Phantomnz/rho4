#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

int main(int, char**) {
    // Setup window
    glfwInit(); // Initialize the GLFW library
    if (GLFW_FALSE == glfwInit()) { 
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    GLFWwindow* window = glfwCreateWindow(1280, 720, "PID Controller GUI", NULL, NULL); // Create a windowed mode window and its OpenGL context
    if (window == NULL) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Make the window's context current
    glfwSwapInterval(1); // Enable vsync

    // Setup ImGui context
    ImGui::CreateContext(); // Create ImGui context
    ImGui_ImplGlfw_InitForOpenGL(window, true); // Initialize ImGui for GLFW
    ImGui_ImplOpenGL3_Init("#version 130"); // Initialize ImGui for OpenGL3

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); // Poll and handle events

        // Prepare new frame
        ImGui_ImplOpenGL3_NewFrame(); // Start new ImGui frame for OpenGL3
        ImGui_ImplGlfw_NewFrame(); // Start new ImGui frame for GLFW
        ImGui::NewFrame(); // Start new ImGui frame

        // Your GUI code goes here
        ImGui::Begin("First Window");
        ImGui::Text("Hello, world!");
        ImGui::End();

        // Rendering
        ImGui::Render(); // Render ImGui frame
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f); // Set clear color
        glClear(GL_COLOR_BUFFER_BIT); // Clear the screen
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Render ImGui draw data
        glfwSwapBuffers(window); // Swap front and back buffers, making the rendered frame visible
    }
    // Cleanup (must be done in reverse order of initialization)
    ImGui_ImplOpenGL3_Shutdown(); // Shutdown ImGui OpenGL3 backend
    ImGui_ImplGlfw_Shutdown(); // Shutdown ImGui GLFW backend
    ImGui::DestroyContext(); // Destroy ImGui context
    glfwTerminate(); // Terminate GLFW
    return 0;
}