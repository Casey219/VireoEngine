#pragma once

#include "Core/Base.h"
namespace Vireo
{
	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() {}
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void BindRange(uint32_t slot, uint32_t offset, uint32_t size) = 0;

		static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding);
	};
}
