#pragma once

#ifndef EVENT_H
#define EVENT_H

#include <sstream>

namespace engine {

    enum class EventType {
        None, WindowClosed, WindowResized, WindowMoved, WindowFocused, WindowLostFocus, GameFrame, GameUpdate,
        GameRender, KeyPressed, KeyDown, KeyReleased, KeyTyped, MouseButtonPressed, MouseButtonDown, MouseButtonReleased,
        MouseScrolled, MouseMoved, WindowMinimized
    };

    enum EventCategory {
        None,
        EventCategoryGame = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyboard = BIT(2),
        EventCategoryMouse = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };

#define EVENT_CLASS_TYPE(_eventType) static EventType getStaticType() { return EventType::_eventType; }\
                                     virtual EventType getEventType() const override { return getStaticType(); }\
                                     virtual const char* getName() const override { return #_eventType; }

#define EVENT_CLASS_CATEGORY(_eventCategory) virtual int getCategoryFlags() const override { return _eventCategory; }

    class Event {
        friend class EventDispatcher;

    public:
        bool handled = false;

    public:
        static EventType getStaticType();
        virtual EventType getEventType() const = 0;
        virtual const char* getName() const = 0;
        virtual int getCategoryFlags() const = 0;
        virtual std::string toString() const { return this->getName(); }

        inline bool isInCategory(EventCategory _category) { return this->getCategoryFlags() & _category; }
    };

    class EventDispatcher {

        template<typename T>
        using EventFn = std::function<bool(T&)>;

    private:
        Event& event;

    public:
        EventDispatcher(Event& _event) : event(_event) {}

        template<typename T>
        bool dispatchEvent(EventFn<T> _func) {
            if (this->event.getEventType() == T::getStaticType()) {
                this->event.handled = _func(*(T*)& this->event);
                return true;
            }

            return false;
        }
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e) {
        return os << e.toString();
    }
}

#endif //EVENT_H
