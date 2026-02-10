#include"virpch.h"
#include "Application.h"


#include"Logger.h"

#include"Core/Input.h"
#include <Renderer/RenderCommand.h>
#include "Renderer/Renderer.h"
#include <GLFW/glfw3.h>



namespace Vireo {
	Application* Application::s_Instance = nullptr;

	
	Application::Application(const std::string& name)
	{
		VIR_PROFILE_FUNCTION();

		VIR_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;


		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(VIR_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);


	}

	Application::~Application() {
		VIR_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	

	void Application::PushLayer(Layer* layer)
	{
		VIR_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		VIR_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}


	void Application::Run() {
		VIR_PROFILE_FUNCTION();

		while (m_Running)
		{
			VIR_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					VIR_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				m_ImGuiLayer->Begin();
				{
					VIR_PROFILE_SCOPE("LayerStack OnImGuiRender");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();	

		}

	}


	void Application::OnEvent(Event& e) {
		VIR_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(VIR_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(VIR_BIND_EVENT_FN(Application::OnWindowResize));
		//VIR_CORE_TRACE( e.ToString());
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it){
			(*it)->OnEvent(e);
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
		VIR_PROFILE_FUNCTION();

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