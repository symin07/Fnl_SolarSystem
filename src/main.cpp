
#include <spdlog/spdlog.h>
#include <glad/glad.h>          // Before glfw3.h, if not error 
#include <GLFW/glfw3.h>
#include <typeinfo>
#include "context.h"


void OnFramebufferSizeChange(GLFWwindow* window, int width, int height) { 
    SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height); 
    glViewport(0, 0, width, height);
}

void OnKeyEvent(GLFWwindow* window,
int key, int scancode, int action, int mods) {
    SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}", 
    key, scancode,
    action == GLFW_PRESS ? "Pressed" :
    action == GLFW_RELEASE ? "Released" :
    action == GLFW_REPEAT ? "Repeat" : "Unknown",
    mods & GLFW_MOD_CONTROL ? "C" : "-",
    mods & GLFW_MOD_SHIFT ? "S" : "-",
    mods & GLFW_MOD_ALT ? "A" : "-");
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { 
        glfwSetWindowShouldClose(window, true);
    } 
}

int main(int argc, const char** argv) 
{ 
    SPDLOG_INFO("Start Program");
    // Itialize glfw library, if fails, it will exit after printing error
    SPDLOG_INFO("Initialize glfw");
     if (!glfwInit()) {
        const char* description = nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("failed to initialize glfw: {}", description);
        return -1;
    }

    // add OpenGL version which you wanna make.
   
   
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    // Create a glfw window, if fails, it will exit after printing error
    SPDLOG_INFO("Create glfw window");
    auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME,
      nullptr, nullptr);
    if (!window) {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }

    // Use a OpenGL context which is created while creating a window
    glfwMakeContextCurrent(window); 

    // OpenGL function loading using glad
    if( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){       // OpenGL function Load
        SPDLOG_ERROR("failed to initialize glad");
        glfwTerminate();
        return -1;
    }
    
    auto glVersion = glGetString(GL_VERSION);
    SPDLOG_INFO("TYPE of glVersion: {}", typeid(glVersion).name());
    //SPDLOG_INFO("OpenGL context version: {}", glVersion);   --- It does not work, b/c the current SPDLOG version is 1.12.0
    //                                                            It worked well in version 1.8.5
    //                                                            But, after that it is excluded. Expecting that unsigned char* is 
    //                                                            generally used to represent binary data type of byte.
    SPDLOG_INFO("OpenGL context version: {}", (char*)glVersion);
    /** Refactoring, This part is moved to context.cpp
    // // After loading GL function ---> GLAD
    // // auto vertexShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);          // retrun shaderUprt(unique pointer)                              
    // ShaderPtr vertexShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);        // So, auto --> change into SharedPtr, b/c sharedPtr = Uptr --> atuomatically changed 
    // ShaderPtr fragmentShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
    // SPDLOG_INFO("vertex shader id: {}", vertexShader->Get());
    // SPDLOG_INFO("fragment shader id: {}", fragmentShader->Get());


    // auto program = Program::Create({fragmentShader, vertexShader});    // vector type 
    // SPDLOG_INFO("Program id: {}", program->Get());    
    // set context, register defined callbacks to window
    */
    auto context = Context::Create();
    if( !context ){
        SPDLOG_ERROR("failed to create context");
        glfwTerminate();
        return -1;
    }
    



    OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);                     
    glfwSetKeyCallback(window, OnKeyEvent);                     
    
    
    // execute glfw loop, if click "close button", terminated normally. 
    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        //glClear(GL_COLOR_BUFFER_BIT);   // GL_COLOR_BUFFER_BIT: SCREEN BUFFER ON DISPAY -> IT CLEARS SCREEN
        context->Render();              // changed from glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);        // DOUBLE BUFFERING
    } 
    context.reset();                    // or context = nullptr;
    glfwTerminate();
    return 0;
    
}