#include<virpch.h>
#include<Renderer/Renderer.h>
#include "VertexArray.h"
#include<Renderer/RenderCommand.h>

namespace Vireo {

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}