#pragma once

#include"Base.h"
#include<memory>
#include"Window.h"
#include<Events/ApplicationEvent.h>
#include"LayerStack.h"
#include <ImGui/ImGuiLayer.h>

namespace Vireo {
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			VIR_CORE_ASSERT(index < Count);
			return Args[index];
		}
	};

	class  Application
	{
	public:
		Application(const std::string& name = "Vireo App", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		inline Window& GetWindow() { return *m_Window; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application& Get() { return *s_Instance; }

		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }
		void Run();

		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		ApplicationCommandLineArgs m_CommandLineArgs;
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	//to be defined in client
	Application* CreateApplication(ApplicationCommandLineArgs args);



}


