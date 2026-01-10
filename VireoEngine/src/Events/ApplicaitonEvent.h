#pragma once

#include"Event.h"


namespace Vireo {

	class VIR_API WindowResizeEvent :public Event
	{
	public:
		WindowResizeEvent(unsigned int width,unsigned int height)
			:m_Width(width),m_Height(height){ }
		inline unsigned int GetWidth() { return m_Width; }
		inline unsigned int GetHeight() { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_TYPE(WindowResize)
		EVENT_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width,  m_Height;
	};

	class VIR_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_TYPE(WindowClose)
		EVENT_CATEGORY(EventCategoryApplication)
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		EVENT_TYPE(AppTick)
		EVENT_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		EVENT_TYPE(AppUpdate)
		EVENT_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_TYPE(AppRender)
		EVENT_CATEGORY(EventCategoryApplication)
	};

}