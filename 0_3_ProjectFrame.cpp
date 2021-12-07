// 0_2_HelloTriangle.cpp : Draw a triangle with texture
//

// HeadFile----------------------------d
#include <iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// GLM Library
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
// ---------------------------------------------------------------------

// Global Parameter-------------------
int WindowWidth = 800, WindowHeight = 600;
const char* WindowTitle = "HelloWorld";

float now; // Record current time from glfwGetTime()

float textureOpacity = 0.5;

Camera camera;

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

float lastX = 400, lastY = 300;
bool firstMouse = true;

// ---------------------------------------------------------------------

// Function----------------------------

//
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Keyboard Input of the window
void processInput(GLFWwindow* window);

// Mouse Input of the window
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Mouse Wheel Input of the window
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// ---------------------------------------------------------------------


int main()
{
    // Init-----------------------

    // GLFW Initiation
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // GLFW Window Initiation
    GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //glViewport(0, 0, WindowWidth, WindowHeight);

    // GLAD OpenGL Load
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // -----------------------------------------------------------------


    // Shader---------------------

    Shader ourShader("shader.vs.txt", "shader.fs.txt");

    // -----------------------------------------------------------------


    // Vertex-----------------------

    float vertices[] = {
        // Location           //Texture
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
      glm::vec3(0.0f,  0.0f,  0.0f),
      glm::vec3(2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),
      glm::vec3(1.5f,  2.0f, -2.5f),
      glm::vec3(1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
    };


    // Vertex Buffer Object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Activate the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Allocate space for the buffer

    // Vertex Array Object VAO1
    unsigned int VAO1; // VAO marks the data state of the current buffer target
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);

    // VAO1 Location Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // The first parameter means the location id
    glEnableVertexAttribArray(0);
    // VAO1 Texture Attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // -----------------------------------------------------------------

    // Texture--------------------

    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("box.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture 1" << std::endl;
    }
    stbi_image_free(data);


    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    data = stbi_load("cow.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture 2" << std::endl;
    }
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    // -----------------------------------------------------------------


    // MVP Transformation-----------------
    
    //glm::mat4 trans = glm::mat4(1.0f);

    //正射投影矩阵（left, right, bottom, top, near, far）
    //glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);

    //透视投影矩阵（FoV, width height ratio, near, far）
    //glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);


    glm::mat4 model =       glm::mat4(1.0f);
    glm::mat4 view =        glm::mat4(1.0f);
    glm::mat4 projection =  glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // Object View Transformation--
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -6.0f));
    
    // Camera View Transformation--
    
    // My Own camera LookAt-
    
    // Calculate Camera Coordinate System
    // ---------------
    //glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 6.0f); 
    //glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    //glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    //glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
    //glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    //glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    //// Remember that In glm we access elements as mat[col][row] due to column-major layout
    //view = glm::mat4(glm::vec4(cameraRight.x, cameraUp.x, cameraDirection.x, 0),
    //    glm::vec4(cameraRight.y, cameraUp.y, cameraDirection.y, 0),
    //    glm::vec4(cameraRight.z, cameraUp.z, cameraDirection.z, 0),
    //    glm::vec4(-cameraPos, 1));


    // OpenGL camera LookAt-
    // ---------------
    view = camera.GetViewMatrix();

    float AspectRatio = WindowWidth / WindowHeight;
    projection = glm::perspective(glm::radians(camera.Zoom), AspectRatio, 0.1f, 100.0f);


    // -----------------------------------------------------------------

    //glBindVertexArray(VAO1);

    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Render Loop-----------------------

    while (!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);

        // Render

        now = glfwGetTime();

        // GLFW Window Background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Texture Opacity
        ourShader.setFloat("textureOpacity", textureOpacity);

        // Camera Rotate around the boxes over time
        //float radius = 10.0f;
        //float camX = sin(glfwGetTime()) * radius;
        //float camZ = cos(glfwGetTime()) * radius;
        //glm::mat4 view = glm::mat4(1.0f);
        //view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

        view = camera.GetViewMatrix();
        projection = glm::perspective(camera.Zoom, AspectRatio, 0.1f, 100.0f);

        // MVP Transformation
        //model = glm::rotate(model, glm::radians(2.0f), glm::vec3(0.0, 0.0, 1.0));
        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        
        // Draw Element
        
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw ten boxes
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            if ( i==0 || !((i+1)%3)) {
                model = glm::rotate(model, glm::radians(angle + 100*now), glm::vec3(1.0f, 0.3f, 0.5f));
            }
            else{
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            }
            
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        
        // Check poll events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // -----------------------------------------------------------------


    // End----------------------------

    glDeleteVertexArrays(1, &VAO1);
    //glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;

    // -----------------------------------------------------------------
}



// Function-----------------------

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Texture Opacity Change
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (textureOpacity < 1 && textureOpacity >= -0.00001) { // why I set "textureOpacity >= -0.00001": there would be something wrong with floating-point comparisons
            textureOpacity += 0.05;
            std::cout << textureOpacity << std::endl;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (textureOpacity <= 1.00001 && textureOpacity > 0) {
            textureOpacity -= 0.05;
            std::cout << textureOpacity << std::endl;
        }
    }

    // Camera Movement Controled by Keyboard
    deltaTime = now - lastFrame;
    lastFrame = now;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessCameraMovement('n', deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessCameraMovement('s', deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessCameraMovement('w', deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessCameraMovement('e', deltaTime);

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
    float yoffset = -(ypos - lastY); // (xpos, ypos) relative to the upper left corner of the window
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);

}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


// ---------------------------------------------------------------------