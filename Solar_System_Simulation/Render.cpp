#include <header/solar.h>

// Constants
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera variables
Camera camera;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing variables
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f;

// OpenGL buffers
unsigned int VBO, VAO, EBO;

// Planet properties
const float SUN_ROTATION_AXIS = 7.25;
const float MERCURY_ROTATION_AXIS = 0.01;
const float VENUS_ROTATION_AXIS = 177.4;
const float EARTH_ROTATION_AXIS = 23.5;
const float MARS_ROTATION_AXIS = 25.2;
const float JUPITER_ROTATION_AXIS = 3.1;
const float SATURN_ROTATION_AXIS = 26.7;
const float URANUS_ROTATION_AXIS = 97.8;
const float NEPTUNE_ROTATION_AXIS = 28.3;

// distance, let 10.0 = distance from sun to neptune
const float SUN_RADIUS = 0.00155;
const float MERCURY_RADIUS = 0.0000054;
const float VENUS_RADIUS = 0.0000134;
const float EARTH_RADIUS = 0.0000142;
const float MARS_RADIUS = 0.0000075;
const float JUPITER_RADIUS = 0.000155;
const float SATURN_RADIUS = 0.000129;
const float URANUS_RADIUS = 0.000056;
const float NEPTUNE_RADIUS = 0.000055;

const float MERCURY_DISTANCE_FROM_SUN = 0.129;
const float VENUS_DISTANCE_FROM_SUN = 0.240;
const float EARTH_DISTANCE_FROM_SUN = 0.333;
const float MARS_DISTANCE_FROM_SUN = 0.506;
const float JUPITER_DISTANCE_FROM_SUN = 1.730;
const float SATURN_DISTANCE_FROM_SUN = 3.171;
const float URANUS_DISTANCE_FROM_SUN = 6.386;
const float NEPTUNE_DISTANCE_FROM_SUN = 10.0;

// period, let 1.0 = 1 year on earth
const float SUN_ROTATION_PERIOD_YEAR = 0.104;
const float MERCURY_ROTATION_PERIOD_YEAR = 0.16;
const float VENUS_ROTATION_PERIOD_YEAR = 0.67;
const float EARTH_ROTATION_PERIOD_YEAR = 0.00274;
const float MARS_ROTATION_PERIOD_YEAR = 0.00282;
const float JUPITER_ROTATION_PERIOD_YEAR = 0.00114;
const float SATURN_ROTATION_PERIOD_YEAR = 0.00122;
const float URANUS_ROTATION_PERIOD_YEAR = 0.00196;
const float NEPTUNE_ROTATION_PERIOD_YEAR = 0.00184;

const float MERCURY_ORBITAL_PERIOD_YEAR = 0.24;
const float VENUS_ORBITAL_PERIOD_YEAR = 0.62;
const float EARTH_ORBITAL_PERIOD_YEAR = 1.0;
const float MARS_ORBITAL_PERIOD_YEAR = 1.88;
const float JUPITER_ORBITAL_PERIOD_YEAR = 11.86;
const float SATURN_ORBITAL_PERIOD_YEAR = 29.46;
const float URANUS_ORBITAL_PERIOD_YEAR = 84.01;
const float NEPTUNE_ORBITAL_PERIOD_YEAR = 164.8;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset, false);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// draw each sphere using its own texture and coordinates
// ----------------------------------------------------------------------
void drawSphere(Sphere sphere, Shader shaderProgram, bool wireframe)
{
    shaderProgram.use();
    glActiveTexture(sphere.getTextureGL());
    glBindTexture(GL_TEXTURE_2D, sphere.getTexture());
    shaderProgram.setInt("material.diffuse", sphere.getTextureInt());
    shaderProgram.setInt("material.specular", sphere.getTextureInt());

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // set VBO from vetices generated from Sphere
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sphere.getInterleavedVertexSize(), sphere.getInterleavedVertices(), GL_STATIC_DRAW);

    // set EBO from indices generated from Sphere
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.getIndexSize(), sphere.getIndices(), GL_STATIC_DRAW);

    // set stride from sphere
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sphere.getInterleavedStride(), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sphere.getInterleavedStride(), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sphere.getInterleavedStride(), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // draw
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

// load texture and handle error
// ----------------------------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}