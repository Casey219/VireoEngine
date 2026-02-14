#pragma once


#include"Event.h"


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
		KeyPressedEvent(int keyCode, int repeatedCount) :KeyEvent(keyCode), m_RepeatedCount(repeatedCount) {}
		EVENT_TYPE(KeyPressed);
			std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent-KeyCode" << m_KeyCode << ",RepeatedCount:" << m_RepeatedCount;
			return ss.str();
		}
		inline int GetRepeatedCount() const { return m_RepeatedCount; }
	private:
		int m_RepeatedCount;
	};

	class  KeyReleasedEvent :public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode) :KeyEvent(keyCode) {}
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
		KeyTypedEvent(int KeyCode) :KeyEvent(KeyCode) {}

		EVENT_TYPE(KeyTyped)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent" << m_KeyCode;
			return ss.str();
		}
	};


}
