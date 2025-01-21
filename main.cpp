#include "Head.h"
#include "Function/Shader.hpp"
#include "Function/Texture2D.hpp"
#include "Tool/PointGenerate.hpp"
#include "Tool/CatmullRom.hpp"
#include "Function/ToolFunction.h"
#include "ParticleSystem2D/ParticleSystemQuad.hpp"
#include "Parameters.hpp"
#include "Common/Node.hpp"
#include "Point/Point.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

int main()
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Particles", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // 启用混合
    glEnable(GL_BLEND);
    // 设置混合方程
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 启用point_size
    glEnable(GL_PROGRAM_POINT_SIZE);
    
    // ParticleSystem2D
    ParticleSystemQuadPtr particleSystem = ParticleSystemQuad::Create();
    
    // Point (parent node)
    PointPtr point = Point::Create(10.0f, vec4(0.0, 1.0, 0.0, 1.0));
    point->SetPosition3D(vec3(2.0f,2.0f,0.0f));
    point->AddChild(std::static_pointer_cast<Node>(particleSystem));
    particleSystem->SetPosition3D(vec3(-4.0, -4.0f,0.0f));
    
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        // 如果当前帧时间间隔小于目标帧时间间隔，则等待一段时间
        if (deltaTime < TARGET_FRAME_TIME) {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>((TARGET_FRAME_TIME - deltaTime) * 1000)));
            currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
        }
        lastFrame = currentFrame;
        
        // Update
        // ------
        particleSystem->Update(deltaTime);
        point->Move(vec3(1.0f,0.0f,0.0f), deltaTime);
        particleSystem->Move(vec3(-1.0f, 0.0f ,0.0f), deltaTime);

        // render
        // ------
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        particleSystem->Draw();
        point->Draw();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
