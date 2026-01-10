#include"virpch.h"
#include "Application.h"

#include"Events/ApplicaitonEvent.h"
#include"Logger.h"

namespace Vireo {

	Application::Application() {

	}

	Application::~Application() {

	}

	void Application::Run() {
		WindowResizeEvent e(1200, 800);
		//VIR_TRACE(e);

		//VIR_TRACE(e.ToString());

		if (e.IsInCategory(EventCategoryApplication)) {
			VIR_TRACE(e.ToString());
		}
		if (e.IsInCategory(EventCategoryInput)) {
			VIR_TRACE(e.ToString());
		}

	}
}