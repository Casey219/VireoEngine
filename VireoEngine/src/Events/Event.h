#pragma once

#include"../Core.h"




/*
    Events in VireoEngine are not being buffered, instead they are blocking
    That means every time an event gets dispatched it must be processed
    TODO: Buffer Events and process during the event part of the update stage
*/

namespace Vireo {
    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled

    };

    enum  EventCategoryFlags    
    {
        None=0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyboard = BIT(2),
        EventCategoryMouse = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };

#define EVENT_TYPE(type) static EventType GetStaticType(){return EventType::type;}\
                   virtual EventType GetEventType() const override { return GetStaticType(); }\
                   virtual const char* GetName() const override { return #type; }
#define EVENT_CATEGORY(category) virtual int GetCategoryFlags() const override {return  category;}
    class VIR_API Event
    {
    public:
        virtual ~Event() = default;

        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        inline bool IsInCategory(EventCategoryFlags category) 
        {
            return GetCategoryFlags() & static_cast<int>(category);
        }

        bool Handled = false; //indicates whether this event has been handled

        
    };


    class EventDispatcher
    {
        template<typename T>
        using EventFn = std::function<bool(T&)>;

    public:
        EventDispatcher(Event &event):m_Event(event) {}
        template<typename T>
        bool Dispatch(EventFn<T> func)
        {
            if (m_Event.GetEventType() == T::GetStaticType()) {
                m_Event.Handled = func(static_cast<T&>&m_Event);
                return true    
            }
            return false;
        }


    private:
        Event &m_Event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e) {
        return os << e.ToString();
    }
}


