#include "virpch.h"
#include "Shader.h"
#include<glad/glad.h>
#include<Core.h>
#include <glm/gtc/type_ptr.hpp>
#include"Platform/OpenGL/OpenGLShader.h"
#include"Renderer/Renderer.h"

namespace Vireo {
	Shader* Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    VIR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLShader(filepath);
		}

		VIR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}



	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    VIR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		VIR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}