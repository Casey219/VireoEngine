#include"Render3DLayer.h"

namespace Vireo {
	Render3DLayer::Render3DLayer()
		: Layer("Render3DLayer"), m_EditorCamera(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f)
	{
	}
	void Render3DLayer::OnAttach()
	{
		
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		struct Vertex {
			glm::vec3 Position;
			glm::vec3 Color;
		};

		float positions[] = {
			// Back face
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, // 0: Black
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // 1: Red
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, // 2: Yellow
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // 3: Green
			// Front face
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // 4: Blue
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, // 5: Magenta
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // 6: White
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f  // 7: Cyan
		};

		unsigned int indices[] = {
			// Back face
			0, 1, 2,  2, 3, 0,
			// Front face
			4, 6, 5,  5, 6, 7,
			// Left face
			0, 3, 7,  7, 4, 0,
			// Right face
			1, 5, 6,  6, 2, 1,
			// Top face
			3, 2, 6,  6, 7, 3,
			// Bottom face
			0, 4, 5,  5, 1, 0
		};
		m_CubeIndexCount = sizeof(indices) / sizeof(unsigned int);
		m_CubeVA = VertexArray::Create();
		 Ref<VertexBuffer> cubeVB = VertexBuffer::Create(positions, sizeof(positions));
		 cubeVB->SetLayout({
			 { ShaderDataType::Float3, "a_Position" },
			 { ShaderDataType::Float3, "a_Color" }
			 });
		 m_CubeVA->AddVertexBuffer(cubeVB);
		 Ref<IndexBuffer> cubeIB = IndexBuffer::Create(indices, m_CubeIndexCount);
		 m_CubeVA->SetIndexBuffer(cubeIB);
		 m_CubeShader = Shader::Create("assets/shaders/Cube.glsl");
		 m_UniformBuffer=UniformBuffer::Create(sizeof(MVPMatrix), 0);
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

		// 2. 开启深度测试
		RenderCommand::EnableDepthTest();

		// 3. 开始 3D 场景 
		m_CubeShader->Bind();

		// 设置 Uniforms
		m_MVPMatrix.u_Transform=glm::mat4(1.0f);
		m_MVPMatrix.u_ViewProjection=m_EditorCamera.GetViewProjection();
		m_UniformBuffer->SetData(&m_MVPMatrix, sizeof(MVPMatrix));

		// 4. 绘制
		m_CubeVA->Bind();
		RenderCommand::DrawIndexed(m_CubeVA,m_CubeIndexCount);
		

	}
	void Render3DLayer::OnImGuiRender()
	{
		
	}
	void Render3DLayer::OnEvent(Event& e)
	{
		
	}
}
