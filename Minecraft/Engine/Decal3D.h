#pragma once

#include "vec3d.h"
#include "VertexBuffer.h"
#include "SpriteShader3D.h"
#include "Sprite2D.h"
#include "Camera.h"

namespace Engine
{
	class Decal3D
	{
	private:
		VertexbufferSprite3D m_vertexBuffer{};
		Sprite2D             m_texture{};

	public:
		Decal3D() noexcept = default;

		Decal3D(const Sprite2D &texture, const vf3d &pos, const vf3d &size, bool bInverseNormal = false) noexcept
		{
			m_texture = texture;
			
			vf3d normal = Engine::vf3d(0.0f, 0.0f, size.z).cross(Engine::vf3d(size.x, size.y, 0.0f)).normalize();

			Vertex3D vertices[6] = {
				{ { pos.x + size.x, pos.y, pos.z          }, normal, 1.0f, 0.0f },
				{ { pos.x,          pos.y, pos.z          }, normal, 0.0f, 0.0f },
				{ { pos.x,          pos.y, pos.z + size.z }, normal, 0.0f, 1.0f },
				{ { pos.x + size.x, pos.y, pos.z          }, normal, 1.0f, 0.0f },
				{ { pos.x,          pos.y, pos.z + size.z }, normal, 0.0f, 1.0f },
				{ { pos.x + size.x, pos.y, pos.z + size.z }, normal, 1.0f, 1.0f }
			};

			if (bInverseNormal)
			{
				normal = -normal;
				std::swap(vertices[0], vertices[1]);
				std::swap(vertices[3], vertices[4]);
			}

			m_vertexBuffer = VertexbufferSprite3D(vertices, 6);
		}

	public:
		void render(const SpriteShader3D &shader, const Camera &camera) const noexcept
		{
			shader.setMVP(camera.getViewProj());
			m_texture.bind();
			m_vertexBuffer.render(GL_TRIANGLES);
		}
	};
}
