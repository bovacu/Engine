#include "pch.h"
#include "RenderCommand.h"

namespace engine {

    std::unique_ptr<RenderAPI> RenderCommand::renderAPI = RenderAPI::create();

}