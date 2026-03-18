#pragma once


namespace Vireo {
	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene();
		static void EndScene();
		static void Submit();
	};
}
