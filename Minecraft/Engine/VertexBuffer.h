// VertexBuffer.h - Version 2.0

#pragma once

#include "Defines.h"
#include <cstdint>
#include <stddef.h>


namespace Engine
{
	template <class VertexType, uint8_t nPos = 0, uint8_t nNormal = 0, uint8_t nTexture = 0, uint8_t nColour = 0, uint8_t nExtra = 0>
	class BaseVertexbuffer
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
	protected:
		size_t                          m_nVertices = 0;

	public:
		BaseVertexbuffer() noexcept = default;

	protected:
		virtual void createVertexAttribs() const noexcept = 0;

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
	
	template <class VertexType, uint8_t nPos = 0, uint8_t nNormal = 0, uint8_t nTexture = 0, uint8_t nColour = 0, uint8_t nExtra = 0>
	class StandardVertexbuffer : public BaseVertexbuffer<VertexType, nPos, nNormal, nTexture, nColour, nExtra>
	{
		using Base = BaseVertexbuffer<VertexType, nPos, nNormal, nTexture, nColour, nExtra>;

	public:
		StandardVertexbuffer() noexcept = default;

		StandardVertexbuffer(const std::vector<VertexType> &vVertices) noexcept
			: StandardVertexbuffer(vVertices.data(), vVertices.size()) {}

		StandardVertexbuffer(const VertexType *pVertices, const size_t nVertices) noexcept
		{
			Base::m_nVertices = nVertices;
			Base::genBuffer();
			Base::setData(pVertices);
			createVertexAttribs();
			Base::unbind();
		}

	protected:
		void createVertexAttribs() const noexcept override
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
	};

	class MinecraftVertexbuffer : public BaseVertexbuffer<MinecraftVertex, 0, 0, 0, 0, 0>
	{
	public:
		MinecraftVertexbuffer() noexcept = default;
		
		MinecraftVertexbuffer(const std::vector<MinecraftVertex> &vVertices)
			: MinecraftVertexbuffer(vVertices.data(), vVertices.size()) {}

		MinecraftVertexbuffer(const MinecraftVertex *pVertices, const size_t nVertices)
		{
			m_nVertices = nVertices;
			genBuffer();
			setData(pVertices);
			createVertexAttribs();
			unbind();
		}

	protected:
		void createVertexAttribs() const noexcept override
		{
			glEnableVertexAttribArray(0);
			glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(MinecraftVertex), (const void *)offsetof(MinecraftVertex, data[0]));
			glEnableVertexAttribArray(1);
			glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(MinecraftVertex), (const void *)offsetof(MinecraftVertex, data[1]));
		}
	};

	using VertexbufferSprite3D  = StandardVertexbuffer<Vertex3D, 3, 3, 2, 0, 0>;
	using VertexbufferSprite2D  = StandardVertexbuffer<Vertex2D, 2, 0, 2, 0, 0>;
}
