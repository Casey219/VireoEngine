#include<Vireo.h>
#include"Core/EntryPoint.h"

#include "Sandbox2D.h"
#include "ExampleLayer.h"
#include "EditorLayer.h"

namespace Vireo {
	class EditorApplication :public Application {
	public:
		EditorApplication() 
			: Application("VireoEditor") {
			PushLayer(new EditorLayer());
		}
		~EditorApplication() {

		}

	};


	Application* CreateApplication() {
		return new EditorApplication();
	}

}