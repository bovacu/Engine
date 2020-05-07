#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include "engine/render/RenderCommand.h"
#include "engine/render/elements/Shader.h"
#include "engine/render/camera/OrthographicCamera.h"

namespace engine {

    class Renderer {
        private:
            struct SceneData {
                glm::mat4 viewProjectionMatrix;
            };

        static std::unique_ptr<SceneData> sceneData;

        public:
            static void init();
            static void shutdown();

            static void onWindowResize(uint32_t _width, uint32_t _height);

            static void beginRender(OrthographicCamera& _camera);
            static void endRender();

            static void submit(const std::shared_ptr<Shader>& _shader, const std::shared_ptr<VertexArray>& _vertexArray,
                    const glm::mat4& _transform = glm::mat4(1.0f));

            static RenderAPI::API getAPI() { return RenderAPI::getAPI(); }

    };

}

#endif // RENDERER_H
