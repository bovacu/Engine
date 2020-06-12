#pragma once

#ifndef WINDOW_EVENT_H
#define WINDOW_EVENT_H

#include <engine/event/Event.h>

namespace engine {

    class WindowResizedEvent    : public Event {
        private:
            unsigned int width, height;

        public:
            WindowResizedEvent(unsigned int _width, unsigned int _height) : width(_width), height(_height) {}
            inline unsigned int getWidth() { return this->width; };
            inline unsigned int getHeight() { return this->height; }

            std::string toString() const override {
                std::stringstream _sst;
                _sst << "WindowResizedEvent -> (width = " << this->width << ", height = " << this->height << ")";
                return _sst.str();
            }

            EVENT_CLASS_TYPE(WindowMoved);
            EVENT_CLASS_CATEGORY(EventCategoryGame);
    };

    class WindowMovedEvent    : public Event {
        private:
            unsigned int x, y;

        public:
            WindowMovedEvent(unsigned int _x, unsigned int _y) : x(_x), y(_y) {}
            inline unsigned int getX() { return this->x; };
            inline unsigned int getY() { return this->y; }

            std::string toString() const override {
                std::stringstream _sst;
                _sst << "WindowMovedEvent -> (x = " << this->x << ", y = " << this->y << ")";
                return _sst.str();
            }

            EVENT_CLASS_TYPE(WindowResized);
            EVENT_CLASS_CATEGORY(EventCategoryGame);
    };

    class WindowClosedEvent     : public Event {
        public:
            EVENT_CLASS_TYPE(WindowClosed);
            EVENT_CLASS_CATEGORY(EventCategoryGame);
    };

    class WindowFrameEvent      : public Event {
        public:
            EVENT_CLASS_TYPE(GameFrame);
            EVENT_CLASS_CATEGORY(EventCategoryGame);
    };

    class WindowUpdateEvent     : public Event {
        public:
            EVENT_CLASS_TYPE(GameUpdate);
            EVENT_CLASS_CATEGORY(EventCategoryGame);
    };

    class WindowRenderEvent     : public Event {
        public:
            EVENT_CLASS_TYPE(GameRender);
            EVENT_CLASS_CATEGORY(EventCategoryGame);
    };
}

#endif //WINDOW_EVENT_H
