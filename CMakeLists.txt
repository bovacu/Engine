cmake_minimum_required(VERSION 3.10)
project(Engine)

set(CMAKE_CXX_STANDARD 17)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

set(GLFW_DIR "${LIB_DIR}/glfw")
add_subdirectory("${GLFW_DIR}")

set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")

set(GLM_DIR "${LIB_DIR}/glm")
add_subdirectory("${GLM_DIR}")

set(STB_DIR "${LIB_DIR}/stb")
include_directories(${STB_DIR})
link_directories(${STB_DIR})

set(ENTT_DIR "${LIB_DIR}/entt")
include_directories(${ENTT_DIR})
link_directories(${ENTT_DIR})

set(IMGUI_DOCKING "${LIB_DIR}/imgui-docking")
include_directories(${IMGUI_DOCKING})
link_directories(${IMGUI_DOCKING})

set(SPDLOG_DIR "${LIB_DIR}/spdlog")
add_subdirectory(${SPDLOG_DIR})

set(BOX2D_DIR "${LIB_DIR}/box2d")
add_subdirectory("${BOX2D_DIR}")

add_executable( Engine engine/main/Application.cpp engine/util/EngineUtil.cpp
                engine/input/Input.h engine/render/elements/Texture.cpp engine/render/elements/Texture.h
                engine/render/elements/Buffer.cpp engine/render/elements/Buffer.h engine/render/elements/Shader.cpp
                engine/render/elements/Shader.h engine/render/elements/VertexArray.cpp engine/render/elements/VertexArray.h
                engine/render/camera/OrthographicCamera.cpp engine/render/camera/OrthographicCamera.h
                engine/render/elements/GraphicsContext.cpp engine/render/elements/GraphicsContext.h
                engine/input/KeyboardKeys.h engine/input/MouseKeys.h engine/event/Event.h engine/event/WindowEvent.h
                engine/event/KeyEvent.h engine/event/MouseEvent.h engine/input/GamePadKeys.h
                engine/render/Window.cpp engine/render/Window.h  engine/api/openGL/OpenGLShader.cpp engine/api/openGL/OpenGLShader.h
                lib/stb/stb_image.cpp engine/util/Logger.cpp engine/main/Core.h engine/platform/Windows/WindowsWindow.cpp
                engine/platform/Windows/WindowsWindow.h engine/render/Renderer.cpp engine/render/Renderer.h
                engine/render/RenderAPI.cpp engine/render/RenderAPI.h
                engine/render/Render2D.cpp engine/render/Render2D.h
                engine/api/openGL/OpenGLVertexArray.cpp engine/api/openGL/OpenGLVertexArray.h engine/api/openGL/OpenGLBuffer.cpp
                engine/api/openGL/OpenGLBuffer.h engine/api/openGL/OpenGLGraphicsContext.cpp engine/api/openGL/OpenGLGraphicsContext.h
                engine/api/openGL/OpenGLRenderAPI.cpp engine/api/openGL/OpenGLRenderAPI.h engine/api/openGL/OpenGLTexture.cpp
                engine/api/openGL/OpenGLTexture.h engine/platform/Windows/WindowsInput.cpp
                engine/main/EngineExecuter.h engine/render/layer/Layer.h engine/render/layer/LayerStack.cpp engine/render/layer/LayerStack.h
                engine/util/Delta.h SaFaTor/Game.cpp SaFaTor/Safator.cpp SaFaTor/Safator.h
                engine/render/camera/OrthographicCameraController.cpp engine/render/camera/OrthographicCameraController.h
                engine/input/Input.cpp engine/render/layer/ImGuiLayer.h engine/render/layer/ImGuiLayer.cpp
                engine/util/Clock.cpp engine/util/Clock.h engine/render/elements/TextureRegion.cpp engine/render/elements/TextureRegion.h
                engine/render/shapes/Shape.cpp engine/render/shapes/Shape.h
                engine/platform/Linux/LinuxWindow.h engine/platform/Linux/LinuxWindow.cpp engine/platform/Linux/LinuxInput.cpp
                engine/api/openGL/OpenGLFrameBuffer.cpp engine/api/openGL/OpenGLFrameBuffer.h
                engine/render/elements/FrameBuffer.cpp engine/render/elements/FrameBuffer.h
                engine/ecs/GameObject.h engine/ecs/Scene.cpp engine/ecs/Scene.h engine/ecs/Components.h engine/ecs/ScriptableObject.h
                engine/render/camera/Camera.h engine/ecs/SceneCamera.cpp engine/ecs/SceneCamera.h engine/ecs/EngineAPI.cpp engine/ecs/EngineAPI.h

                tests/stress/Stress.cpp tests/stress/StressTest.cpp tests/stress/StressTest.h

                engineInspector/InspectorApplication.cpp engineInspector/Inspector.cpp engineInspector/Inspector.h

                lib/imgui-docking/imgui.cpp lib/imgui-docking/imgui_draw.cpp lib/imgui-docking/imgui_widgets.cpp  lib/imgui-docking/imgui_demo.cpp
                lib/imgui-docking/examples/imgui_impl_glfw.cpp lib/imgui-docking/examples/imgui_impl_opengl3.cpp engine/ecs/GameObject.cpp engine/ecs/Physics.cpp engine/ecs/Physics.h engine/ecs/DebugPhysicsDraw.cpp engine/ecs/DebugPhysicsDraw.h)

target_link_libraries ( Engine spdlog)
target_link_libraries ( ${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
target_link_libraries ( ${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")
target_link_libraries ( ${PROJECT_NAME} "glm" "${GLM_LIBRARIES}")
target_link_libraries ( ${PROJECT_NAME} "spdlog" "${SPDLOG_LIBRARIES}")
target_link_libraries ( ${PROJECT_NAME} "box2d" "${BOX2D_LIBRARIES}")

target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_include_directories("${PROJECT_NAME}" PRIVATE "${GLM_DIR}/glm")
target_include_directories("${PROJECT_NAME}" PRIVATE "${SPDLOG_DIR}/include")
target_include_directories("${PROJECT_NAME}" PRIVATE "${BOX2D_DIR}/include")