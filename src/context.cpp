#include "context.h"


ContextUPtr Context::Create() {
    auto context = ContextUPtr(new Context()); 
    if (!context->Init())
        return nullptr;
    return std::move(context); 
}

bool Context::Init() {

    float vertices[] = {            // Data for the 3D VBO(Vertex Buffer Obejectt)
        //[x   y     z      r    g     b     s     t]
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,         // position 3, color 3
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    uint32_t indices[] = {      // note that we start from 0!
        0, 1, 3,                // first triangle
        1, 2, 3,                // second triangle
    };

     m_vertexLayout = VertexLayout::Create();
 
    // after refactoring, 3 lines above is changed into 1 line as below
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 32);


    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);               //  for x y x 
    m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float)*3); //  for r g b
    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float)*6); //  for s t
    

    // after refactoring, 3 lines above is changed into 1 line as below
    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, 
        GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

 
    ShaderPtr vertShader = Shader::CreateFromFile("./shader/texture.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/texture.fs", GL_FRAGMENT_SHADER); 
    if (!vertShader || !fragShader)
        return false;
    SPDLOG_INFO("vertex shader id: {}", vertShader->Get()); 
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());
    
    m_program = Program::Create({fragShader, vertShader}); 
    if (!m_program)
        return false;
    SPDLOG_INFO("program id: {}", m_program->Get()); 
    
    // For GLSL, Youtube #10 This is for assigning value into uniform variable
    // You can change color in app code such as context.cpp and other cpps. 
    // You don't have to change color in vs file.
    auto loc = glGetUniformLocation(m_program->Get(), "color");     // take "color", which is variable name
    m_program->Use();
    glUniform4f(loc, 0.0f, 1.0f, 0.0f, 1.0f);

    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
    


    auto image = Image::Load("./image/container.jpg"); 
    if (!image)
        return false;
    SPDLOG_INFO("image2: {}x{}, {} channels",image->GetWidth(), image->GetHeight(), image->GetChannelCount());


    auto image2 = Image::Load("./image/awesomeface.png"); 
    if (!image2)
        return false;
    SPDLOG_INFO("image2: {}x{}, {} channels",image2->GetWidth(), image2->GetHeight(), image2->GetChannelCount());
    
    // auto image = Image::Create(512, 512);
    // image->SetCheckImage(16, 16);

    // after image loading, we can create texture.
    m_texture = Texture::CreateFromImage(image.get()); // image.get() internal function get. return address of image
    m_texture2 = Texture::CreateFromImage(image2.get()); // image.get() internal function get. return address of image

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->Get());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture2->Get());
    m_program->Use();
    glUniform1i(glGetUniformLocation(m_program->Get(), "tex"), 0);
    glUniform1i(glGetUniformLocation(m_program->Get(), "tex2"), 1);

    return true;
}

void Context::Render() { 
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->Use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}