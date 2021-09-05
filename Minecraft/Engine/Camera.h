#pragma once

#include "defines.h"

namespace Engine
{
	class Camera
	{
	protected:
		glm::vec3 m_vPosition{};
		glm::mat4 m_mProjection{};
		glm::mat4 m_mView{};
		glm::mat4 m_mViewProj{};
		float m_fFov = 0.0f;

	private:
		vf2d m_vScreenSize;

	public:
		Camera() noexcept = default;

		Camera(const float fov, const float width, const float height) noexcept
		{
			m_fFov = fov;
			m_vScreenSize = vf2d(width, height);
			m_mView     = glm::mat4(1.0f);
			m_vPosition = glm::vec3(0.0f);

			setFieldOfView(fov);
			update();
		}

		Camera(const float fov, const vf2d vScreenSize) noexcept
			: Camera(fov, vScreenSize.x, vScreenSize.y) {}

	public:
		glm::mat4 getViewProj() const noexcept
		{
			return m_mViewProj;
		}

		glm::mat4 getView() const noexcept
		{
			return m_mView;
		}

		Engine::vf3d getPosition() const noexcept
		{
			return m_vPosition;
		}
		
		void setPosition(const glm::vec3 &pos) noexcept
		{
			m_vPosition = pos;
		}

		void setProjectionMatrix(const glm::mat4 &newProjection) noexcept
		{
			m_mProjection = newProjection;
		}

		virtual void update() noexcept
		{
			m_mViewProj = m_mProjection * m_mView;
		}

		virtual void translate(const glm::vec3 &v) noexcept
		{
			m_vPosition += v;
			m_mView = glm::translate(m_mView, -v);
		}

		// fov in degrees
		void setFieldOfView(const float fov) noexcept
		{
			m_mProjection = glm::perspective(glm::radians(fov), m_vScreenSize.x / m_vScreenSize.y, 0.1f, 1000.0f);
		}

		float getFieldOfView() const noexcept
		{
			return m_fFov;
		}
	};

	class FPSCamera : public Camera
	{
	private:
		inline static const float s_fMouseSensitivity = 0.2f;

	protected:
		float m_fYaw = 0.0f;
		float m_fPitch = 0.0f;
		glm::vec3 m_vLookAt{};
		glm::vec3 m_vUp{};

	public:
		FPSCamera() noexcept = default;

		FPSCamera(const float fov, const float width, const float height) noexcept
			: Camera(fov, width, height)
		{
			m_vUp = glm::vec3(0.0f, 1.0f, 0.0f);
			m_fYaw = -90.0f;
			m_fPitch = 0.0f;
			onMouseMoved(0.0f, 0.0f);
			update();
		}

		FPSCamera(const float &fov, Engine::vf2d vScreenSize) noexcept
			: FPSCamera(fov, vScreenSize.x, vScreenSize.y) {}

	public:
		void onMouseMoved(const float &xRel, const float &yRel) noexcept
		{
			m_fYaw += xRel * s_fMouseSensitivity;
			m_fPitch -= yRel * s_fMouseSensitivity;
			if (m_fPitch > 89.9f)
				m_fPitch = 89.9f;
			if (m_fPitch < -89.9f)
				m_fPitch = -89.9f;

			glm::vec3 front{};
			front.x = cos(glm::radians(m_fPitch)) * cos(glm::radians(m_fYaw));
			front.y = sin(glm::radians(m_fPitch));
			front.z = cos(glm::radians(m_fPitch)) * sin(glm::radians(m_fYaw));
			m_vLookAt = glm::normalize(front);
		}

		void update() noexcept override
		{
			m_mView = glm::lookAt(m_vPosition, m_vPosition + m_vLookAt, m_vUp);
			m_mViewProj = m_mProjection * m_mView;
		}

		void moveFront(const float &amount) noexcept
		{
			translate(glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) * m_vLookAt) * amount);
		}

		glm::vec3 getMoveFrontVector() const noexcept
		{
			return glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) * m_vLookAt);
		}

		void moveSideways(const float &amount) noexcept
		{
			translate(glm::normalize(glm::cross(m_vLookAt, m_vUp)) * amount);
		}

		glm::vec3 getMoveSidewaysVector() const noexcept
		{
			return glm::normalize(glm::cross(m_vLookAt, m_vUp));
		}

		glm::vec3 getLookAt() const noexcept
		{
			return m_vLookAt;
		}
	};

	class FloatingCamera : public FPSCamera
	{
	public:
		FloatingCamera() noexcept = default;

		FloatingCamera(const float fov, const Engine::vf2d vScreenSize) noexcept
			: FPSCamera(fov, vScreenSize) {}

		FloatingCamera(const float fov, const float width, const float height) noexcept
			: FPSCamera(fov, width, height) {}

	public:
		void moveUp(const float &fAmount) noexcept
		{
			translate(m_vUp * fAmount);
		}

		glm::vec3 getMoveUpVector() const noexcept
		{
			return m_vUp;
		}
	};
}
