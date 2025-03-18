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
#include "ParticleSystem3D/PS3ParticleSystem.hpp"
#include "ParticleSystem3D/PS3ParticleSystemGPU.hpp"
#include "ParticleSystem3D/PS3ParticleSystemCPU.hpp"
#include "Common/Background.hpp"
#include "Common/KeyPoint.hpp"
#include "ParticleSystem3D/Renderer/PS3Trail.hpp"
#include "Common/Sphere.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, PS3ParticleSystemPtr ps);

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

// camera
CameraPtr camera = std::make_shared<Camera>();
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;



int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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
    
    // 启用混合
    glEnable(GL_BLEND);
    // 设置混合方程
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 启用point_size
    glEnable(GL_PROGRAM_POINT_SIZE);
    
    // Background
    BackgroundPtr background = std::make_shared<Background>();
    background->SetCamera(camera);
    
    // KeyPoint
    KeyPointPtr keyPoint = std::make_shared<KeyPoint>();
    keyPoint->SetCamera(camera);
    
    // ParticleSystem2D
//    ParticleSystemQuadPtr particleSystem = ParticleSystemQuad::Create();
    
    // Point (parent node)
//    PointPtr point = Point::Create(10.0f, vec4(0.0, 1.0, 0.0, 1.0));
//    point->SetPosition3D(vec3(2.0f,2.0f,0.0f));
//    point->AddChild(std::static_pointer_cast<Node>(particleSystem));
//    particleSystem->SetPosition3D(vec3(-4.0, -4.0f,0.0f));
    
    // ParticleSystem3D
    PS3ParticleSystemGPUPtr particleSystem = std::make_shared<PS3ParticleSystemGPU>(20000);
    particleSystem->_renderer->_model->_renderer->SetCamera(camera);
    particleSystem->_shapeModule->_emitterRenderer->SetCamera(camera);
    particleSystem->SetRotation(vec3(0, 0, 45));
    PS3ParticleSystemGPUPtr subSystem = std::make_shared<PS3ParticleSystemGPU>(5000);
    subSystem->_renderer->_model->_renderer->SetCamera(camera);
    subSystem->_shapeModule = PS3CircleEmitter::CreateCircleEmitter(ArcMode::EVEN, 0, 360, CurveRange::CreateCurveByConstant(90), 0.00001, 0, 5, subSystem.get());
    subSystem->_isEmitting = false;
    subSystem->SetRotation(vec3(45, 0, 0));
    subSystem->_isSubEmitter = true;
        ColorKey colorKey1 = {vec3(1.0f, 0.0f, 0.0f), 0.0f};
        ColorKey colorKey2 = {vec3(0.0f, 0.0f, 1.0f), 1.0f};
        AlphaKey alphaKey1 = {1.0f, 0.0f};
        AlphaKey alphaKey2 = {1.0f, 1.0f};
        std::vector<ColorKey> colorKeys = {colorKey1, colorKey2};
        std::vector<AlphaKey> alphaKeys = {alphaKey1, alphaKey2};
        GradientPtr gradient = Gradient::CreateByColorKeyAndAlphaKey(colorKeys, alphaKeys);
        GradientRangePtr gradientRange = GradientRange::CreateByOneGradient(gradient);
     subSystem->_overtimeModules["colorOvertime"] = std::make_shared<PS3ColorOvertime>(gradientRange);
    subSystem->_overtimeModules["velocityOvertime"]->_enable = false;
    // subSystem->_renderer->InitUniform(); // hack：手动吧overtime的module再更新一次
//    particleSystem->AddSubEmitter({
//        EventType::DEATH,
//        subSystem
//    });
    
    // 球体
    SpherePtr sphere = std::make_shared<Sphere>(0.5f, 72, 36);
    sphere->SetPosition3D(vec3(0, 0, 0));
    sphere->_sphereRenderer->SetCamera(camera);
    
    // 构建父子关系
    // sphere->AddChild(particleSystem);
    
    // particleSystem->Play();
    
    // trail
//    PS3TrailPtr trail = std::make_shared<PS3Trail>();
//    trail->_renderer->SetCamera(camera);
//    particleSystem->SetTrailModule(trail);
//    
//    PS3TrailPtr trail2 = std::make_shared<PS3Trail>();
//    trail2->_renderer->SetCamera(camera);
//    subSystem->SetTrailModule(trail2);
    
//    float vertices[] = {
//        // 位置              // 颜色              // 纹理坐标
//        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 左下角
//         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下角
//         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f  // 顶部
//    };
//    Texture2DPtr texture1 = std::make_shared<Texture2D>("/Users/evanbfeng/work/resource/textures/grass.png");
//    CurveRangePtr cr = CurveRange::CreateCurveByConstant(1.0);
//    Texture2DPtr texture2 = CurveRange::PackCurveRangeN(32, cr);
//    if (texture1->textureID == 0 || texture2->textureID == 0) {
//        std::cerr << "Error: Failed to load texture!" << std::endl;
//    }
//    
//    // 创建 VBO 和 VAO
//    unsigned int VBO, VAO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//
//    // 绑定 VAO
//    glBindVertexArray(VAO);
//
//    // 绑定 VBO 并传递数据
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    // 设置顶点属性指针
//    // 位置属性
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    // 颜色属性
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//    // 纹理坐标属性
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//    glEnableVertexAttribArray(2);
//
//    // 解绑 VBO 和 VAO
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//    auto shader = std::make_shared<Shader>("./shader_file/shader.vs", "./shader_file/shader.fs");
//    
//    GLint loc1 = glGetUniformLocation(shader->ID, "texture1");
//    if (loc1 == -1) {
//        std::cerr << "Error: Uniform 'texture1' not found!" << std::endl;
//    }
//    GLint loc2 = glGetUniformLocation(shader->ID, "texture2");
//    if (loc2 == -1) {
//        std::cerr << "Error: Uniform 'texture2' not found!" << std::endl;
//    }
//    // 绑定纹理到纹理单元
//
//    shader->use();
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, texture1->textureID);
//    glUniform1i(loc1, 0);
//
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D, texture2->textureID);
//    glUniform1i(loc2, 1);
//                  
//    GLenum err;
//    while ((err = glGetError()) != GL_NO_ERROR) {
//        std::cerr << "OpenGL error: " << err << std::endl;
//    }
//    
//    // 创建 Transform Feedback 对象
//    unsigned int TBO;
//    glGenTransformFeedbacks(1, &TBO);
//    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TBO);
//
//    // 创建一个新的 VBO 用于存储 Transform Feedback 捕获的数据
//    unsigned int feedbackVBO;
//    glGenBuffers(1, &feedbackVBO);
//    glBindBuffer(GL_ARRAY_BUFFER, feedbackVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), nullptr, GL_STATIC_READ);
//
//    // 将 feedbackVBO 绑定到 Transform Feedback 对象
//    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, feedbackVBO);
//    
//    // 设置 Transform Feedback 捕获的变量
//    const char* feedbackVaryings[] = { "outPos" };
//    glTransformFeedbackVaryings(shader->ID, 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);
//
//    // 重新链接着色器程序
//    glLinkProgram(shader->ID);
    
    // 再来三个ps
//    PS3ParticleSystemPtr particleSystem2 = std::make_shared<PS3ParticleSystem>(10000);
//    particleSystem2->_renderer->_model->_renderer->SetCamera(camera);
//    particleSystem2->_shapeModule->_emitterRenderer->SetCamera(camera);
//    particleSystem2->SetRotation(vec3(0, 0, 0));
//    particleSystem2->SetPosition3D(vec3(0, 0, 0));
//    
//    PS3ParticleSystemPtr particleSystem3 = std::make_shared<PS3ParticleSystem>(10000);
//    particleSystem3->_renderer->_model->_renderer->SetCamera(camera);
//    particleSystem3->_shapeModule->_emitterRenderer->SetCamera(camera);
//    particleSystem3->SetRotation(vec3(0, 0, 0));
//    particleSystem3->SetPosition3D(vec3(5, 0, 0));
//    
//    PS3ParticleSystemPtr particleSystem4 = std::make_shared<PS3ParticleSystem>(10000);
//    particleSystem4->_renderer->_model->_renderer->SetCamera(camera);
//    particleSystem4->_shapeModule->_emitterRenderer->SetCamera(camera);
//    particleSystem4->SetRotation(vec3(0, 0, 0));
//    particleSystem4->SetPosition3D(vec3(0, 0, 5));
//    
    //particleSystem->Play();
//    particleSystem2->Play();
//    particleSystem3->Play();
//    particleSystem4->Play();
    // subSystem->Play();
    
    // 帧率计算变量
    float lastFrame = glfwGetTime();
    int frameCount = 0;

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window, particleSystem);

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
//        particleSystem->Update(deltaTime);
//        point->Move(vec3(1.0f,0.0f,0.0f), deltaTime);
//        particleSystem->Move(vec3(-1.0f, 0.0f ,0.0f), deltaTime);
        //particleSystem->Move(vec3(1.0f,0.0f,0.0f), deltaTime * 0.5);
        particleSystem->Update(deltaTime);
        sphere->Move(deltaTime);
//        particleSystem2->Update(deltaTime);
//        particleSystem3->Update(deltaTime);
//        particleSystem4->Update(deltaTime);

        // render
        // ------
        glClearColor(0.f, 0.f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        background->Render();
        keyPoint->Render();
        
        sphere->Render();
        
//        shader->use();
//        // 绑定 VAO
//        glBindVertexArray(VAO);
//        
//        // 开始 Transform Feedback
//        glBeginTransformFeedback(GL_TRIANGLES);
//
//        // 绘制三角形
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//        
//        // 结束 Transform Feedback
//        glEndTransformFeedback();
//        
//        // 从 feedbackVBO 中读取捕获的顶点数据
//        glBindBuffer(GL_ARRAY_BUFFER, feedbackVBO);
//        float* feedbackData = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
//
//        // 输出捕获的顶点数据
//        if (feedbackData) {
//            std::cout << "Frame " << currentFrame << " captured positions:" << std::endl;
//            for (int i = 0; i < 3; ++i) { // 3 个顶点
//                std::cout << "Vertex " << i << ": ("
//                          << feedbackData[i * 3] << ", " // 位置 x
//                          << feedbackData[i * 3 + 1] << ", " // 位置 y
//                          << feedbackData[i * 3 + 2] << ")" // 位置 z
//                          << std::endl;
//            }
//            glUnmapBuffer(GL_ARRAY_BUFFER);
//        }
//
//        // 解绑 VAO
//        glBindVertexArray(0);
        
//        particleSystem->Draw();
//        point->Draw();
        particleSystem->Render();
//        particleSystem2->Render();
//        particleSystem3->Render();
//        particleSystem4->Render();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, PS3ParticleSystemPtr ps)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboardMovement(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboardMovement(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboardMovement(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboardMovement(RIGHT, deltaTime);
    
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        ps->Play();
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        ps->Pause();
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        ps->Restart();
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        ps->Stop();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

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

    camera->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->ProcessMouseScroll(static_cast<float>(yoffset));
}
