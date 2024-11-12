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
#include "basegamefeature/components/basegamefeature.h"
#include "game/world.h"
#include "basegamefeature/managers/timemanager.h"
#include "properties/spaceship.h"
#include "properties/input.h"
#include "basegamefeature/components/position.h"
#include "basegamefeature/components/orientation.h"

namespace Demo
{

__ImplementClass(Demo::PlayerManager, 'DPlM', Game::Manager);
__ImplementSingleton(PlayerManager)

//------------------------------------------------------------------------------
/**
*/
void
CameraFollowSpaceShip()
{
    using namespace Math;
    Game::TimeSource const* const time = Game::Time::GetTimeSource(TIMESOURCE_GAMEPLAY);
    std::function UpdateCamera = [time](
        Game::World* world,
        PlayerInput const& playerInput,
        Game::Position& position,
        Game::Orientation& orientation,
        SpaceShip const& ship,
        CameraFollow& camFollow
        )
        {
            if (!playerInput.hasFocus)
                return;
            // update camera view transform
            vec3 desiredCamPos = position + Math::rotate(orientation, camFollow.targetCamLocalOffset);
            camFollow.currentCamOffset = lerp(camFollow.currentCamOffset, desiredCamPos, time->frameTime * camFollow.cameraSmoothFactor);
            mat4 camTransform = inverse(lookatrh(camFollow.currentCamOffset, camFollow.currentCamOffset + orientation.z_axis(), orientation.y_axis()));

            if (world->IsValid(PlayerManager::Instance()->mainCameraEntity))
            {
                auto cam = PlayerManager::Instance()->mainCameraEntity;
                GraphicsFeature::Camera camera = world->GetComponent<GraphicsFeature::Camera>(cam);
                camera.localTransform = camTransform;
                world->SetComponent(cam, camera);
            }
        };

    Game::World* world = Game::GetWorld(WORLD_DEFAULT);
    Game::ProcessorBuilder(world, "MovementManager.UpdateCamera")
        .Func(UpdateCamera)
        .On("OnBeginFrame")
        .Order(101)
        .Build();
}

//------------------------------------------------------------------------------
/**
*/
PlayerManager::PlayerManager()
{
    __ConstructSingleton
}

//------------------------------------------------------------------------------
/**
*/
PlayerManager::~PlayerManager()
{
    __DestructSingleton
}

//------------------------------------------------------------------------------
/**
*/
void
PlayerManager::OnActivate()
{
    Game::Manager::OnActivate();
    auto view = GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultView();
    auto stage = GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultStage();

    auto const windowId = Base::DisplayDeviceBase::Instance()->GetMainWindow();
    auto const displayMode = CoreGraphics::WindowGetDisplayMode(windowId);
    SizeT width = displayMode.GetWidth();
    SizeT height = displayMode.GetHeight();

    Game::World* world = Game::GetWorld(WORLD_DEFAULT);

    Game::EntityCreateInfo playerCreateInfo;
    playerCreateInfo.templateId = Game::GetTemplateId("Camera/default"_atm);
    playerCreateInfo.immediate = true;
    this->mainCameraEntity = world->CreateEntity(playerCreateInfo);

    GraphicsFeature::Camera camera = world->GetComponent<GraphicsFeature::Camera>(this->mainCameraEntity);
    camera.aspectRatio = (float)width / (float)height;
    camera.viewHandle = GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultViewHandle();
    world->SetComponent<GraphicsFeature::Camera>(this->mainCameraEntity, camera);

    this->freeCamUtil.Setup({0, 50, -3}, {0,0,-1});

    CameraFollowSpaceShip();
}

//------------------------------------------------------------------------------
/**
*/
void
PlayerManager::OnDeactivate()
{
    Game::Manager::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
*/
void
PlayerManager::OnBeginFrame()
{
    Game::World* world = Game::GetWorld(WORLD_DEFAULT);
    if (Input::InputServer::Instance()->GetDefaultMouse()->ButtonPressed(Input::MouseButton::Code::RightButton))
    {
        if (world->IsValid(this->mainCameraEntity))
        {
            auto cam = PlayerManager::Instance()->mainCameraEntity;
            GraphicsFeature::Camera camera = world->GetComponent<GraphicsFeature::Camera>(cam);
            camera.localTransform = Math::inverse(this->freeCamUtil.GetTransform());
            world->SetComponent(cam, camera);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
PlayerManager::OnCleanup(Game::World*)
{
    this->mainCameraEntity = Game::Entity::Invalid();
}

} // namespace Game


