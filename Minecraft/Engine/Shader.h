#pragma once
#include <fstream>
#include "Defines.h"
#include <string>
#include <iostream>
#include <memory>

namespace Engine
{
	class Shader
	{
		struct SharedResource
		{
			GLuint m_shaderId{};

		public:
			SharedResource() noexcept = default;
			SharedResource(GLuint id) noexcept { m_shaderId = id; }
			virtual ~SharedResource() noexcept { glDeleteProgram(m_shaderId); }
		};

	private:
		std::shared_ptr<SharedResource> m_shrBufferId;

	public:
		Shader() noexcept = default;

		Shader(const std::string &sVertexFilename, const std::string &sFragmentFilename) noexcept
		{
			m_shrBufferId = std::make_shared<SharedResource>(createShader(sVertexFilename, sFragmentFilename));
		}

		Shader(const std::string &sPath) noexcept
			: Shader(sPath + ".vert", sPath + ".frag") {}

	private:
		static GLuint compile(const std::string &shaderSource, GLenum type) noexcept
		{
			GLuint id = glCreateShader(type);
			const char *src = shaderSource.c_str();
			glShaderSource(id, 1, &src, 0);
			glCompileShader(id);

			int result;
			glGetShaderiv(id, GL_COMPILE_STATUS, &result);
			if (result != GL_TRUE)
			{
				std::cout << "OpenGL Shader Compile Error: " << result << std::endl;

				int length = 0;
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
				std::shared_ptr<char> msg = std::shared_ptr<char>(new char[length]);
				glGetShaderInfoLog(id, length, &length, msg.get());

				std::cout << msg << std::endl;

				return 0;
			}

			return id;
		}

		static std::string parse(const std::string &sFilename) noexcept
		{
			std::ifstream file = std::ifstream(sFilename);
			std::string sSource{}, sLine{};

			if (file.is_open())
			{
				while (std::getline(file, sLine))
					sSource += sLine + '\n';
				file.close();
			}

			return sSource;
		}

		GLuint createShader(const std::string &sVertexFilename, const std::string &sFragmentFilename) const noexcept
		{
			std::string vertexShaderSource = parse(sVertexFilename);
			std::string fragmentShaderSource = parse(sFragmentFilename);

			GLuint programId = glCreateProgram();
			GLuint vs = compile(vertexShaderSource, GL_VERTEX_SHADER);
			GLuint fs = compile(fragmentShaderSource, GL_FRAGMENT_SHADER);

			glAttachShader(programId, vs);
			glAttachShader(programId, fs);
			glLinkProgram(programId);

			glDetachShader(programId, vs);
			glDetachShader(programId, fs);
			glDeleteShader(vs);
			glDeleteShader(fs);

			return programId;
		}

	public:
		void bind() const noexcept
		{
			glUseProgram(getShaderId());
		}

		static void unbind() noexcept
		{
			glUseProgram(0);
		}

		GLuint getShaderId() const noexcept
		{
			return m_shrBufferId.get()->m_shaderId;
		}

		int getUniformLocation(const char *pVariableName) const noexcept
		{
			bind();
			return glGetUniformLocation(getShaderId(), pVariableName);
		}

		int getUniformLocation(const std::string &sVariableName) const noexcept
		{
			return getUniformLocation(sVariableName.c_str());
		}
	};
}
