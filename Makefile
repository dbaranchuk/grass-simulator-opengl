CXX = g++
CXXFLAGS = -std=c++11 -O3
PROG = Mashgraph_Task3_OpenGL
WORK_DIR = Mashgraph_Task3_OpenGL/bin
BUILD_DIR = Mashgraph_Task3_OpenGL/build
OBJECTS = $(BUILD_DIR)/Camera.o $(BUILD_DIR)/GL.o $(BUILD_DIR)/mat4.o $(BUILD_DIR)/Overall.o $(BUILD_DIR)/ShaderProgram.o $(BUILD_DIR)/Texture.o $(BUILD_DIR)/uvec3.o $(BUILD_DIR)/vec2.o $(BUILD_DIR)/vec3.o $(BUILD_DIR)/vec4.o
SOIL_OBJ = $(BUILD_DIR)/SOIL/SOIL.o $(BUILD_DIR)/SOIL/stb_image_aug.o $(BUILD_DIR)/SOIL/image_DXT.o $(BUILD_DIR)/SOIL/image_helper.o
SOIL = Mashgraph_Task3_OpenGL/libSOIL/src
SRC = Mashgraph_Task3_OpenGL/Utility/src
INCLUDE = Mashgraph_Task3_OpenGL/Utility/include

run: $(WORK_DIR)/$(PROG)
	cd $(WORK_DIR); \
	./$(PROG); \
	cd ../..

$(WORK_DIR)/$(PROG): $(OBJECTS) $(SOIL_OBJ) $(SRC)/main.cpp $(INCLUDE)/Utility.h
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(SOIL_OBJ) $(SRC)/main.cpp -I$(INCLUDE) -I$(SOIL) -lGL -lglut -lGLEW -o $(WORK_DIR)/$(PROG)

$(BUILD_DIR)/SOIL/%.o: $(SOIL)/%.c $(SOIL)/%.h
	$(CXX) $(CXXFLAGS) $< -I$(SOIL) -c -o $@

$(BUILD_DIR)/%.o: $(SRC)/%.cpp $(INCLUDE)/%.h
	$(CXX) $(CXXFLAGS) $< -I$(INCLUDE) -I$(SOIL) -c -o $@

clean:
	rm $(BUILD_DIR)/*.o; \
        rm $(BUILD_DIR)/SOIL/*.o; \
	rm $(WORK_DIR)/$(PROG)
