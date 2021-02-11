#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <learnopengl/shader_m.h>
#include <iostream>
#include <learnopengl/MyCamera.h>
#include <learnopengl/2DTexture.h>
#include <learnopengl/model.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mod);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool firstMouse = true;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Globals ______________________________
glm::vec3 pointLightPosition;
bool isNight = true;
Camera camera = Camera();
bool update = false;
float rotationSpeed = 0.001f;
glm::vec3 sunColor = glm::vec3(1.0f);
//__________________________________

int main()
{

    std::cerr << "Type ""O"" for options" << std::endl;

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
    glfwSetKeyCallback(window, keyboard_callback);




    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    //Zemlja, vertices
    float vertices[] = {
            // vertices           //normale           // Texcoords
            -0.5f, -0.01f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
            0.5f, -0.01f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
            0.5f,  0.01f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            0.5f,  0.01f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.01f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.01, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.01f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.01f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
            0.5f,  0.01f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            0.5f,  0.01f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.01f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.01f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f,  0.01f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.01f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.01f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.01f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.01f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.01f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            0.5f,  0.01f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.01f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.01f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.01f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.01f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            0.5f,  0.01f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    };

    //Donji deo Zemlje, vertices
    float bottom_vertices[] = {
            -0.5f, -0.01f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.01f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.01f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.01f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f
    };

    //Gorlji deo Zemlje, veritces
    float top_vertices[] = {
            -0.5f,  0.01f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.01f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
            0.5f,  0.01f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.01f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    };

    unsigned int bottom_indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    unsigned int top_indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    float sun_vertices[] = {
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
    };
    unsigned int sunVBO, sunVAO;
    glGenVertexArrays(1, &sunVAO);
    glGenBuffers(1, &sunVBO);

    glBindVertexArray(sunVAO);

    glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sun_vertices), sun_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Earth
    unsigned int VBO, VAO, VBO1, VAO1, EBO1, VBO2, VAO2, EBO2;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_UNSIGNED_INT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //Donji deo zemlje, object attributes
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &EBO1);

    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bottom_vertices), bottom_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bottom_indices), bottom_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //Dodajem Gornji deo zemlje
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO2);

    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(top_vertices), top_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(top_indices), top_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);




    Shader earthShader("resources/shaders/earth.vs",
                     "resources/shaders/earth.fs");

    Shader sunShader("resources/shaders/sun.vs",
                     "resources/shaders/sun.fs");

    Shader earthModelShader("resources/shaders/earthModel.vs",
                            "resources/shaders/earthModel.fs");

    Tex2D mapDiffuseEarth = Tex2D("resources/textures/earth.jpg");
    Tex2D mapEarthSpecular = Tex2D("resources/textures/earth_specular_improved.jpg");
    Tex2D mapSeaSpecular = Tex2D("resources/textures/signature.jpg");
    Tex2D mapDiffuseSea = Tex2D("resources/textures/sea.jpg");

    earthShader.use();
    earthShader.setInt("material.diffuse", 0);
    earthShader.setInt("material.specular", 1);

    // resources/objects/backpack/backpack.obj
    // resources/objects/Earth/world/world.obj

    Model earthModel = Model("resources/objects/Earth/world/world.obj");

    glEnable(GL_DEPTH_TEST);



    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()),(float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.getCameraView();
        glm::mat4 model = glm::mat4(1.0f);


        if(!update){

            earthShader.use();

            model = glm::mat4(1.0f);

            earthShader.setMat4("projection", projection);
            earthShader.setMat4("view", view);
            earthShader.setMat4("model", model);


            earthShader.setVec3("viewPos", camera.getPosition());
            pointLightPosition = glm::vec3(2.0f * glm::sin((float)glfwGetTime() * 0.5), 1.0f, 1.05f * glm::cos((float)glfwGetTime() * 0.5));


            //Postavljamo materijal
            earthShader.setFloat("material.shininess", 12.0f);


            //Podesavamje svetla___________________

            //Dierkciono svetlo
            earthShader.setVec3("spotLight.direction", glm::vec3(0.0f, 1.0f, 0.0f));

            earthShader.setVec3("spotLight.ambient", glm::vec3(0.0f));
            earthShader.setVec3("spotLight.diffuse", glm::vec3(0.5f));
            earthShader.setVec3("spotLight.specular", glm::vec3(0.5f));

            //Point svetla
            earthShader.setVec3("pointLight.position", pointLightPosition);

            earthShader.setVec3("pointLight.ambient", glm::vec3(0.1f));
            earthShader.setVec3("pointLight.diffuse", glm::vec3(sunColor));
            earthShader.setVec3("pointLight.specular", glm::vec3(0.9f));

            earthShader.setFloat("pointLight.constant", 1.0f);
            earthShader.setFloat("pointLight.linear", 0.09f);
            earthShader.setFloat("pointLight.quadratic", 0.032f);
            //_____________________________________


            //Zemlja
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mapDiffuseSea.getId());

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mapEarthSpecular.getId());

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 24);

            //Zemlja donja strana
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mapDiffuseSea.getId());

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mapSeaSpecular.getId());
            glBindVertexArray(VAO1);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            //Zemlja gornja strana
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mapDiffuseEarth.getId());

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mapEarthSpecular.getId());
            glBindVertexArray(VAO2);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            //Drawing sun

            sunShader.use();

            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPosition);
            model = glm::scale(model, glm::vec3(0.2));
            sunShader.setMat4("model", model);
            sunShader.setMat4("view", view);
            sunShader.setMat4("projection", projection);
            sunShader.setVec3("outColor", sunColor);

            glBindVertexArray(sunVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }
        else{

            //Zemlja kruzna


            earthModelShader.use();


            earthModelShader.setVec3("viewPosition", camera.getPosition());

            model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(1.25f)); //Ne moze da se skalira ocigledno

            // x = rsinFcosG y = rsinFsinG z = cosG

            model = glm::translate(model, glm::vec3(1.9f * glm::sin((float)glfwGetTime() * rotationSpeed),
                                                       0.0f,
                                                       1.9f * glm::cos((float)glfwGetTime() * rotationSpeed)));

            //model = glm::rotate(model, (float)(glfwGetTime() * 0.1), glm::vec3(1.0, 0.0, 0.0));

            model = glm::rotate(model, 3.14159f, glm::vec3(1.0, 0.0, 0.0));

            model = glm::rotate(model, (float)(glfwGetTime() * rotationSpeed * 3.65f), glm::vec3(0.0, 0.5, -0.0));

            earthModelShader.setMat4("model", model);
            earthModelShader.setMat4("view", view);
            earthModelShader.setMat4("projection", projection);

            earthModelShader.setVec3("pointLight.position", glm::vec3(0.0f, 0.0f, 0.0f));

            earthModelShader.setVec3("pointLight.ambient", glm::vec3(0.2));
            earthModelShader.setVec3("pointLight.diffuse",glm::vec3(0.6f, 0.5f, 0.6f));
            earthModelShader.setVec3("pointLight.specular",glm::vec3(0.9f));

            earthModelShader.setFloat("pointLight.constant", 1.0f);
            earthModelShader.setFloat("pointLight.linear", 0.09f);
            earthModelShader.setFloat("pointLight.quadratic", 0.032f);

            earthModel.Draw(earthModelShader);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        update = true;

    }

    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        update = false;
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        if(rotationSpeed < 0.002f){
            rotationSpeed = 0.001f;
        }else{
            rotationSpeed -= 0.001f;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        if(rotationSpeed > 5.0f){
            rotationSpeed = 5.0f;
        }else{
            rotationSpeed += 0.001f;
        }
    }

    if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        sunColor = glm::vec3(1.0, 0.0, 0.0);
    }
    if(glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS){
        sunColor = glm::vec3(0.0, 1.0, 0.0);
    }
    if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS){
        sunColor = glm::vec3(0.0, 0.0, 1.0);
    }
    if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
        sunColor = glm::vec3(1.0, 1.0, 1.0);
    }

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

void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mod){
    if(key == GLFW_KEY_O && action == GLFW_PRESS){
        std::cerr   << "\t  OPTIONS\n"
                    << "ESC\t\t\t\tEXIT\n"
                    << "N   \t\t\tNIGHT/DAY\n"
                    << "\n FLAT EARTH_____________\n"
                    << "R\t\t\t\tSUN COLOR RED\n"
                    << "G\t\t\t\tSUN COLOR GREEN\n"
                    << "B\t\t\t\tSUN COLOR RED BLUE\n"
                    << "F\t\t\t\tSUN COLOR WHITE"
                    << "ARROW_UP\t\tUPGRADE\n"
                    << "\n SPHERICAL EARTH___________\n"
                    << "ARROW_LEFT\t\tSLOW DOWN AND GO BACK IN TIME\n"
                    << "ARROW_RIGHT\t\tFAST FORWARD\n"
                    << "ARROW_DOWN\t\tDOWNGRADE\n"

                << std::endl;
    }
    if(key == GLFW_KEY_N && action == GLFW_PRESS){
        if(isNight){
            glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
            isNight = false;
        }else{
            glClearColor(0.001f, 0.001f, 0.001f, 1.0f);
            isNight = true;
        }
    }
}