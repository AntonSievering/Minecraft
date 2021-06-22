#pragma once
#include "defines.h"

namespace Engine
{
	class Camera
	{
	protected:
		glm::vec3 position{};
		glm::mat4 projection{};
		glm::mat4 view{};
		glm::mat4 viewProj{};
		float fov = 0.0f;

	private:
		float width = 0.0f, height = 0.0f;

	public:
		Camera() noexcept = default;

		Camera(const float &fov, const float &width, const float &height) noexcept
		{
			this->fov = fov;
			this->width = width;
			this->height = height;
			view = glm::mat4(1.0f);
			position = glm::vec3(0.0f);

			setFieldOfView(fov);
			update();
		}

	public:
		glm::mat4 getViewProj() const noexcept
		{
			return viewProj;
		}

		glm::mat4 getView() const noexcept
		{
			return view;
		}

		Engine::vf3d getPosition() const noexcept
		{
			return position;
		}
		void setPosition(const glm::vec3 &pos) noexcept
		{
			position = pos;
		}

		void setProjectionMatrix(const glm::mat4 &newProjection) noexcept
		{
			projection = newProjection;
		}

		virtual void update() noexcept
		{
			viewProj = projection * view;
		}

		virtual void translate(const glm::vec3 &v) noexcept
		{
			position += v;
			view = glm::translate(view, -v);
		}

		void setFieldOfView(const float &fov) noexcept
		{
			projection = glm::perspective(fov, width / height, 0.01f, 10000.0f);
		}

		float getFieldOfView() const noexcept
		{
			return fov;
		}
	};

	class FPSCamera : public Camera
	{
	private:
		constexpr static const float mouseSensitivity = 0.2f;

	protected:
		float yaw = 0.0f;
		float pitch = 0.0f;
		glm::vec3 lookAt{};
		glm::vec3 up{};

	public:
		FPSCamera() noexcept = default;

		FPSCamera(const float &fov, const float &width, const float &height) noexcept
			: Camera(fov, width, height)
		{
			up = glm::vec3(0.0f, 1.0f, 0.0f);
			yaw = -90.0f;
			pitch = 0.0f;
			onMouseMoved(0.0f, 0.0f);
			update();
		}

	public:
		void onMouseMoved(const float &xRel, const float &yRel) noexcept
		{
			yaw += xRel * mouseSensitivity;
			pitch -= yRel * mouseSensitivity;
			if (pitch > 89.9f)
				pitch = 89.9f;
			if (pitch < -89.9f)
				pitch = -89.9f;

			glm::vec3 front{};
			front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
			front.y = sin(glm::radians(pitch));
			front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
			lookAt = glm::normalize(front);
		}

		void update() noexcept override
		{
			view = glm::lookAt(position, position + lookAt, up);
			viewProj = projection * view;
		}

		void moveFront(const float &amount) noexcept
		{
			translate(glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) * lookAt) * amount);
		}

		glm::vec3 getMoveFrontVector(const float &amount) const noexcept
		{
			return glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) * lookAt) * amount;
		}

		void moveSideways(const float &amount) noexcept
		{
			translate(glm::normalize(glm::cross(lookAt, up)) * amount);
		}

		glm::vec3 getMoveSidewaysVector(const float &amount) const noexcept
		{
			return glm::normalize(glm::cross(lookAt, up)) * amount;
		}

		glm::vec3 getLookAt() const noexcept
		{
			return lookAt;
		}
	};

	class FloatingCamera : public FPSCamera
	{
	public:
		FloatingCamera() noexcept = default;

		FloatingCamera(const float &fov, const float &width, const float &height) noexcept
			: FPSCamera(fov, width, height) {}

		void moveUp(const float &fAmount) noexcept
		{
			translate(up * fAmount);
		}

		glm::vec3 getMoveUpVector(const float &fAmount) noexcept
		{
			return up * fAmount;
		}
	};
}
