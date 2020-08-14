/// This class is the main one to be included in the games or applications made with this engine. No othe includes to
/// elements of the engine should be made.

#include <engine/util/Logger.h>

#include <engine/main/Application.h>

#include <engine/render/layer/Layer.h>
#include <engine/render/layer/LayerStack.h>

#include <engine/render/camera/OrthographicCameraController.h>
#include <engine/main/Core.h>

#include <engine/event/Event.h>
#include <engine/event/WindowEvent.h>
#include <engine/event/KeyEvent.h>
#include <engine/event/MouseEvent.h>

#include <engine/render/elements/VertexArray.h>
#include <engine/render/elements/FrameBuffer.h>
#include <engine/render/elements/Shader.h>
#include <engine/render/elements/Texture.h>

#include <engine/render/Renderer.h>
#include <engine/render/shapes/Shape.h>

#include <engine/input/Input.h>
#include <engine/input/KeyboardKeys.h>
#include <engine/input/MouseKeys.h>
#include <engine/input/GamePadKeys.h>

#include <engine/ecs/Scene.h>
#include <engine/ecs/GameObject.h>
#include <engine/ecs/Components.h>
#include <engine/ecs/ScriptableObject.h>
#include <engine/ecs/SceneCamera.h>

#include <glm/glm.hpp>
#include <imgui.h>