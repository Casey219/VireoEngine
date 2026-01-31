#pragma once

#include"Core.h"
#include<memory>
#include<Window.h>
#include<Events/ApplicationEvent.h>
#include<LayerStack.h>
#include <ImGui/ImGuiLayer.h>

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
		bool OnWindowResize(WindowResizeEvent& e);
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	//to be defined in client
	Application* CreateApplication();


}


