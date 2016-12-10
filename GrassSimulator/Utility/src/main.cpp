#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>

#include "SOIL.h"
#include "Utility.h"
#include "Serializer.h"
#include "Data.h"

using namespace std;

const char *TEXT_FILE_PATH = "/Users/dmitrybaranchuk/машграф2016/asvk3/text.txt";
const char *BINARY_FILE_PATH = "/Users/dmitrybaranchuk/машграф2016/asvk3/binary.bin";

const uint FREQUENCY = 100;
const uint GRASS_INSTANCES = 10000;

GL::Camera camera;

GLuint grassPointsCount;
GLuint grassShader;
GLuint grassVAO;

GLuint grassVariance;
GLuint grassPositions;
GLuint grassRotations;
GLuint grassScales;
GLuint grassColor;

vector<VM::vec4> grassVarianceData(GRASS_INSTANCES);
vector<VM::vec2> grassPositionsData(GRASS_INSTANCES);
vector<VM::vec2> grassRotationsData(GRASS_INSTANCES);
vector<float> grassScalesData(GRASS_INSTANCES);
vector<VM::vec4> grassColorData(GRASS_INSTANCES);

GLuint textureGrass;
GLuint groundShader;
GLuint groundVAO;
GLuint textureGround;

uint screenWidth = 1200;
uint screenHeight = 900;

Data data;
Wind wind;

bool BLOCK = false;
bool captureMouse = true;

void DrawGround()
{
    glUseProgram(groundShader);                                                  CHECK_GL_ERRORS
    GLint cameraLocation = glGetUniformLocation(groundShader, "camera");           CHECK_GL_ERRORS
    glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, camera.getMatrix().data().data()); CHECK_GL_ERRORS

    glBindVertexArray(groundVAO);                                                CHECK_GL_ERRORS

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureGround);
    glDrawArrays(GL_TRIANGLES, 0, 6);                                            CHECK_GL_ERRORS

    // Отсоединяем VAO
    glBindVertexArray(0);                                                        CHECK_GL_ERRORS
    // Отключаем шейдер
    glUseProgram(0);                                                             CHECK_GL_ERRORS
}

void UpdateGrassVariance()
{
    for (uint i = 0; i < GRASS_INSTANCES; ++i) {
        grassVarianceData[i] = wind.compute_variance();
    }

    glBindBuffer(GL_ARRAY_BUFFER, grassVariance);                                CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * GRASS_INSTANCES, grassVarianceData.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS
}

void UpdateGrassPositions()
{
    grassPositionsData = data.getGrassPositionsData();

    glBindBuffer(GL_ARRAY_BUFFER, grassPositions);                                CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec2) * GRASS_INSTANCES, grassPositionsData.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS
}

void UpdateGrassRotations()
{
    grassRotationsData = data.getGrassRotationsData();

    glBindBuffer(GL_ARRAY_BUFFER, grassRotations);                                CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec2) * GRASS_INSTANCES, grassRotationsData.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS
}

void UpdateGrassScales()
{
    grassScalesData = data.getGrassScalesData();

    glBindBuffer(GL_ARRAY_BUFFER, grassScales);                                CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * GRASS_INSTANCES, grassScalesData.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS
}

void UpdateGrassColor()
{
    grassColorData = data.getGrassColorData();

    glBindBuffer(GL_ARRAY_BUFFER, grassColor);                                CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * GRASS_INSTANCES, grassColorData.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS
}

void DrawGrass()
{
    if (BLOCK) return;
    glUseProgram(grassShader);                                                   CHECK_GL_ERRORS
    GLint cameraLocation = glGetUniformLocation(grassShader, "camera");          CHECK_GL_ERRORS
    glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, camera.getMatrix().data().data()); CHECK_GL_ERRORS
    glBindVertexArray(grassVAO);                                                   CHECK_GL_ERRORS

    UpdateGrassVariance();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureGrass);
    glEnable(GL_MULTISAMPLE);

    glDrawArraysInstanced(GL_TRIANGLES, 0, grassPointsCount, GRASS_INSTANCES);   CHECK_GL_ERRORS
    glBindVertexArray(0);                                                        CHECK_GL_ERRORS
    glUseProgram(0);                                                             CHECK_GL_ERRORS
}

void RenderLayouts()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawGround();
    DrawGrass();
    glutSwapBuffers();
}

void FinishProgram()
{
    glutDestroyWindow(glutGetWindow());
}

void KeyboardEvents(unsigned char key, int x, int y)
{
    if (key == 27) {
        FinishProgram();
    } else if (key == 'w') {
        camera.goForward();
    } else if (key == 's') {
        camera.goBack();
    } else if (key == 'm') {
        captureMouse = !captureMouse;
        if (captureMouse) {
            glutWarpPointer(screenWidth / 2, screenHeight / 2);
            glutSetCursor(GLUT_CURSOR_NONE);
        } else {
            glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
        }
    } else if (key == 'x') {
        if (wind.get_mode() == OFF) {
            wind.turn_on();
        } else {
            wind.turn_off();
        }
    } else if (key == 'k') {
        data.set_step(wind.get_step());
        data.set_mode(wind.get_mode());

        TextSerializerWriter serializer(TEXT_FILE_PATH);
        data.serialize(serializer);

    } else if (key == 'l') {
        BLOCK = true;
        TextSerializerReader serializer(TEXT_FILE_PATH);
        data.serialize(serializer);
    
        wind.set_step(data.get_step());
        wind.set_mode(data.get_mode());

        UpdateGrassPositions();
        UpdateGrassRotations();
        UpdateGrassScales();
        UpdateGrassColor();
        BLOCK = false;

    } else if (key == 'i') {
        data.set_step(wind.get_step());
        data.set_mode(wind.get_mode());

        BinarySerializerWriter serializer(BINARY_FILE_PATH);
        data.serialize(serializer);

    } else if (key == 'o') {
        BLOCK = true;
        BinarySerializerReader serializer(BINARY_FILE_PATH);
        data.serialize(serializer);

        wind.set_step(data.get_step());
        wind.set_mode(data.get_mode());

        UpdateGrassPositions();
        UpdateGrassRotations();
        UpdateGrassScales();
        UpdateGrassColor();
        BLOCK = false;
    }
}

void SpecialButtons(int key, int x, int y)
{
    if (key == GLUT_KEY_RIGHT) {
        camera.rotateY(0.02);
    } else if (key == GLUT_KEY_LEFT) {
        camera.rotateY(-0.02);
    } else if (key == GLUT_KEY_UP) {
        camera.rotateTop(-0.02);
    } else if (key == GLUT_KEY_DOWN) {
        camera.rotateTop(0.02);
    }
}

void MouseMove(int x, int y)
{
    if (captureMouse) {
        int centerX = screenWidth / 2,
            centerY = screenHeight / 2;
        if (x != centerX || y != centerY) {
            camera.rotateY((x - centerX) / 1000.0f);
            camera.rotateTop((y - centerY) / 1000.0f);
            glutWarpPointer(centerX, centerY);
        }
    }
}

void windowReshapeFunc(GLint newWidth, GLint newHeight)
{
    glViewport(0, 0, newWidth, newHeight);
    screenWidth = newWidth;
    screenHeight = newHeight;

    camera.screenRatio = (float)screenWidth / screenHeight;
}

void mytime(int)
{
    glutPostRedisplay();
    glutTimerFunc(FREQUENCY, mytime, 0);
}

// Инициализация окна
void InitializeGLUT(int argc, char **argv)
{
    glutInit(&argc, argv);

#ifdef __APPLE__
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
#else
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE | GLUT_DEPTH);
#endif
#ifndef __APPLE__
    glutInitContextVersion(3, 3);
    glutInitContextFlags(GLUT_CORE_PROFILE);
    glutInitContextProfile(GLUT_CORE_PROFILE);
#endif
    glutInitWindowPosition(-1, -1);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("Grass Simulator");
    glutWarpPointer(400, 300);
    glutSetCursor(GLUT_CURSOR_NONE);

    glutDisplayFunc(RenderLayouts);
    glutKeyboardFunc(KeyboardEvents);
    glutSpecialFunc(SpecialButtons);
    glutPassiveMotionFunc(MouseMove);
    glutReshapeFunc(windowReshapeFunc);

    glutTimerFunc(FREQUENCY, mytime, 0);
}

vector<VM::vec4> GenMesh(uint n)
{
    return {
        VM::vec4(0, 0, 0, 1),
        VM::vec4(0.8, 0, 0, 1),
        VM::vec4(0.4, 0.3, 0, 1),

        VM::vec4(0.4, 0.3, 0, 1),
        VM::vec4(1.2, 0.3, 0, 1),
        VM::vec4(0.8, 0, 0, 1),

        VM::vec4(0.4, 0.3, 0, 1),
        VM::vec4(1.2, 0.3, 0, 1),
        VM::vec4(1, 0.6, 0, 1),

        VM::vec4(1, 0.6, 0, 1),
        VM::vec4(1.6, 0.6, 0, 1),
        VM::vec4(1.2, 0.3, 0, 1),

        VM::vec4(1, 0.6, 0, 1),
        VM::vec4(1.6, 0.6, 0, 1),
        VM::vec4(2, 1, 0, 1),
    };
}

void InitGrassPositionsData()
{
    for (uint i = 0; i < GRASS_INSTANCES; ++i) {
        grassPositionsData[i] = VM::vec2((float)rand()/RAND_MAX, (float)rand()/RAND_MAX) * 1.5;
    }
}

void InitGrassRotations()
{
    for (uint i = 0; i < GRASS_INSTANCES; i++) {
        float angle = float(rand()) / RAND_MAX * M_PI;
        grassRotationsData[i] = VM::vec2(cos(angle), sin(angle));
    }
}

void InitGrassScales()
{
    for (uint i = 0; i < GRASS_INSTANCES; i++){
        grassScalesData[i] = (float)rand()/RAND_MAX;
    }
}

void InitGrassColor()
{
    for (uint i = 0; i < GRASS_INSTANCES; i++){
        VM::vec4 color = VM::vec4(0.7, 0.8 - ((float)rand()/RAND_MAX)/5, 0.01, 1.0);
        grassColorData[i] = color;
    }
}

void InitGrassVariance()
{
    for (uint i = 0; i < GRASS_INSTANCES; ++i) {
        grassVarianceData[i] = VM::vec4(0, 0, 0, 0);
    }
}

GLuint LoadTexture(const GLchar* path)
{
    //Generate texture ID and load texture data
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width, height;
    unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);

    return textureID;
}

vector<VM::vec2> GenerateGrassTexture()
{
    return {
        VM::vec2(0, 0),
        VM::vec2(0.4, 0),
        VM::vec2(0.2, 0.3),

        VM::vec2(0.2, 0.3),
        VM::vec2(0.6, 0.3),
        VM::vec2(0.4, 0),

        VM::vec2(0.2, 0.3),
        VM::vec2(0.6, 0.3),
        VM::vec2(0.5, 0.6),

        VM::vec2(0.5, 0.6),
        VM::vec2(0.8, 0.6),
        VM::vec2(0.6, 0.3),

        VM::vec2(0.5, 0.6),
        VM::vec2(0.8, 0.6),
        VM::vec2(1, 1),
    };
}

void CreateGrass()
{
    uint LOD = 1;
    vector<VM::vec4> grassPoints = GenMesh(LOD);
    grassPointsCount = static_cast<GLuint>(grassPoints.size());

    InitGrassPositionsData();
    InitGrassRotations();
    InitGrassScales();
    InitGrassColor();

    data.setGrassPositionsData(grassPositionsData);
    data.setGrassRotationsData(grassRotationsData);
    data.setGrassVarianceData(grassVarianceData);
    data.setGrassScalesData(grassScalesData);
    data.setGrassColorData(grassColorData);

    vector<VM::vec2> grassTexture = GenerateGrassTexture();
    grassShader = GL::CompileShaderProgram("grass");

    GLuint pointsBuffer;
    glGenBuffers(1, &pointsBuffer);                                              CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, pointsBuffer);                                 CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * grassPoints.size(), grassPoints.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

    // Создание VAO
    // Генерация VAO
    glGenVertexArrays(1, &grassVAO);                                             CHECK_GL_ERRORS
    // Привязка VAO
    glBindVertexArray(grassVAO);                                                 CHECK_GL_ERRORS

    // Получение локации параметра 'point' в шейдере
    GLuint pointsLocation = glGetAttribLocation(grassShader, "point");           CHECK_GL_ERRORS
    // Подключаем массив атрибутов к данной локации
    glEnableVertexAttribArray(pointsLocation);                                   CHECK_GL_ERRORS
    // Устанавливаем параметры для получения данных из массива (по 4 значение типа float на одну вершину)
    glVertexAttribPointer(pointsLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);          CHECK_GL_ERRORS

    /* Position */
    glGenBuffers(1, &grassPositions);                                            CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, grassPositions);                               CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec2) * grassPositionsData.size(), grassPositionsData.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

    GLuint positionLocation = glGetAttribLocation(grassShader, "position");      CHECK_GL_ERRORS
    glEnableVertexAttribArray(positionLocation);                                 CHECK_GL_ERRORS
    glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);        CHECK_GL_ERRORS
    glVertexAttribDivisor(positionLocation, 1);                                  CHECK_GL_ERRORS

    /* Variance */
    glGenBuffers(1, &grassVariance);                                            CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, grassVariance);                               CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * GRASS_INSTANCES, grassVarianceData.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

    GLuint varianceLocation = glGetAttribLocation(grassShader, "variance");      CHECK_GL_ERRORS
    glEnableVertexAttribArray(varianceLocation);                                 CHECK_GL_ERRORS
    glVertexAttribPointer(varianceLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);        CHECK_GL_ERRORS
    glVertexAttribDivisor(varianceLocation, 1);                                  CHECK_GL_ERRORS

    /* Rotation */
    glGenBuffers(1, &grassRotations);                                            CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, grassRotations);                               CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec2) * GRASS_INSTANCES, grassRotationsData.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

    GLuint rotationLocation = glGetAttribLocation(grassShader, "rotation");      CHECK_GL_ERRORS
    glEnableVertexAttribArray(rotationLocation);                                 CHECK_GL_ERRORS
    glVertexAttribPointer(rotationLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);        CHECK_GL_ERRORS
    glVertexAttribDivisor(rotationLocation, 1);                                  CHECK_GL_ERRORS
    
    /* Scale */
    glGenBuffers(1, &grassScales);                                            CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, grassScales);                               CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * GRASS_INSTANCES, grassScalesData.data(), GL_STATIC_DRAW);
        CHECK_GL_ERRORS
    GLuint scaleLocation = glGetAttribLocation(grassShader, "scale");      CHECK_GL_ERRORS
    glEnableVertexAttribArray(scaleLocation);                                 CHECK_GL_ERRORS
    glVertexAttribPointer(scaleLocation, 1, GL_FLOAT, GL_FALSE, 0, 0);        CHECK_GL_ERRORS
    glVertexAttribDivisor(scaleLocation, 1);                                  CHECK_GL_ERRORS

    /* Color */
    glGenBuffers(1, &grassColor);                                            CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, grassColor);                               CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * GRASS_INSTANCES, grassColorData.data(), GL_STATIC_DRAW);  CHECK_GL_ERRORS
    GLuint colorLocation = glGetAttribLocation(grassShader, "color");          CHECK_GL_ERRORS
    glEnableVertexAttribArray(colorLocation);                                 CHECK_GL_ERRORS
    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);        CHECK_GL_ERRORS
    glVertexAttribDivisor(colorLocation, 1);                                  CHECK_GL_ERRORS

    /* Texture */
    textureGrass = LoadTexture("Texture/grass.jpg"); CHECK_GL_ERRORS

    GLuint textureGrassBuffer;
    glGenBuffers(1, &textureGrassBuffer);                                            CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, textureGrassBuffer);                               CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec2) * grassTexture.size(), grassTexture.data(), GL_STATIC_DRAW);CHECK_GL_ERRORS

    GLuint textureLocation = glGetAttribLocation(grassShader, "texture");      CHECK_GL_ERRORS
    glEnableVertexAttribArray(textureLocation);                                 CHECK_GL_ERRORS
    glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);        CHECK_GL_ERRORS

    glBindVertexArray(0);                                                        CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS
}

// Создаём камеру (Если шаблонная камера вам не нравится, то можете переделать, но я бы не стал)
void CreateCamera() {
    camera.angle = 45.0f / 180.0f * M_PI;
    camera.direction = VM::vec3(0, 0.3, -1);
    camera.position = VM::vec3(0.5, 0.2, 0);
    camera.screenRatio = (float)screenWidth / screenHeight;
    camera.up = VM::vec3(0, 1, 0);
    camera.zfar = 50.0f;
    camera.znear = 0.0001f;
}


vector<VM::vec2> GenerateGroundTexture()
{
    return {
        VM::vec2(0, 0),
        VM::vec2(1, 0),
        VM::vec2(1, 1),

        VM::vec2(0, 0),
        VM::vec2(1, 1),
        VM::vec2(0, 1),
    };
}

void CreateGround() {
    vector<VM::vec4> meshPoints = {
        VM::vec4(0, 0, 0, 1),
        VM::vec4(1.5, 0, 0, 1),
        VM::vec4(1.5, 0, 1.5, 1),

        VM::vec4(0, 0, 0, 1),
        VM::vec4(1.5, 0, 1.5, 1),
        VM::vec4(0, 0, 1.5, 1),
    };
    vector<VM::vec2> groundTexture = GenerateGroundTexture();
    groundShader = GL::CompileShaderProgram("ground");

    GLuint pointsBuffer;
    glGenBuffers(1, &pointsBuffer);                                              CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, pointsBuffer);                                 CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * meshPoints.size(), meshPoints.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

    glGenVertexArrays(1, &groundVAO);                                            CHECK_GL_ERRORS
    glBindVertexArray(groundVAO);                                                CHECK_GL_ERRORS

    GLuint index = glGetAttribLocation(groundShader, "point");                   CHECK_GL_ERRORS
    glEnableVertexAttribArray(index);                                            CHECK_GL_ERRORS
    glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, 0, 0);                   CHECK_GL_ERRORS

    /* Texture */
    textureGround = LoadTexture("Texture/ground1.jpg"); CHECK_GL_ERRORS

    GLuint textureGroundBuffer;
    glGenBuffers(1, &textureGroundBuffer);                                            CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, textureGroundBuffer);                               CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec2) * groundTexture.size(), groundTexture.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

    GLuint textureLocation = glGetAttribLocation(groundShader, "texture");      CHECK_GL_ERRORS
    glEnableVertexAttribArray(textureLocation);                                 CHECK_GL_ERRORS
    glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);        CHECK_GL_ERRORS

    glBindVertexArray(0);                                                        CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS
}

int main(int argc, char **argv)
{
    srand(time(nullptr));
    try {
        cout << "Start" << endl;
        InitializeGLUT(argc, argv);
        cout << "GLUT inited" << endl;
#ifndef __APPLE__
        glewExperimental = GL_TRUE;
        glewInit();
        cout << "glew inited" << endl;
#endif
        CreateCamera();
        cout << "Camera created" << endl;
        CreateGrass();
        cout << "Grass created" << endl;
        CreateGround();
        cout << "Ground created" << endl;
        glutMainLoop();
    } catch (string s) {
        cout << s << endl;
    }
}
