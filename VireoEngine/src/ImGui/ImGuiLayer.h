#pragma once
#include"Core/Layer.h"


namespace Vireo {
	class  ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		
	private:
		float m_Time = 0.0f;
		
	};
}