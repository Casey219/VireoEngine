#pragma once

#include"Core.h"
#include<memory>
#include<Window.h>
#include<Events/ApplicationEvent.h>
namespace Vireo {
	class VIR_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	//to be defined in client
	Application* CreateApplication();


}


