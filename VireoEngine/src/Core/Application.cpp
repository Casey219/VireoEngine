#include"virpch.h"
#include "Application.h"


#include"Logger.h"

#include"Core/Input.h"
#include <Renderer/RenderCommand.h>
#include "Renderer/Renderer.h"
#include <GLFW/glfw3.h>



namespace Vireo {
	Application* Application::s_Instance = nullptr;

	
	Application::Application() 
	{
		VIR_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;


		m_Window = Window::Create();
		m_Window->SetEventCallback(VIR_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);


	}

	Application::~Application() {
		Renderer::Shutdown();
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
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			//auto [x, y] = Input::GetMousePosition();
			//VIR_CORE_ERROR("{0}, {1}", x, y);

			m_Window->OnUpdate();

		}


	}


	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(VIR_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(VIR_BIND_EVENT_FN(Application::OnWindowResize));
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

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}