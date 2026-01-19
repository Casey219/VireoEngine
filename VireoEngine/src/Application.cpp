#include"virpch.h"
#include "Application.h"


#include"Logger.h"
#include<glad/glad.h>
#include<GLFW//glfw3.h>
#include<Input.h>

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)



namespace Vireo {
	Application* Application::s_Instance = nullptr;

	Application::Application() {
		VIR_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;


		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

	}

	Application::~Application() {

	}

	

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}


	void Application::Run() {
		WindowResizeEvent e(1200, 800);
		
		while (m_Running)
		{
			glClearColor(0.1, 0.2, 0.1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			auto [x, y] = Input::GetMousePosition();
			VIR_CORE_ERROR("{0}, {1}", x, y);

			m_Window->OnUpdate();


		}


	}


	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		//VIR_CORE_TRACE( e.ToString());
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}


	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}