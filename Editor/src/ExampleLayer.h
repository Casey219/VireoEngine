#pragma once

#include "Vireo.h"

class ExampleLayer : public Vireo::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Vireo::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Vireo::Event& e) override;
private:
	Vireo::ShaderLibrary m_ShaderLibrary;
	Vireo::Ref<Vireo::Shader> m_Shader;
	Vireo::Ref<Vireo::VertexArray> m_VertexArray;

	Vireo::Ref<Vireo::Shader> m_FlatColorShader;
	Vireo::Ref<Vireo::VertexArray> m_SquareVA;

	Vireo::Ref<Vireo::Texture2D> m_Texture, m_ChernoLogoTexture;

	Vireo::OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};
