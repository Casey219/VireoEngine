#pragma once

#include<Vireo.h>


class Sandbox2D: public Vireo::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Vireo::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Vireo::Event& e) override;
private:
	Vireo::OrthographicCameraController m_CameraController;

	// Temp
	Vireo::Ref<Vireo::VertexArray> m_SquareVA;
	Vireo::Ref<Vireo::Shader> m_FlatColorShader;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
