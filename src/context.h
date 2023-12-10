#ifndef __CONTEXT_H__
#define __CONTEXT_H__
#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "image.h"
#include "texture.h"



CLASS_PTR(Context) 
class Context {
public:
    static ContextUPtr Create(); 
    void Render();
    void ProcessInput(GLFWwindow* window);
    void Reshape(int width, int height);
    void MouseMove(double x, double y);
    void MouseButton(int button, int action, double x, double y);
private:
    Context() {}
    bool Init(); 
    ProgramUPtr m_program;
    ProgramUPtr m_simpleProgram;

    // uint32_t m_vertexArrayObject;   // member variable for VAO(Vertex Array Object)
    VertexLayoutUPtr m_vertexLayout;
    // uint32_t m_vertexBuffer;  // member variabe for VBO(Vertex Buffer Object)
    BufferUPtr m_vertexBuffer;    
    // uint32_t m_indexBuffer;
    BufferUPtr m_indexBuffer;
    TextureUPtr m_texture;         // variable to save texture ID
    TextureUPtr m_texture2;         // variable to save texture ID

    // animation
    bool m_animation { true };
    
    // clear color
    glm::vec4 m_clearColor {glm::vec4(0.1f, 0.2f, 0.3f, 0.0f)};

    // light parameter
    // light parameter
    struct Light {
       // glm::vec3 position { glm::vec3(3.0f, 3.0f, 3.0f) };
        glm::vec3 direction { glm::vec3(0.2f, -1.0f, -0.3f) };
        glm::vec3 ambient { glm::vec3(0.1f, 0.1f, 0.1f) };
        glm::vec3 diffuse { glm::vec3(0.5f, 0.5f, 0.5f) };
        glm::vec3 specular { glm::vec3(1.0f, 1.0f, 1.0f) };
    };
    Light m_light;
    // material parameter
    struct Material {
        TextureUPtr diffuse;
        TextureUPtr specular;
        float shininess { 32.0f };
    };
    Material m_material;

    // camera parameter
    float m_cameraPitch { 0.0f };
    float m_cameraYaw { 0.0f };
    bool m_cameraControl {false};                   // 현마우스의 상태를 나타낸다. 우클릭
    glm::vec2 m_prevMousePos {glm::vec2(0.0f)};     // MouseMove에서 쓰던 변수를 멤버변수로 한다. 
                                                    // MouseMove도 MouseButton도 조작해야하는 변수이기 때문이다.

    glm::vec3 m_cameraPos { glm::vec3(0.0f, 0.0f, 3.0f) };
    glm::vec3 m_cameraFront { glm::vec3(0.0f, 0.0f, -1.0f) };
    glm::vec3 m_cameraUp { glm::vec3(0.0f, 1.0f, 0.0f) };

    int m_width { WINDOW_WIDTH };
    int m_height { WINDOW_HEIGHT };
};



#endif // __CONTEXT_H__