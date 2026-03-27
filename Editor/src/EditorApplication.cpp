#include<Vireo.h>
#include"Core/EntryPoint.h"

#include "Sandbox2D.h"
#include "ExampleLayer.h"
#include "EditorLayer.h"
#include "Render3DLayer.h"
#include "Editor3DLayer.h"

namespace Vireo {
	class EditorApplication :public Application {
	public:
		EditorApplication(ApplicationCommandLineArgs args) 
			: Application("VireoEditor", args) 
		{
			//PushLayer(new EditorLayer());
			//PushLayer(new Render3DLayer());
			PushLayer(new Editor3DLayer());
		}
		~EditorApplication() {

		}

	};


	Application* CreateApplication(ApplicationCommandLineArgs args) {
		return new EditorApplication(args);
	}

}
