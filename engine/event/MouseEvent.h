#pragma once

#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

#include <engine/event/Event.h>
#include <engine/input/MouseKeys.h>

namespace engine {

    /// This class represents all of the events that involves moving the mouse.
    class MouseMovedEvent           : public Event {
        private:
            float xPos, yPos;

        public:
            MouseMovedEvent(float _xPos, float _yPos) : xPos(_xPos), yPos(_yPos) {  }

            /// Returns the x position of the mouse.
            /// @return x position of the mouse.
            [[nodiscard]] inline float getX() const { return this->xPos; }

            /// Returns the y position of the mouse.
            /// @return y position of the mouse.
            [[nodiscard]] inline float getY() const { return this->yPos; }

            [[nodiscard]] std::string toString() const override {
                std::stringstream _sst;
                _sst << getName() << ": moved (x = " << this->xPos << ", y = " << this->yPos << ")";
                return _sst.str();
            }

            /// Implementation of the rest of the static and virtual methods.
            static EventType getStaticType()                                                 { return EventType::MouseMoved; }
            [[nodiscard]] EventType getEventType()  const                           override { return getStaticType(); }
            [[nodiscard]] const char* getName()     const                           override { return "MouseMoved"; }
            [[nodiscard]] int getCategoryFlags()    const                           override { return EventCategoryInput | EventCategoryMouse; }
    };




    /// This class represents all of the events that involves scrolling with the mouse.
    class MouseScrolledEvent        : public Event {
        private:
            float xScrolled, yScrolled;

        public:
            MouseScrolledEvent(float _xScrolled, float _yScrolled) : xScrolled(_xScrolled), yScrolled(_yScrolled) {  }

            /// Returns the scrolled value on x.
            /// @return x scrolling value.
            [[nodiscard]] inline float getScrollX() const { return this->xScrolled; }

            /// Returns the scrolled value on y.
            /// @return y scrolling value.
            [[nodiscard]] inline float getScrollY() const { return this->yScrolled; }

            [[nodiscard]] std::string toString() const override {
                std::stringstream _sst;
                _sst << getName() << ": scrolled (x = " << this->xScrolled << ", y = " << this->yScrolled << ")";
                return _sst.str();
            }

            /// Implementation of the rest of the static and virtual methods.
            static EventType getStaticType()                                                 { return EventType::MouseScrolled; }
            [[nodiscard]] EventType getEventType()  const                           override { return getStaticType(); }
            [[nodiscard]] const char* getName()     const                           override { return "MouseScrolled"; }
            [[nodiscard]] int getCategoryFlags()    const                           override { return EventCategoryInput | EventCategoryMouse; }
    };




    /// This class represents all of the events that involves doing stuff with the mouse buttons.
    class MouseButtonEvent          : public Event {
        protected:
            MouseCode mouseButton;

        protected:
            explicit MouseButtonEvent(MouseCode _mouseButton) : mouseButton(_mouseButton) {  }

        public:
            /// Returns the button that made the event happen.
            /// @return The code of the button that triggered the event.
            [[nodiscard]] inline MouseCode getMouseButton() const           { return this->mouseButton; }
            [[nodiscard]] int getCategoryFlags()            const override  { return EventCategoryMouseButton | EventCategoryInput; }
    };




    /// This class represents all of the events that involves pressing the mouse buttons.
    class MouseButtonPressedEvent   : public MouseButtonEvent {
        private:
            int repeatedTimes = 1;

        public:
            explicit MouseButtonPressedEvent(MouseCode _mouseButton) : MouseButtonEvent(_mouseButton) {  }
            [[nodiscard]] inline int getRepeatedTimes() const { return this->repeatedTimes; }

            [[nodiscard]] std::string toString() const override {
                std::stringstream _sst;
                _sst << getName() << ": button = " << this->mouseButton << " (repeated " << this->repeatedTimes << " times)";
                return _sst.str();
            }

            /// Implementation of the rest of the static and virtual methods.
            static EventType getStaticType()                                                 { return EventType::MouseButtonPressed; }
            [[nodiscard]] EventType getEventType()  const                           override { return getStaticType(); }
            [[nodiscard]] const char* getName()     const                           override { return "MouseButtonPressed"; }
    };




    /// This class represents all of the events that involves releasing the mouse buttons.
    class MouseButtonReleasedEvent  : public MouseButtonEvent {
        public:
            explicit MouseButtonReleasedEvent(MouseCode _mouseButton) : MouseButtonEvent(_mouseButton) {  }

            [[nodiscard]] std::string toString() const override {
                std::stringstream _sst;
                _sst << getName() << ": button = " << this->mouseButton;
                return _sst.str();
            }

            /// Implementation of the rest of the static and virtual methods.
            static EventType getStaticType()                                                 { return EventType::MouseButtonReleased; }
            [[nodiscard]] EventType getEventType()  const                           override { return getStaticType(); }
            [[nodiscard]] const char* getName()     const                           override { return "MouseButtonReleased"; }
    };

}

#endif //MOUSE_EVENT_H
