#include "Sandbox2D.h"
#include <Platform/OpenGL/OpenGLShader.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <Renderer/Renderer2D.h>
#include "Debug/Instrumentor.h"


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1600.0f / 1000.0f)
{
}

void Sandbox2D::OnAttach()
{
	VIR_PROFILE_FUNCTION();

	m_CheckerboardTexture = Vireo::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	VIR_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Vireo::Timestep ts)
{
	VIR_PROFILE_FUNCTION();

	// Update
	
	m_CameraController.OnUpdate(ts);
	

	// Render

	{
		VIR_PROFILE_SCOPE("Renderer Prep");
		Vireo::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Vireo::RenderCommand::Clear();
	}
	
	{
		VIR_PROFILE_SCOPE("Renderer Draw");
		Vireo::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Vireo::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Vireo::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
		//Vireo::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture,10.0f);
		//Vireo::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), { 0.2f, 0.2f, 0.3f, 1.0f });
		Vireo::Renderer2D::EndScene();
	}
	
	
	
}

void Sandbox2D::OnImGuiRender()
{
	VIR_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Vireo::Event& e)
{
	m_CameraController.OnEvent(e);
}