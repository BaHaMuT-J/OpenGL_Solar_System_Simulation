#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <header/camera.h>
#include <header/shader_m.h>
#include <header/Sphere.h>
#include <header/stb_image.h>

#include <iostream>
#include <vector>

// Function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void drawSphere(Sphere sphere, Shader shaderProgram, bool wireframe);
unsigned int loadTexture(const char* path);
int solarScaledSize(bool isBackgroundBlack);
int solarScaledDistance(bool isBackgroundBlack);

// Constants
extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

// Camera variables
extern Camera camera;
extern float lastX;
extern float lastY;
extern bool firstMouse;

// Timing variables
extern float deltaTime;
extern float lastFrame;

// OpenGL buffers
extern unsigned int VBO, VAO, EBO;

// Planet properties
extern const float SUN_ROTATION_AXIS;
extern const float MERCURY_ROTATION_AXIS;
extern const float VENUS_ROTATION_AXIS;
extern const float EARTH_ROTATION_AXIS;
extern const float MARS_ROTATION_AXIS;
extern const float JUPITER_ROTATION_AXIS;
extern const float SATURN_ROTATION_AXIS;
extern const float URANUS_ROTATION_AXIS;
extern const float NEPTUNE_ROTATION_AXIS;

extern const float SUN_RADIUS;
extern const float MERCURY_RADIUS;
extern const float VENUS_RADIUS;
extern const float EARTH_RADIUS;
extern const float MARS_RADIUS;
extern const float JUPITER_RADIUS;
extern const float SATURN_RADIUS;
extern const float URANUS_RADIUS;
extern const float NEPTUNE_RADIUS;

extern const float MERCURY_DISTANCE_FROM_SUN;
extern const float VENUS_DISTANCE_FROM_SUN;
extern const float EARTH_DISTANCE_FROM_SUN;
extern const float MARS_DISTANCE_FROM_SUN;
extern const float JUPITER_DISTANCE_FROM_SUN;
extern const float SATURN_DISTANCE_FROM_SUN;
extern const float URANUS_DISTANCE_FROM_SUN;
extern const float NEPTUNE_DISTANCE_FROM_SUN;

extern const float SUN_ROTATION_PERIOD_YEAR;
extern const float MERCURY_ROTATION_PERIOD_YEAR;
extern const float VENUS_ROTATION_PERIOD_YEAR;
extern const float EARTH_ROTATION_PERIOD_YEAR;
extern const float MARS_ROTATION_PERIOD_YEAR;
extern const float JUPITER_ROTATION_PERIOD_YEAR;
extern const float SATURN_ROTATION_PERIOD_YEAR;
extern const float URANUS_ROTATION_PERIOD_YEAR;
extern const float NEPTUNE_ROTATION_PERIOD_YEAR;

extern const float MERCURY_ORBITAL_PERIOD_YEAR;
extern const float VENUS_ORBITAL_PERIOD_YEAR;
extern const float EARTH_ORBITAL_PERIOD_YEAR;
extern const float MARS_ORBITAL_PERIOD_YEAR;
extern const float JUPITER_ORBITAL_PERIOD_YEAR;
extern const float SATURN_ORBITAL_PERIOD_YEAR;
extern const float URANUS_ORBITAL_PERIOD_YEAR;
extern const float NEPTUNE_ORBITAL_PERIOD_YEAR;

#endif // SOLARSYSTEM_H
