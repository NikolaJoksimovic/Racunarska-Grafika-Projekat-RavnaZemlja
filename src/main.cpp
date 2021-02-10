#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <learnopengl/shader_m.h>
#include <iostream>
#include <learnopengl/MyCamera.h>
#include <learnopengl/2DTexture.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool firstMouse = true;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera = Camera();

int main()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "~Flat Earth Pojecc~", NULL, NULL);
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




    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    glEnable(GL_DEPTH_TEST);

    float vertices[] = {
            -0.5f, -0.01f, -0.5f,  0.0f, 0.0f, // skorz donja
            0.5f, -0.01f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.01f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.01f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.01f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.01f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.01f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.01f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.01f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.01f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.01f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.01f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.01f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.01f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.01f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.01f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.01f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.01f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.01f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.01f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.01f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.01f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.01f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.01f,  0.5f,  1.0f, 0.0f,

            -0.5f,  0.01f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.01f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.01f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.01f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.01f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.01f, -0.5f,  0.0f, 1.0f
    };

    float bottom_vertices[] = {
            -0.5f, -0.01f, -0.5f,  0.0f, 1.0f, //ovo je ispod zemlje
            0.5f, -0.01f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.01f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.01f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.01f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.01f, -0.5f,  0.0f, 1.0f,
    };

    unsigned int VBO, VAO, VBO1, VAO1;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Donji deo zemlje
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bottom_vertices), bottom_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);





    Shader earthShader("/home/joksa/Desktop/ProjekatRG/Projekat/resources/shaders/earth.vs",
                     "/home/joksa/Desktop/ProjekatRG/Projekat/resources/shaders/earth.fs");
    Shader seaShader("/home/joksa/Desktop/ProjekatRG/Projekat/resources/shaders/earth.vs",
                     "/home/joksa/Desktop/ProjekatRG/Projekat/resources/shaders/earth.fs");

    Tex2D t0 = Tex2D("/home/joksa/Desktop/ProjekatRG/Projekat/resources/textures/earth.jpg");
    Tex2D t1 = Tex2D("/home/joksa/Desktop/ProjekatRG/Projekat/resources/textures/sea.jpg");

    earthShader.use();
    earthShader.setInt("t0", 0);

    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        earthShader.use();


        glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        earthShader.setMat4("projection", projection);

        glm::mat4 view = camera.getCameraView();
        earthShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        earthShader.setMat4("model", model);



        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, t0.getId());

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindTexture(GL_TEXTURE_2D, t1.getId());
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
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
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessScroll(yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}