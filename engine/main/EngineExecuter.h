#pragma once

/// This class is very important to have and we cannot have the main() in the application file. The reason for this is
/// that the Application class is included in other files, therefore, if we declare there the int main(), it also will
/// be included in other files and will also be defined multiple times.
///
/// This is why this file is so important. In Application class we make the main() as a friend so we don't have to
/// include the headers in here, they can also give compatibility problems.

#if defined(ENGINE_PLATFORM_WINDOWS) || defined(ENGINE_PLATFORM_LINUX)

    int main(int argc, char** argv) {
        engine::Logger::init();
        auto _game = engine::createApplication();
        _game->onRun();
        delete _game;
    }

#endif
