#pragma once

#include"Core.h"
#include<memory>
#include<Window.h>
#include<Events/ApplicationEvent.h>
#include<LayerStack.h>
#include <ImGui/ImGuiLayer.h>
#include <Renderer/Shader.h>
#include<Renderer/Buffer.h>
namespace Vireo {
	class  Application
	{
	public:
		Application();
		virtual ~Application();

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

		void Run();

		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray ;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer>  m_IndexBuffer;
	private:
		static Application* s_Instance;
	};

	//to be defined in client
	Application* CreateApplication();


}


