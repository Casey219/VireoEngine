#include"Render3DLayer.h"
#include "Renderer/Renderer3D.h"
#include "Utils/AssetManager.h"
#include "Scene/Components.h"


namespace Vireo {
	Render3DLayer::Render3DLayer()
		: Layer("Render3DLayer"), m_EditorCamera(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f)
	{
	}
	void Render3DLayer::OnAttach()
	{
		
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1600;
		fbSpec.Height = 900;
		m_Framebuffer = Framebuffer::Create(fbSpec);


		Renderer3D::Init();
		//m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
		//auto shader = Shader::Create("assets/shaders/PBR3.glsl");
		
		//m_MeshShader = Shader::Create("assets/shaders/PBR3.glsl");
		m_MeshShader = Shader::Create("assets/shaders/BlinnPhongMultiLights.glsl");

		//auto meshShader = Shader::Create("assets/shaders/DefaultMesh.glsl");

		// 加载模型（Model 类内部会调用 Assimp 并创建多个 Submesh）
		//m_BackpackModel = std::make_shared<Model>("assets/models/Cerberus_Gun/Cerberus_LP.FBX");
		//m_BackpackModel = std::make_shared<Model>("assets/models/mosin/mosin.gltf");
		//m_BackpackModel = std::make_shared<Model>("assets/models/Default/Sphere.gltf");
		//m_BackpackModel = std::make_shared<Model>("assets/models/Sponza/Sponza.gltf");
		//m_BackpackModel = std::make_shared<Model>("assets/models/cerberus/cerberus.gltf");
		//m_BackpackModel = std::make_shared<Model>("assets/models/Vampire/dancing_vampire.dae");
		m_BackpackModel = std::make_shared<Model>("assets/models/DamagedHelmet/DamagedHelmet.gltf");
		//m_BackpackModel = std::make_shared<Model>("assets/models/Hyrule_Shield/HShield.obj");
		//m_BackpackModel = AssetManager::GetModel("assets/models/Hyrule_Shield/HShield.obj";

		
		VIR_CORE_ASSERT(m_BackpackModel->GetSubmeshes().size() > 0, "Failed to load model or model has no submeshes!");
		
		VIR_INFO("Model loaded with {0} submeshes", m_BackpackModel->GetSubmeshes().size());

		m_Scene = CreateRef<Scene>();
		//m_Scene->AddPointLight({ 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 1.0f);
		//m_Scene->AddPointLight({ -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 1.0f);

		//m_Scene->AddPointLight({ 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, 1.0f);//m_Scene->AddPointLight({ 15.0f, 15.0f, 15.0f }, { 1.0f, 1.0f, 1.0f }, 0.2f);
		//m_Scene->AddPointLight({ 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, 2.0f);//m_Scene->AddPointLight({ 15.0f, 15.0f, 15.0f }, { 1.0f, 1.0f, 1.0f }, 0.2f);
		//m_Scene->AddPointLight({ -5.0f, 5.0f, -5.0f }, { 1.0f, 1.0f, 1.0f }, 1.0f);
		auto light1 = m_Scene->CreateEntity("Light Entity1");
		light1.AddComponent<PointLightComponent>();
		light1.GetComponent<TransformComponent>().Scale = glm::vec3(0.1f);
		light1.GetComponent<TransformComponent>().Translation = glm::vec3(2.0f, 3.0f, 1.0f);
		auto light2 = m_Scene->CreateEntity("Light Entity2");
		light2.AddComponent<PointLightComponent>();
		light2.GetComponent<TransformComponent>().Scale = glm::vec3(0.1f);
		light2.GetComponent<TransformComponent>().Translation = glm::vec3(-2.0f, 3.0f, -1.0f);
		
		//auto model2 = std::make_shared<Model>("assets/models/Hyrule_Shield/HShield.obj");
		//auto Entity2 = m_Scene->CreateEntity("Model Entity3");
		//Entity2.AddComponent<MeshRendererComponent>(model2, m_MeshShader);
		////Entity2.GetComponent<TransformComponent>().Scale = glm::vec3(0.01f);
		//Entity2.GetComponent<TransformComponent>().Translation = glm::vec3(5.0, 0.0f, -2.0f);
		
		auto Entity1 = m_Scene->CreateEntity("Model Entity");
		Entity1.AddComponent<MeshRendererComponent>(m_BackpackModel, m_MeshShader);
		//Entity1.GetComponent<TransformComponent>().Scale = glm::vec3(1.0f);
		//Entity1.GetComponent<TransformComponent>().Translation = glm::vec3(-5.0f, 0.0f, 0.0f);
		/*auto model = std::make_shared<Model>("assets/models/Cerberus_Gun/Cerberus_LP.FBX");
		auto Entity1 = m_Scene->CreateEntity("Model Entity2");
		Entity1.AddComponent<MeshRendererComponent>(model, m_MeshShader);
		Entity1.GetComponent<TransformComponent>().Scale = glm::vec3(0.005f);
		Entity1.GetComponent<TransformComponent>().Translation = glm::vec3(0.0f, 0.0f, -2.0f);*/

		

		
	}
	void Render3DLayer::OnDetach()
	{
		
	}
	void Render3DLayer::OnUpdate(Timestep ts)
	{
		
		m_EditorCamera.OnUpdate(ts);
		//m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();


		//Renderer2D::BeginScene(m_CameraController.GetCamera());
		//m_Framebuffer->ClearAttachment(1, -1);
		// 准备渲染环境
		//RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		//RenderCommand::Clear();

		// 开启深度测试
		RenderCommand::EnableDepthTest();

		// 开始 3D 场景渲染（上传 Camera Uniform Buffer）
		//Renderer3D::BeginScene(m_EditorCamera);
		static float time = 0.0f;
		time += ts;
		glm::vec3 dynamicLightPos = { glm::sin(time) * 5.0f, 2.0f, glm::cos(time) * 5.0f };
		//Renderer3D::BeginScene(m_EditorCamera, dynamicLightPos);
		Renderer3D::BeginScene(m_EditorCamera);
		/*
		// 提交绘制请求
		// 绘制一个带纹理的平铺地面
		glm::mat4 floorTransform = glm::translate(glm::mat4(1.0f), { 0.0f, -1.0f, 0.0f })
			* glm::scale(glm::mat4(1.0f), { 10.0f, 0.1f, 10.0f });
		Renderer3D::DrawCube(floorTransform,m_Texture,5.0f,{ 0.8f, 0.8f, 0.8f, 1.0f });
*/
		// 绘制一个红色的旋转立方体
		/*static float rotation = 0.0f;
		rotation += ts * 50.0f;
		glm::mat4 cubeTransform = glm::translate(glm::mat4(1.0f), { 0.0f, 0.5f, 0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 1.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { 1.5f, 1.5f, 1.5f });*/

		//
		//Renderer3D::DrawCube(cubeTransform,nullptr,1.0f,{ 1.0f, 0.3f, 0.3f, 1.0f });
		
		/*for (auto& submesh : m_BackpackModel->GetSubmeshes())
		{
			Renderer3D::DrawMesh(submesh.MeshData, submesh.MaterialData,m_MeshShader,cubeTransform);
		}*/

		m_Scene->OnUpdateEditor(ts, m_EditorCamera);

		// 结束场景
		Renderer3D::EndScene();
		

	}
	void Render3DLayer::OnImGuiRender()
	{
		
	}
	void Render3DLayer::OnEvent(Event& e)
	{
		
	}
}
