#include<virpch.h>
#include<Renderer/VertexArray.h>
#include<Renderer/Renderer.h>
#include<Platform/OpenGL/OpenGLVertexArray.h>

namespace Vireo {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    VIR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLVertexArray();
		}

		VIR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}