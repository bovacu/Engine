#pragma once

#ifndef KEY_EVENT_H
#define KEY_EVENT_H

#include <engine/event/Event.h>
#include <engine/input/KeyboardKeys.h>


namespace engine {

    /// Base class for all the key events
    class KeyEvent          : public Event {
        protected:
            KeyCode keyCode;

        protected:
            explicit KeyEvent(KeyCode _keyCode) : keyCode(_keyCode) {}

        public:
            /// Returns the code of the key that has been pressed, typed or released.
            /// @return The code of the key that made the event happened.
            [[nodiscard]] inline KeyCode getKeyCode() const { return this->keyCode; }

            /// Returns the flag with all the categories that the event fills in.
            /// @return The flag with the categories.
            [[nodiscard]] int getCategoryFlags() const override { return EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput; }
    };

    /// This class represents all of the events that involves pressing a key.
    class KeyPressedEvent   : public KeyEvent {
        private:
            int repeatedTimes;

        public:
            KeyPressedEvent(KeyCode _keyCode, int _repeatedTimes) : KeyEvent(_keyCode), repeatedTimes(_repeatedTimes) {}

            /// Returns the number of times in a sequence that the key has been pressed.
            /// @return The number of pressing times.
            [[nodiscard]] inline int getRepeatedTimes() const { return this->repeatedTimes; }

            [[nodiscard]] std::string toString() const override {
                std::stringstream _sst;
                _sst << getName() << ": Key = " << (char)this->keyCode << " (" << this->keyCode << ") (down " << this->repeatedTimes << " times)";
                return _sst.str();
            }

            /// Implementation of the rest of the static and virtual methods.
            static EventType getStaticType()                                             { return EventType::KeyPressed; }
            [[nodiscard]] EventType getEventType() const                        override { return getStaticType(); }
            [[nodiscard]] const char* getName() const                           override { return "KeyPressed"; }
    };




    /// This class represents all of the events that involves typing a key.
    class KeyTypedEvent     : public KeyEvent {
        private:
            char typedChar;

        public:
            explicit KeyTypedEvent(KeyCode _keyCode) : KeyEvent(_keyCode), typedChar((char)this->keyCode) {  }

            /// Returns the number of times in a sequence that the key has been pressed.
            /// @return The char typed.
            [[nodiscard]] inline char getCharTyped() const { return (char)this->typedChar; }

            [[nodiscard]] std::string toString() const override {
                std::stringstream _sst;
                _sst << getName() << ": Key = " << (char)this->keyCode;
                return _sst.str();
            }

            /// Implementation of the rest of the static and virtual methods.
            static EventType getStaticType()                                             { return EventType::KeyTyped; }
            [[nodiscard]] EventType getEventType() const                        override { return getStaticType(); }
            [[nodiscard]] const char* getName() const                           override { return "KeyTyped"; }
    };




    /// This class represents all of the events that involves releasing a key.
    class KeyReleasedEvent  : public KeyEvent {
        public:
            explicit KeyReleasedEvent(KeyCode _keyCode) : KeyEvent(_keyCode) {}

            [[nodiscard]] std::string toString() const override {
                std::stringstream _sst;
                _sst << getName() << ": Key = " << (char)this->keyCode << " (" << this->keyCode << ")";
                return _sst.str();
            }

            /// Implementation of the rest of the static and virtual methods.
            static EventType getStaticType()                                             { return EventType::KeyReleased; }
            [[nodiscard]] EventType getEventType() const                        override { return getStaticType(); }
            [[nodiscard]] const char* getName() const                           override { return "KeyReleased"; }
    };

}

#endif //KEY_EVENT_H
