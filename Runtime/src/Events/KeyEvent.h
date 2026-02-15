#pragma once


#include"Event.h"
#include "Core/KeyCodes.h"

namespace Vireo {

	class  KeyEvent : public Event
	{
	public:
		int GetKeyCode() const { return m_KeyCode; }

		EVENT_CATEGORY(EventCategoryKeyboard | EventCategoryInput)  

	protected:
		KeyEvent(const int keycode)
			: m_KeyCode(keycode) {
		}

		int m_KeyCode;
	};

	class  KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const KeyCode keyCode, const uint16_t repeatedCount) :KeyEvent(keyCode), m_RepeatedCount(repeatedCount) {}
		EVENT_TYPE(KeyPressed);
			std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent-KeyCode" << m_KeyCode << ",RepeatedCount:" << m_RepeatedCount;
			return ss.str();
		}
		inline uint16_t GetRepeatedCount() const { return m_RepeatedCount; }
	private:
		uint16_t m_RepeatedCount;
	};

	class  KeyReleasedEvent :public KeyEvent
	{
	public:
		KeyReleasedEvent(const KeyCode keyCode) :KeyEvent(keyCode) {}
		EVENT_TYPE(KeyReleased)
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent-KeyCode:" << m_KeyCode;
			return ss.str();
		}


	};

	class  KeyTypedEvent :public KeyEvent
	{
	public:
		KeyTypedEvent(const KeyCode KeyCode) :KeyEvent(KeyCode) {}

		EVENT_TYPE(KeyTyped)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent" << m_KeyCode;
			return ss.str();
		}
	};


}
