#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
// We need our SerialPort class to talk to the Il Matto
#include "SerialPort.hpp" 
// We need <string> for our status message and <stdio.h> for formatting commands
#include <string> 
#include <stdio.h> // For snprintf


// This makes the GUI look a bit more modern and clean.
void StylePIDController() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text]                 = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]             = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    style.Colors[ImGuiCol_Border]               = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_FrameBg]              = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive]        = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive]        = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
    style.Colors[ImGuiCol_Button]               = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered]        = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]         = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_Header]               = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered]        = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive]         = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.WindowRounding = 5.0f;
    style.FrameRounding = 5.0f;
    style.GrabRounding = 5.0f;
}


int main(int, char**) {
    //Setup window (unchanged)
    glfwInit(); 
    if (GLFW_FALSE == glfwInit()) { 
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    GLFWwindow* window = glfwCreateWindow(600, 400, "PID Controller GUI", NULL, NULL); // Create a 600x400 pixel window
    if (window == NULL) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); 
    glfwSwapInterval(1); // Enable vsync

    //Setup ImGui context
    ImGui::CreateContext(); 
    ImGui_ImplGlfw_InitForOpenGL(window, true); 
    ImGui_ImplOpenGL3_Init("#version 130"); 

    // Apply the custom style
    StylePIDController();

    // 4. Application State Variables
    // These variables store the "state" of our GUI.
    
    // Serial port object
    SerialPort serial;

    // A buffer to hold the COM port name (e.g., "COM3")
    char com_port[32] = "COM3";
    
    // A message to show the connection status
    std::string status_message = "Disconnected";

    // These hold the values for our PID sliders
    // We get the initial values from your "config.hpp"
    float kp = 0.5f;   // INITIAL_KP
    float ki = 0.1f;   // INITIAL_KI
    float kd = 0.05f;  // INITIAL_KD
    int setpoint = 512; // INITIAL_SETPOINT

    // A buffer for formatting our serial commands
    char command_buffer[64];


    // Main loop (This is where the GUI logic lives)
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); 

        // Prepare new frame
        ImGui_ImplOpenGL3_NewFrame(); 
        ImGui_ImplGlfw_NewFrame(); 
        ImGui::NewFrame(); 

        // Our GUI Window
        // We replace "Hello, world!" with this.
        ImGui::Begin("PID Controller Dashboard"); 

        // Connection Section
        ImGui::SetNextItemWidth(150); // Make the input box smaller
        ImGui::InputText("COM Port", com_port, 32);
        ImGui::SameLine(); // Place the next widget on the same line
        
        if (ImGui::Button("Connect")) {
            if (serial.connect(com_port)) {
                status_message = "Successfully connected to ";
                status_message += com_port;
            } else {
                status_message = "Failed to connect!";
            }
        }
        
        ImGui::SameLine();
        if (ImGui::Button("Disconnect")) {
            serial.disconnect();
            status_message = "Disconnected";
        }
        
        // Display the status message
        ImGui::Text("Status: %s", status_message.c_str());
        
        ImGui::Separator(); // A horizontal line

        // PID Control Section
        ImGui::Text("PID Gains");
        
        // Kp Slider
        // ImGui::SliderFloat returns 'true' if the user is dragging it.
        if (ImGui::SliderFloat("Kp", &kp, 0.0f, 5.0f, "%.3f")) {
            // Format the command string (e.g., "p0.500\n")
            snprintf(command_buffer, sizeof(command_buffer), "p%f\n", kp);
            // Send it if we are connected
            if (serial.isConnected()) {
                serial.write(command_buffer);
            }
        }
        
        // Ki Slider
        if (ImGui::SliderFloat("Ki", &ki, 0.0f, 1.0f, "%.3f")) {
            snprintf(command_buffer, sizeof(command_buffer), "i%f\n", ki);
            if (serial.isConnected()) {
                serial.write(command_buffer);
            }
        }
        
        // Kd Slider
        if (ImGui::SliderFloat("Kd", &kd, 0.0f, 1.0f, "%.3f")) {
            snprintf(command_buffer, sizeof(command_buffer), "d%f\n", kd);
            if (serial.isConnected()) {
                serial.write(command_buffer);
            }
        }

        ImGui::Separator();

        // Setpoint Control Section
        ImGui::Text("Setpoint (0-1023)");
        
        // Setpoint Slider
        if (ImGui::SliderInt("##Setpoint", &setpoint, 0, 1023)) {
            // Format the command (e.g., "s512\n")
            snprintf(command_buffer, sizeof(command_buffer), "s%d\n", setpoint);
            if (serial.isConnected()) {
                serial.write(command_buffer);
            }
        }

        ImGui::End(); // End the window

        // Rendering
        ImGui::Render(); 
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f); 
        glClear(GL_COLOR_BUFFER_BIT); 
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 
        glfwSwapBuffers(window); 
    }
    
    // Cleanup 
    // Disconnect the serial port before we quit
    if (serial.isConnected()) {
        serial.disconnect();
    }

    ImGui_ImplOpenGL3_Shutdown(); 
    ImGui_ImplGlfw_Shutdown(); 
    ImGui::DestroyContext(); 
    glfwTerminate(); 
    return 0;
}