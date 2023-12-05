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
private:
    Context() {}
    bool Init(); 
    ProgramUPtr m_program;

    // uint32_t m_vertexArrayObject;   // member variable for VAO(Vertex Array Object)
    VertexLayoutUPtr m_vertexLayout;
    // uint32_t m_vertexBuffer;  // member variabe for VBO(Vertex Buffer Object)
    BufferUPtr m_vertexBuffer;    
    // uint32_t m_indexBuffer;
    BufferUPtr m_indexBuffer;
    TextureUPtr m_texture;         // variable to save texture ID
    TextureUPtr m_texture2;         // variable to save texture ID
};



#endif // __CONTEXT_H__