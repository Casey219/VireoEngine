#pragma once

#include "Event.h"
#include "Core/MouseCodes.h"
namespace Vireo {

	class	 MouseButtonEvent :public Event
	{
	public:
		MouseCode GetMouseButton() const { return m_Button; }
		EVENT_CATEGORY(EventCategoryMouse|EventCategoryInput|EventCategoryMouseButton)
	protected:
		MouseButtonEvent(const MouseCode mouseButton) :m_Button(mouseButton) {}
		MouseCode m_Button;
	};

	class  MouseButtonPressedEvent :public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const MouseCode button)
			:MouseButtonEvent(button) {}
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent:" << m_Button;
			return ss.str();
		}

		EVENT_TYPE(MouseButtonPressed)
	};

	class  MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const MouseCode button)
			: MouseButtonEvent(button) {
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_TYPE(MouseButtonReleased)
	};

	class  MouseMovedEvent :public Event
	{
	public:
		MouseMovedEvent(const float x,const float y)
			:m_MouseX(x),m_MouseY(y) {}
		float GetX() const { return m_MouseX; }
		float GetY() const { return m_MouseY; }
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent:" << m_MouseX << "," << m_MouseY;
			return ss.str();
		}

		EVENT_CATEGORY(EventCategoryMouse | EventCategoryInput)
		EVENT_TYPE(MouseMoved)

	private:
		float m_MouseX, m_MouseY;

	};

	


	class  MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {
		}

		float GetXOffset() const { return m_XOffset; }
		float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		EVENT_TYPE(MouseScrolled)
		EVENT_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_XOffset, m_YOffset;
	};
}
