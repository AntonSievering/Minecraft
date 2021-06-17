// VertexBuffer.h - Version 2.0

#pragma once

#include "Defines.h"
#include <cstdint>
#include <stddef.h>


namespace Engine
{
	template <class VertexType, uint8_t nPos = 0, uint8_t nNormal = 0, uint8_t nTexture = 0, uint8_t nColour = 0, uint8_t nExtra = 0>
	class Vertexbuffer
	{
		struct SharedResource
		{
			GLuint m_nBufferId = 0;
			GLuint m_nVao      = 0;

		public:
			SharedResource() noexcept = default;
			SharedResource(GLuint bufferId, GLuint vao) noexcept { m_nBufferId = bufferId; m_nVao = vao; }
			virtual ~SharedResource() noexcept { glDeleteBuffers(1, &m_nBufferId); glDeleteVertexArrays(1, &m_nVao); }
		};

	private:
		std::shared_ptr<SharedResource> m_shrBufferId{};
		size_t                          m_nVertices = 0;

	public:
		Vertexbuffer() noexcept = default;

		Vertexbuffer(const std::vector<VertexType> vVertices) noexcept
			: Vertexbuffer(vVertices.data(), vVertices.size()) {}

		Vertexbuffer(const VertexType *pVertices, const size_t nVertices) noexcept
		{
			m_nVertices = nVertices;
			genBuffer();
			setData(pVertices);
			createVertexAttribs();
			unbind();
		}

	private:
		virtual void createVertexAttribs() const noexcept
		{
			GLuint nIterator = 0;
			size_t nSum = 0;

			auto parseParameter = [&](size_t dims) -> void
			{
				if (dims > 0)
				{
					glEnableVertexAttribArray(nIterator);
					glVertexAttribPointer(nIterator, (GLint)dims, GL_FLOAT, GL_FALSE, sizeof(VertexType), (void *)nSum);

					nIterator++;
					nSum += sizeof(float) * dims;
				}
			};

			parseParameter(nPos);
			parseParameter(nNormal);
			parseParameter(nTexture);
			parseParameter(nColour);
			parseParameter(nExtra);
		}

		void genBuffer() noexcept
		{
			GLuint vao{}, bufferId{};
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			glGenBuffers(1, &bufferId);
			m_shrBufferId = std::make_shared<SharedResource>(bufferId, vao);
		}

		GLuint getVAO() const noexcept
		{
			return m_shrBufferId.get()->m_nVao;
		}

		GLuint getBufferId() const noexcept
		{
			return m_shrBufferId.get()->m_nBufferId;
		}

	public:
		void setData(const VertexType *pVertices) noexcept
		{
			bind();
			glBindBuffer(GL_ARRAY_BUFFER, getBufferId());
			glBufferData(GL_ARRAY_BUFFER, getVerticesCount() * sizeof(VertexType), pVertices, GL_STATIC_DRAW);
		}

		void bind() const noexcept
		{
			glBindVertexArray(getVAO());
		}

		static void unbind() noexcept
		{
			glBindVertexArray(0);
		}

		size_t getVerticesCount() const noexcept
		{
			return m_nVertices;
		}

		void render(const GLenum &mode = GL_TRIANGLES) const noexcept
		{
			bind();
			glDrawArrays(mode, 0, (GLsizei)getVerticesCount());
		}
	};
	
	class MinecraftVertexbuffer2 : public Vertexbuffer<MinecraftVertex2, 0, 0, 0, 0, 0>
	{
	public:
		MinecraftVertexbuffer2() noexcept = default;
		
		MinecraftVertexbuffer2(const std::vector<MinecraftVertex2> &vVertices)
			: Vertexbuffer(vVertices) {}

		MinecraftVertexbuffer2(MinecraftVertex2 *pVertices, const size_t nVertices)
			: Vertexbuffer(pVertices, nVertices) {}

	public:
		void createVertexAttribs() const noexcept override
		{
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MinecraftVertex2), (void *)offsetof(MinecraftVertex2, pos));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(0, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(MinecraftVertex2), (void *)offsetof(MinecraftVertex2, data));
		}
	};

	using VertexbufferSprite3D  = Vertexbuffer<Vertex3D, 3, 3, 2, 0, 0>;
	using VertexbufferSprite2D  = Vertexbuffer<Vertex2D, 2, 0, 2, 0, 0>;
	using MinecraftVertexbuffer = Vertexbuffer<MinecraftVertex, 3, 0, 3, 0, 1>;
}
