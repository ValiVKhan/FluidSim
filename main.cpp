#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <ParticleManager.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void updateCircleCenters(std::vector<float>& vertices, ParticleManager& particleManager);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
const float CIRCLE_RADIUS = 0.05f;
const unsigned int numParticles = 1000;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Random Circles", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    std::vector<float> vertices;
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed for random number generation

    // const int numCircles = 100; // Define the number of circles
    // for (int i = 0; i < numCircles; ++i)
    // {
    //     float centerX = static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f; // Random x-coordinate in range [-1, 1]
    //     float centerY = static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f; // Random y-coordinate in range [-1, 1]

    //     for (int j = 0; j < 360; ++j)
    //     {
    //         float angle = static_cast<float>(j) * (2.0f * 3.14159265358979323846f) / 360;
    //         float x = CIRCLE_RADIUS * std::cos(angle) + centerX;
    //         float y = CIRCLE_RADIUS * std::sin(angle) + centerY;
    //         vertices.push_back(x);
    //         vertices.push_back(y);
    //         vertices.push_back(0.0f);
    //     }
    // }

    ParticleManager particleManager(numParticles);

    for (int i = 0; i < numParticles; ++i)
    {
        float centerX = (particleManager.getPositions()[2 * i] + particleManager.getLowerBoundX()) / (particleManager.getUpperBoundX() + particleManager.getLowerBoundX()) * 2.0f - 1.0f;
        float centerY = (particleManager.getPositions()[2 * i + 1] + particleManager.getLowerBoundY()) / (particleManager.getUpperBoundY() + particleManager.getLowerBoundY()) * 2.0f - 1.0f;

        for (int j = 0; j < 360; ++j)
        {
            float angle = static_cast<float>(j) * (2.0f * 3.14159265358979323846f) / 360;
            float x = CIRCLE_RADIUS * std::cos(angle) + centerX;
            float y = CIRCLE_RADIUS * std::sin(angle) + centerY;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(0.0f);
        }
    }


    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime(); 
 
        particleManager.step(currentTime - lastTime); //time step based on framerate

        updateCircleCenters(vertices, particleManager);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        lastTime = currentTime;


        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); 
        
        for (int i = 0; i < numParticles; ++i)
            glDrawArrays(GL_LINE_LOOP, i * 360, 360); // Draw each circle separately
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void updateCircleCenters(std::vector<float>& vertices, ParticleManager& particleManager)
{
    int numCircles = particleManager.getNumParticles();
    for (int i = 0; i < numCircles; ++i)
    {
        float centerX = (particleManager.getPositions()[2 * i] + particleManager.getLowerBoundX()) / (particleManager.getUpperBoundX() + particleManager.getLowerBoundX()) * 2.0f - 1.0f;
        float centerY = (particleManager.getPositions()[2 * i + 1] + particleManager.getLowerBoundY()) / (particleManager.getUpperBoundY() + particleManager.getLowerBoundY()) * 2.0f - 1.0f;

        for (int j = 0; j < 360; ++j)
        {
            float angle = static_cast<float>(j) * (2.0f * 3.14159265358979323846f) / 360;
            float x = CIRCLE_RADIUS * std::cos(angle) + centerX;
            float y = CIRCLE_RADIUS * std::sin(angle) + centerY;
            vertices[i * 360 * 3 + j * 3] = x;
            vertices[i * 360 * 3 + j * 3 + 1] = y;
        }
    }
}