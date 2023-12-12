
#include <spdlog/spdlog.h>
#include <glad/glad.h>          // Before glfw3.h, if not error 
#include <GLFW/glfw3.h>
#include <typeinfo>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "context.h"


void OnFramebufferSizeChange(GLFWwindow* window, int width, int height) { 
    SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
    // auto context = (Context*)glfwGetWindowUserPointer(window);
    auto context = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));  // more C++
    context->Reshape(width, height);

}
void OnCursorPos(GLFWwindow* window, double x, double y) {
    SPDLOG_INFO("Mouse On Cursor: ({} x {})", x, y);
    auto context = (Context*)glfwGetWindowUserPointer(window);
    context->MouseMove(x, y);
}

void OnMouseButton(GLFWwindow* window, int button, int action, int modifier) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, modifier); 
    auto context = (Context*)glfwGetWindowUserPointer(window);
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    context->MouseButton(button, action, x, y);
}

void OnCharEvent(GLFWwindow* window, unsigned int ch) {
    ImGui_ImplGlfw_CharCallback(window, ch);
}
void OnScroll(GLFWwindow* window, double xoffset, double yoffset) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
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

    auto imguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(imguiContext);
    ImGui_ImplGlfw_InitForOpenGL(window, false);        // callback을 자동으로 setting하면 true, 수동으로 setting할 계획
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplOpenGL3_CreateFontsTexture();
    ImGui_ImplOpenGL3_CreateDeviceObjects();
    
    
    auto context = Context::Create();
    if( !context ){
        SPDLOG_ERROR("failed to create context");
        glfwTerminate();
        return -1;
    }

    glfwSetWindowUserPointer(window, context.get());
    



    OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);                     
    glfwSetKeyCallback(window, OnKeyEvent);  
    glfwSetCursorPosCallback(window, OnCursorPos);                   
    glfwSetMouseButtonCallback(window, OnMouseButton);
    glfwSetScrollCallback(window, OnScroll);
    glfwSetCharCallback(window, OnCharEvent);
    
    // execute glfw loop, if click "close button", terminated normally. 
    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();              // 지금부터 새 rengering 프로그래밍
       
        context->ProcessInput(window);
        context->Render();              // changed from glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();                // context를 렌더링한 후 ImGui를 렌더링
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());     // 실제로 이걸로 그림
        glfwSwapBuffers(window);        // DOUBLE BUFFERING
    } 
    context.reset();                    // or context = nullptr;
    
    // 종료전 메모리 반환코드 추가
    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(imguiContext);
    glfwTerminate();
    return 0;
    
}