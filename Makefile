CXX = g++ # Compiler to use
EXE = dashboard.exe # Name of the output executable
IMGUI_DIR = imgui # Path to the ImGui library
GLFW_DIR = glfw # Path to the GLFW library

CXXFLAGS = -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I$(GLFW_DIR)/include 
CXXFLAGS += -std=c++17 -Wall -g # Compiler flags

LIBS = -L$(GLFW_DIR)/lib-mingw-w64 -lglfw3 -lopengl32 -lgdi32 -limm32 # Libraries to link against, lglfw3 for Windows, opengl32 for OpenGL, gdi32 for Windows GUI, imm32 for Input Method Manager

all:
	$(CXX) -o $(EXE) $(CXXFLAGS) \
		main.cpp \
		SerialPort.cpp \
		$(IMGUI_DIR)/imgui.cpp \
		$(IMGUI_DIR)/imgui_draw.cpp \
		$(IMGUI_DIR)/imgui_tables.cpp \
		$(IMGUI_DIR)/imgui_widgets.cpp \
		$(IMGUI_DIR)/backends/imgui_impl_glfw.cpp \
		$(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp \
		$(LIBS)
clean:
	rm -f $(EXE)
	
