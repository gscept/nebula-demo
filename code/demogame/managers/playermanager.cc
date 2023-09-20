//------------------------------------------------------------------------------
//  playermanager.cc
//  (C) 2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "application/stdneb.h"
#include "playermanager.h"
#include "input/inputserver.h"
#include "graphics/cameracontext.h"
#include "visibility/visibilitycontext.h"
#include "imgui.h"
#include "dynui/im3d/im3dcontext.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "input/mouse.h"
#include "renderutil/mouserayutil.h"
#include "game/api.h"

namespace Demo
{

__ImplementSingleton(PlayerManager)

//------------------------------------------------------------------------------
/**
*/
Game::ManagerAPI
PlayerManager::Create()
{
    n_assert(!PlayerManager::HasInstance());
    Singleton = new PlayerManager;

    Game::ManagerAPI api;
    api.OnActivate = &PlayerManager::OnActivate;
    api.OnBeginFrame = &PlayerManager::OnBeginFrame;
    api.OnCleanup = &PlayerManager::OnCleanup;
    return api;
}

//------------------------------------------------------------------------------
/**
*/
void
PlayerManager::Destroy()
{
    n_assert(PlayerManager::HasInstance());
    delete Singleton;
}

//------------------------------------------------------------------------------
/**
*/
void
PlayerManager::OnActivate()
{
    auto view = GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultView();
    auto stage = GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultStage();

    auto const windowId = Base::DisplayDeviceBase::Instance()->GetMainWindow();
    auto const displayMode = CoreGraphics::WindowGetDisplayMode(windowId);
    SizeT width = displayMode.GetWidth();
    SizeT height = displayMode.GetHeight();

    Game::World* world = Game::GetWorld(WORLD_DEFAULT);

    Game::EntityCreateInfo playerCreateInfo;
    playerCreateInfo.templateId = Game::GetTemplateId("Player/player"_atm);
    playerCreateInfo.immediate = true;
    Singleton->playerEntity = Game::CreateEntity(world, playerCreateInfo);

    GraphicsFeature::Camera camera = Game::GetComponent<GraphicsFeature::Camera>(world, Singleton->playerEntity, Game::GetComponentId("Camera"_atm));
    camera.aspectRatio = (float)width / (float)height;
    camera.viewHandle = GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultViewHandle();
    Game::SetComponent<GraphicsFeature::Camera>(world, Singleton->playerEntity, Game::GetComponentId("Camera"_atm), camera);

    Singleton->freeCamUtil.Setup({0, 50, -3}, {0,0,-1});

    GraphicsFeature::GraphicsFeatureUnit::Instance()->AddRenderUICallback([]()
    {
        //static Math::line ray;
        //if (Input::InputServer::Instance()->GetDefaultMouse()->ButtonPressed(Input::MouseButton::Code::RightButton))
        //{
        //    const Math::mat4 proj = Singleton->camera.GetProjectionMatrix();
        //    ray = RenderUtil::MouseRayUtil::ComputeWorldMouseRay(
        //        Input::InputServer::Instance()->GetDefaultMouse()->GetScreenPosition(),
        //        1000.0f,
        //        Math::inverse(Singleton->camera.view),
        //        Math::inverse(proj),
        //        0.1f
        //    );
        //}
        //Im3d::Im3dContext::DrawLine(ray, 2.0f, { 1.0f, 0.3f, 0.0f, 1.0f });
    });
}

//------------------------------------------------------------------------------
/**
*/
void
PlayerManager::OnBeginFrame()
{
    auto& io = ImGui::GetIO();
    if (!ImGui::GetIO().WantCaptureMouse)
    {
        Singleton->freeCamUtil.SetForwardsKey(io.KeysDown[Input::Key::W]);
        Singleton->freeCamUtil.SetBackwardsKey(io.KeysDown[Input::Key::S]);
        Singleton->freeCamUtil.SetRightStrafeKey(io.KeysDown[Input::Key::D]);
        Singleton->freeCamUtil.SetLeftStrafeKey(io.KeysDown[Input::Key::A]);
        Singleton->freeCamUtil.SetUpKey(io.KeysDown[Input::Key::Q]);
        Singleton->freeCamUtil.SetDownKey(io.KeysDown[Input::Key::E]);
        Singleton->freeCamUtil.SetMouseMovement({ -io.MouseDelta.x, -io.MouseDelta.y });
        Singleton->freeCamUtil.SetAccelerateButton(io.KeyShift);
        Singleton->freeCamUtil.SetRotateButton(io.MouseDown[Input::MouseButton::RightButton]);
        Singleton->freeCamUtil.SetMovementSpeed(0.1f);
        Singleton->freeCamUtil.Update();
    }

    Game::World* world = Game::GetWorld(WORLD_DEFAULT);

    //Math::mat4 worldTransform = Game::GetComponent(Singleton->playerEntity, Game::GetComponentId("WorldTransform"_atm));
    if (Game::IsValid(world, Singleton->playerEntity))
        Game::SetComponent<Math::mat4>(world, Singleton->playerEntity, Game::GetComponentId("WorldTransform"_atm), Math::inverse(Singleton->freeCamUtil.GetTransform()));
}

//------------------------------------------------------------------------------
/**
*/
void
PlayerManager::OnCleanup(Game::World*)
{
    Singleton->playerEntity = Game::Entity::Invalid();
}

//------------------------------------------------------------------------------
/**
*/
PlayerManager::PlayerManager()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
PlayerManager::~PlayerManager()
{
    // empty
}

} // namespace Game


