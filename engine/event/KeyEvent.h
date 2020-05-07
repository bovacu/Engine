#pragma once

#ifndef KEY_EVENT_H
#define KEY_EVENT_H

#include "Event.h"
#include "engine/input/KeyboardKeys.h"


namespace engine {
    /// ------------------------------------------------ KEY EVENT -------------------------------------------------
    class KeyEvent : public Event {
        protected:
            KeyCode keyCode;

        protected:
            explicit KeyEvent(KeyCode _keyCode) : keyCode(_keyCode) {}

        public:
            inline KeyCode getKeyCode() const { return this->keyCode; }
            EVENT_CLASS_CATEGORY(EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput);
    };
    /// ------------------------------------------------ KEY EVENT END ---------------------------------------------


    /// ------------------------------------------------ KEY PRESSED -----------------------------------------------
    class KeyPressedEvent : public KeyEvent {
        private:
            int repeatedTimes;

        public:
            KeyPressedEvent(KeyCode _keyCode, int _repeatedTimes) : KeyEvent(_keyCode), repeatedTimes(_repeatedTimes) {}
            inline int getRepeatedTimes() const { return this->repeatedTimes; }

            std::string toString() const override {
                std::stringstream _sst;
                _sst << "KeyPressedEvent: KeyPressed = " << (char)this->keyCode << " (" << this->keyCode << ") (pressed " << this->repeatedTimes << " times)";
                return _sst.str();
            }

            EVENT_CLASS_TYPE(KeyPressed);
    };
    /// ------------------------------------------------ KEY PRESSED END ------------------------------------------


    /// ------------------------------------------------ KEY TYPED ------------------------------------------------
    class KeyTypedEvent : public KeyEvent {
        public:
            explicit KeyTypedEvent(KeyCode _keyCode) : KeyEvent(_keyCode) {  }

            std::string toString() const override {
                std::stringstream _sst;
                _sst << "KeyTypedEvent: KeyPressed = " << (char)this->keyCode;
                return _sst.str();
            }

            EVENT_CLASS_TYPE(KeyTyped);
    };
    /// ------------------------------------------------ KEY TYPED END -------------------------------------------


    /// ------------------------------------------------ KEY RELEASED ---------------------------------------------
    class KeyReleasedEvent : public KeyEvent {
        public:
            explicit KeyReleasedEvent(KeyCode _keyCode) : KeyEvent(_keyCode) {}

            std::string toString() const override {
                std::stringstream _sst;
                _sst << "KeyReleasedEvent: KeyReleased = " << (char)this->keyCode << " (" << this->keyCode << ")";
                return _sst.str();
            }

            EVENT_CLASS_TYPE(KeyReleased);
    };
    /// ------------------------------------------------ KEY RELEASED END -----------------------------------------
}

#endif //KEY_EVENT_H
