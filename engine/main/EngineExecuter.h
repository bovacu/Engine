#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS

    int main(int argc, char** argv) {
        engine::Logger::init();
        auto _game = engine::createApplication();
        _game->onRun();
        delete _game;
    }
#endif
