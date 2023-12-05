#include "buffer.h"

BufferUPtr Buffer::CreateWithData(uint32_t bufferType, uint32_t usage, const void* data, size_t dataSize) { 
    auto buffer = BufferUPtr(new Buffer());
    if (!buffer->Init(bufferType, usage, data, dataSize)) 
        return nullptr;
   return std::move(buffer); 
}

Buffer::~Buffer() { 
    if (m_buffer) {
       glDeleteBuffers(1, &m_buffer); 
    }
}

void Buffer::Bind() const { 
    glBindBuffer(m_bufferType, m_buffer); 
}

// m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 32);

bool Buffer::Init( uint32_t bufferType, uint32_t usage, const void* data, size_t dataSize) { 
    m_bufferType = bufferType;
    m_usage = usage;
    glGenBuffers(1, &m_buffer); 
    Bind();
    glBufferData(m_bufferType, dataSize, data, usage); 
    return true;
}