#include <header/solar.h>

int solarScaledDistance()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Solar_System", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader program
    // ------------------------------------
    Shader planetShader("solar.vs", "solar.fs"), sunShader("solar.vs", "sun.fs");

    // load and create a texture 
    // -------------------------
    stbi_set_flip_vertically_on_load(true);
    unsigned int sunTexture = loadTexture("sun.jpg"),
        mercuryTexture = loadTexture("mercury.jpg"),
        venusTexture = loadTexture("venus.jpg"),
        earthTexture = loadTexture("earth.jpg"),
        marsTexture = loadTexture("mars.jpg"),
        jupiterTexture = loadTexture("jupiter.jpg"),
        saturnTexture = loadTexture("saturn.jpg"),
        saturnRingTexture = loadTexture("saturnRing.jpg"),
        uranusTexture = loadTexture("uranus.jpg"),
        uranusRingTexture = loadTexture("uranusRing.jpg"),
        neptuneTexture = loadTexture("neptune.jpg");
    
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // create stars

    // scale radius for visibility
    float mult = 10000.0;
    Sphere sun(SUN_RADIUS * mult / 8), // true scale sun is too big
        mercury(MERCURY_RADIUS * mult),
        venus(VENUS_RADIUS * mult),
        earth(EARTH_RADIUS * mult),
        mars(MARS_RADIUS * mult),
        jupiter(JUPITER_RADIUS * mult),
        saturn(SATURN_RADIUS * mult),
        uranus(URANUS_RADIUS * mult),
        neptune(NEPTUNE_RADIUS * mult);

    // set tecture of each star
    sun.setTexture(sunTexture);
    sun.setTextureInt(0);
    sun.setTextureGL(GL_TEXTURE0);

    mercury.setTexture(mercuryTexture);
    mercury.setTextureInt(1);
    mercury.setTextureGL(GL_TEXTURE1);

    venus.setTexture(venusTexture);
    venus.setTextureInt(2);
    venus.setTextureGL(GL_TEXTURE2);

    earth.setTexture(earthTexture);
    earth.setTextureInt(3);
    earth.setTextureGL(GL_TEXTURE3);

    mars.setTexture(marsTexture);
    mars.setTextureInt(4);
    mars.setTextureGL(GL_TEXTURE4);

    jupiter.setTexture(jupiterTexture);
    jupiter.setTextureInt(5);
    jupiter.setTextureGL(GL_TEXTURE5);

    saturn.setTexture(saturnTexture);
    saturn.setTextureInt(6);
    saturn.setTextureGL(GL_TEXTURE6);

    uranus.setTexture(uranusTexture);
    uranus.setTextureInt(8);
    uranus.setTextureGL(GL_TEXTURE8);

    neptune.setTexture(neptuneTexture);
    neptune.setTextureInt(10);
    neptune.setTextureGL(GL_TEXTURE10);

    // set uniform of planetShader
    planetShader.use();

    // shininess for specular light
    planetShader.setFloat("material.shininess", 32.0f);

    // pointlight properties
    planetShader.setVec3("pointLights.position", 0.0f, 0.0f, 0.0f);
    planetShader.setVec3("pointLights.ambient", 0.2f, 0.2f, 0.2f);
    planetShader.setVec3("pointLights.diffuse", 0.5f, 0.5f, 0.5f);
    planetShader.setVec3("pointLights.specular", 1.0f, 1.0f, 1.0f);
    planetShader.setFloat("pointLights.constant", 1.0f);
    planetShader.setFloat("pointLights.linear", 0.09f);
    planetShader.setFloat("pointLights.quadratic", 0.032f);
    planetShader.setFloat("pointLights.linear", 0.0014f);
    planetShader.setFloat("pointLights.quadratic", 0.000007f);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------ 
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // uncomment this line if you want black background (more space-like)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        planetShader.use();

        // camera position
        planetShader.setVec3("viewPos", camera.Position);

        // projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();

        // model transformation (different for each object), then draw
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)(360 - SUN_ROTATION_AXIS), glm::vec3(1.0f, 0.0f, 1.0f));
        model = glm::rotate(model, (float)90.0, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() / SUN_ROTATION_PERIOD_YEAR, glm::vec3(0.0f, 1.0f, 0.0f));
        sunShader.use();
        sunShader.setMat4("projection", projection);
        sunShader.setMat4("view", view);
        sunShader.setMat4("model", model);
        drawSphere(sun, sunShader, false);

        planetShader.use();
        planetShader.setMat4("projection", projection);
        planetShader.setMat4("view", view);

        // scale distance for visibility
        float distanceMult = 16.0;

        model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() / MERCURY_ORBITAL_PERIOD_YEAR, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(distanceMult * MERCURY_DISTANCE_FROM_SUN, 0.0f, 0.0f));
        model = glm::rotate(model, (float)(360 - MERCURY_ROTATION_AXIS), glm::vec3(1.0f, 0.0f, 1.0f));
        model = glm::rotate(model, (float)90.0, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() / MERCURY_ROTATION_PERIOD_YEAR, glm::vec3(0.0f, 1.0f, 0.0f));
        planetShader.setMat4("model", model);
        drawSphere(mercury, planetShader, false);

        model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() / VENUS_ORBITAL_PERIOD_YEAR, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(distanceMult * VENUS_DISTANCE_FROM_SUN, 0.0f, 0.0f));
        model = glm::rotate(model, (float)(360 - VENUS_ROTATION_AXIS), glm::vec3(1.0f, 0.0f, 1.0f));
        model = glm::rotate(model, (float)90.0, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, -(float)glfwGetTime() / VENUS_ROTATION_PERIOD_YEAR, glm::vec3(0.0f, 1.0f, 0.0f));
        planetShader.setMat4("model", model);
        drawSphere(venus, planetShader, false);

        model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() / EARTH_ORBITAL_PERIOD_YEAR, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(distanceMult * EARTH_DISTANCE_FROM_SUN, 0.0f, 0.0f));
        model = glm::rotate(model, (float)(360 - EARTH_ROTATION_AXIS), glm::vec3(1.0f, 0.0f, 1.0f));
        model = glm::rotate(model, (float)90.0, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() / EARTH_ROTATION_PERIOD_YEAR, glm::vec3(0.0f, 1.0f, 0.0f));
        planetShader.setMat4("model", model);
        drawSphere(earth, planetShader, false);

        model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() / MARS_ORBITAL_PERIOD_YEAR, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(distanceMult * MARS_DISTANCE_FROM_SUN, 0.0f, 0.0f));
        model = glm::rotate(model, (float)(360 - MARS_ROTATION_AXIS), glm::vec3(1.0f, 0.0f, 1.0f));
        model = glm::rotate(model, (float)90.0, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() / MARS_ROTATION_PERIOD_YEAR, glm::vec3(0.0f, 1.0f, 0.0f));
        planetShader.setMat4("model", model);
        drawSphere(mars, planetShader, false);

        // after mars, all star distance from sun will be halved (so that they are not too far from sun)

        model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() / JUPITER_ORBITAL_PERIOD_YEAR, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3((distanceMult / 2) * JUPITER_DISTANCE_FROM_SUN, 0.0f, 0.0f));
        model = glm::rotate(model, (float)(360 - JUPITER_ROTATION_AXIS), glm::vec3(1.0f, 0.0f, 1.0f));
        model = glm::rotate(model, (float)90.0, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() / JUPITER_ROTATION_PERIOD_YEAR, glm::vec3(0.0f, 1.0f, 0.0f));
        planetShader.setMat4("model", model);
        drawSphere(jupiter, planetShader, false);

        model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() / SATURN_ORBITAL_PERIOD_YEAR, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3((distanceMult / 2) * SATURN_DISTANCE_FROM_SUN, 0.0f, 0.0f));
        model = glm::rotate(model, (float)(360 - SATURN_ROTATION_AXIS), glm::vec3(1.0f, 0.0f, 1.0f));
        model = glm::rotate(model, (float)90.0, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() / SATURN_ROTATION_PERIOD_YEAR, glm::vec3(0.0f, 1.0f, 0.0f));
        planetShader.setMat4("model", model);
        drawSphere(saturn, planetShader, false);

        model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() / URANUS_ORBITAL_PERIOD_YEAR, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3((distanceMult / 2) * URANUS_DISTANCE_FROM_SUN, 0.0f, 0.0f));
        model = glm::rotate(model, (float)(360 - URANUS_ROTATION_AXIS), glm::vec3(1.0f, 0.0f, 1.0f));
        model = glm::rotate(model, (float)90.0, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, -(float)glfwGetTime() / URANUS_ROTATION_PERIOD_YEAR, glm::vec3(0.0f, 1.0f, 0.0f));
        planetShader.setMat4("model", model);
        drawSphere(uranus, planetShader, false);

        model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() / NEPTUNE_ORBITAL_PERIOD_YEAR, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3((distanceMult / 2) * NEPTUNE_DISTANCE_FROM_SUN, 0.0f, 0.0f));
        model = glm::rotate(model, (float)(360 - NEPTUNE_ROTATION_AXIS), glm::vec3(1.0f, 0.0f, 1.0f));
        model = glm::rotate(model, (float)90.0, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() / NEPTUNE_ROTATION_PERIOD_YEAR, glm::vec3(0.0f, 1.0f, 0.0f));
        planetShader.setMat4("model", model);
        drawSphere(neptune, planetShader, false);
        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}