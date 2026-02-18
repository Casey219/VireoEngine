#include "virpch.h"
#include "UniformBuffer.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"
namespace Vireo
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    VIR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLUniformBuffer>(size, binding);
		}

		VIR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
