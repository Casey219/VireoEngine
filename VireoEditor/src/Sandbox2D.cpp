#include "Sandbox2D.h"
#include <Platform/OpenGL/OpenGLShader.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>



Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_SquareVA = Vireo::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Vireo::Ref<Vireo::VertexBuffer> squareVB;
	squareVB.reset(Vireo::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
		{ Vireo::ShaderDataType::Float3, "a_Position" }
		});
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Vireo::Ref<Vireo::IndexBuffer> squareIB;
	squareIB.reset(Vireo::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_FlatColorShader = Vireo::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Vireo::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Vireo::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Vireo::RenderCommand::Clear();

	Vireo::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Vireo::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Vireo::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Vireo::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Vireo::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Vireo::Event& e)
{
	m_CameraController.OnEvent(e);
}