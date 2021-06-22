#pragma once
#include "defines.h"
#include "VertexBuffer.h"
#include <cstdint>
#include <vector>

namespace Engine
{
	template <class T>
	class IndexBuffer
	{
		static_assert(std::is_same<T, uint8_t>::value || std::is_same<T, uint16_t>::value || std::is_same<T, uint32_t>::value);

		struct SharedResource
		{
			GLuint m_nBufferId{};

		public:
			SharedResource() noexcept = default;
			SharedResource(const GLuint id) noexcept { m_nBufferId = id; }
			virtual ~SharedResource() noexcept { if (m_nBufferId) glDeleteBuffers(1, &m_nBufferId); }
		};

	private:
		std::shared_ptr<SharedResource> m_shrBuffer{};
		uint32_t                        m_nIndices = 0;

	public:
		IndexBuffer() noexcept
		{
			m_shrBuffer = std::make_shared<SharedResource>(SharedResource(0));
		}

		IndexBuffer(const T *data, uint32_t nIndices) noexcept
		{
			GLuint bufferId;
			glGenBuffers(1, &bufferId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (uint64_t)nIndices * sizeof(T), data, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			m_shrBuffer = std::make_shared<SharedResource>(bufferId);
			m_nIndices = nIndices;
		}

		IndexBuffer(const std::vector<T> &data) noexcept
			: IndexBuffer(data.data(), data.size()) {}

	public:
		GLuint getBufferId() const noexcept
		{
			return m_shrBuffer.get()->m_nBufferId;
		}

		uint32_t getIndicesCount() const noexcept
		{
			return m_nIndices;
		}

		void bind() const noexcept
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getBufferId());
		}

		void render(const GLenum mode = GL_TRIANGLES) const noexcept
		{
			GLenum type{};
			if constexpr (std::is_same<T, uint32_t>::value) type = GL_UNSIGNED_INT;
			if constexpr (std::is_same<T, uint16_t>::value) type = GL_UNSIGNED_SHORT;
			if constexpr (std::is_same<T, uint8_t>::value)  type = GL_UNSIGNED_BYTE;

			bind();
			glDrawElements(mode, m_nIndices, type, nullptr);
		}

		bool isValid() const noexcept
		{
			return getBufferId() > 0;
		}
	};
}
