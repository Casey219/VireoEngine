#pragma once
#include "Renderer/UniformBuffer.h"
namespace Vireo {
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint32_t size, uint32_t binding);
		virtual ~OpenGLUniformBuffer();
		virtual uint32_t GetRendererID() const { return m_RendererID; }

		virtual void BindRange(uint32_t slot, uint32_t offset, uint32_t size) override;

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
	private:
		uint32_t m_RendererID = 0;
	};
}
