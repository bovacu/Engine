#pragma once

#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

#include "engine/event/Event.h"
#include "engine/input/MouseKeys.h"

namespace engine {

    class MouseMovedEvent : public Event {
        private:
            float xPos, yPos;

        public:
            MouseMovedEvent(float _xPos, float _yPos) : xPos(_xPos), yPos(_yPos) {}
            inline float getX() const { return this->xPos; }
            inline float getY() const { return this->yPos; }

            std::string toString() const override {
                std::stringstream _sst;
                _sst << "MouseMovedEvent: moved (x = " << this->xPos << ", y = " << this->yPos << ")";
                return _sst.str();
            }

            EVENT_CLASS_TYPE(MouseMoved);
            EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse);
    };

    class MouseScrolledEvent : public Event {
        private:
            float xScrolled, yScrolled;

        public:
            MouseScrolledEvent(float _xScrolled, float _yScrolled) : xScrolled(_xScrolled), yScrolled(_yScrolled) {}
            inline float getScrollX() const { return this->xScrolled; }
            inline float getScrollY() const { return this->yScrolled; }

            std::string toString() const override {
                std::stringstream _sst;
                _sst << "MouseScrolledEvent: scrolled (x = " << this->xScrolled << ", y = " << this->yScrolled << ")";
                return _sst.str();
            }

            EVENT_CLASS_TYPE(MouseScrolled);
            EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse);
    };

    class MouseButtonEvent : public Event {
        protected:
            MouseCode mouseButton;

        protected:
            explicit MouseButtonEvent(MouseCode _mouseButton) : mouseButton(_mouseButton) {}

        public:
            inline MouseCode getMouseButton() const { return this->mouseButton; }
            EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryInput);
    };

    class MouseButtonPressedEvent : public MouseButtonEvent {
        private:
            int repeatedTimes = 1;

        public:
            MouseButtonPressedEvent(MouseCode _mouseButton) : MouseButtonEvent(_mouseButton) {}
            inline int getRepeatedTimes() const { return this->repeatedTimes; }

            std::string toString() const override {
                std::stringstream _sst;
                _sst << "MouseButtonPressedEvent: mouseButtonPressed = " << this->mouseButton << " (repeated " << this->repeatedTimes << " times)";
                return _sst.str();
            }

            EVENT_CLASS_TYPE(MouseButtonPressed);
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent {
        public:
            explicit MouseButtonReleasedEvent(MouseCode _mouseButton) : MouseButtonEvent(_mouseButton) {}

            std::string toString() const override {
                std::stringstream _sst;
                _sst << "MouseButtonReleasedEvent: mouseButtonReleased = " << this->mouseButton;
                return _sst.str();
            }

            EVENT_CLASS_TYPE(MouseButtonReleased);
    };

}

#endif //MOUSE_EVENT_H
