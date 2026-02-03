#include<virpch.h>
#include<Renderer/RenderCommand.h>
#include<Platform//OpenGL/OpenGLRendererAPI.h>

namespace Vireo {

	//RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}