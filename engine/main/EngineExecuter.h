#pragma once

#if defined(ENGINE_PLATFORM_WINDOWS) || defined(ENGINE_PLATFORM_LINUX)

    int main(int argc, char** argv) {
        engine::Logger::init();
        auto _game = engine::createApplication();
        _game->onRun();
        delete _game;
    }

#endif
