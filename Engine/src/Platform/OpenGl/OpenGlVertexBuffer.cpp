#include "OpenGlVertexBuffer.hpp"
#include <Utilities/InstrumentationMacros.hpp>
#include <Core/Core.hpp>
#include <Gl.h>

namespace ant::OpenGl
{
    GlVertexBuffer::GlVertexBuffer()
    {   
        m_layout = MakeRef<GlVertexLayout>();

        CORE_DETAILED_PROFILE_FUNC();

        m_glId = GL_INVALID_INDEX;
        m_vertexArrayGlId = GL_INVALID_INDEX;

        glCreateBuffers(1, &m_glId);
        glCreateVertexArrays(1, &m_vertexArrayGlId);

        CORE_ASSERT(m_glId != GL_INVALID_INDEX, "Failed to create glVertexBuffer");
        CORE_ASSERT(m_vertexArrayGlId != GL_INVALID_INDEX, "Failed to create glVertexArray");
    }

    GlVertexBuffer::~GlVertexBuffer()
    {
        CORE_DETAILED_PROFILE_FUNC();

        glDeleteBuffers(1, &m_glId);
        glDeleteVertexArrays(1, &m_vertexArrayGlId);
    }

    void GlVertexBuffer::Bind() const
    {
        CORE_DETAILED_PROFILE_FUNC();

        glBindVertexArray(m_vertexArrayGlId);
        glBindBuffer(GL_ARRAY_BUFFER, m_glId);
        m_layout->Enable(); //this has to be done only once
    }

    void GlVertexBuffer::UnBind() const
    {
        CORE_DETAILED_PROFILE_FUNC();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        m_layout->Disable();
    }

    void GlVertexBuffer::UploadData(float *data, size_t size)
    {
        CORE_INTERMEDIATE_PROFILE_FUNC();

        Bind();
        glBufferData(GL_ARRAY_BUFFER, m_layout->GetVertexSize() * size, data, GL_STATIC_DRAW);
        UnBind();
    }
   
}