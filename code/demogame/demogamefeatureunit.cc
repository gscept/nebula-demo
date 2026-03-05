//------------------------------------------------------------------------------
//  demogamefeatureunit.cc
//  (C) 2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "foundation/stdneb.h"
#include "demogamefeatureunit.h"
#include "basegamefeature/basegamefeatureunit.h"
#include "gamestatemanager.h"
#include "profiling/profiling.h"
#include "managers/inputmanager.h"
#include "managers/movementmanager.h"
#include "managers/playermanager.h"
#include "game/api.h"
#include "properties/input.h"
#include "properties/movement.h"
#include "properties/spaceship.h"
#include "game/world.h"
#include "managers/aimanager.h"
#include "appgame/gameapplication.h"
#include "frame/default.h"
#include "coregraphics/swapchain.h"

namespace Demo
{

__ImplementClass(DemoGameFeatureUnit, 'DGFU', Game::FeatureUnit);
__ImplementSingleton(DemoGameFeatureUnit);

//------------------------------------------------------------------------------
/**
*/
DemoGameFeatureUnit::DemoGameFeatureUnit()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
DemoGameFeatureUnit::~DemoGameFeatureUnit()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
DemoGameFeatureUnit::OnAttach()
{
    this->RegisterComponentType<Demo::PlayerInput>();
    this->RegisterComponentType<Demo::Movement>();
    this->RegisterComponentType<Demo::ShotSpawn>();
    this->RegisterComponentType<Demo::CameraFollow>();
    this->RegisterComponentType<Demo::SpaceShip>();
    this->RegisterComponentType<Demo::SpaceShipMoveParams>();
    this->RegisterComponentType<Demo::AiControlled>();
}

//------------------------------------------------------------------------------
/**
*/
void
DemoGameFeatureUnit::OnActivate()
{
    FeatureUnit::OnActivate();

    // Setup game state
    this->AttachManager(Demo::GameStateManager::Create());
    this->AttachManager(Demo::InputManager::Create());
    this->AttachManager(Demo::MovementManager::Create());
    this->AttachManager(Demo::PlayerManager::Create());
    this->AttachManager(Demo::AiManager::Create());

#if WITH_NEBULA_EDITOR
    if (!App::GameApplication::IsEditorEnabled())
#endif
    {
        Graphics::GraphicsServer::Instance()->AddEndFrameCall([](IndexT frameIndex, IndexT bufferIndex)
        {
            const auto& windows = Graphics::GraphicsServer::Instance()->GetWindows();
            for (const auto& window : windows)
            {
                CoreGraphics::DisplayMode mode = CoreGraphics::WindowGetDisplayMode(window);
                CoreGraphics::SwapchainId swapchain = CoreGraphics::WindowGetSwapchain(window);

                CoreGraphics::SwapchainSwap(swapchain);
                CoreGraphics::QueueType queue = CoreGraphics::SwapchainGetQueueType(swapchain);

                // Allocate command buffer to run swap
                CoreGraphics::CmdBufferId cmdBuf = CoreGraphics::SwapchainAllocateCmds(swapchain);
                CoreGraphics::CmdBufferBeginInfo beginInfo;
                beginInfo.submitDuringPass = false;
                beginInfo.resubmittable = false;
                beginInfo.submitOnce = true;
                CoreGraphics::CmdBeginRecord(cmdBuf, beginInfo);
                CoreGraphics::CmdBeginMarker(cmdBuf, NEBULA_MARKER_TURQOISE, "Swap");

                FrameScript_default::Synchronize("Present_Sync", cmdBuf, CoreGraphics::GraphicsQueueType, { { (FrameScript_default::TextureIndex)FrameScript_default::Export_ColorBuffer.index, CoreGraphics::PipelineStage::TransferRead } }, nullptr);
                CoreGraphics::SwapchainCopy(swapchain, cmdBuf, FrameScript_default::Export_ColorBuffer.tex);

                CoreGraphics::CmdEndMarker(cmdBuf);
                CoreGraphics::CmdFinishQueries(cmdBuf);
                CoreGraphics::CmdEndRecord(cmdBuf);
                auto submission = CoreGraphics::SubmitCommandBuffers(
                    { cmdBuf }
                    , queue
                    , { FrameScript_default::Submission_Scene }
#if NEBULA_GRAPHICS_DEBUG
                    , "Swap"
#endif

                );
                CoreGraphics::DeferredDestroyCmdBuffer(cmdBuf);
            }
       });
    }
}

//------------------------------------------------------------------------------
/**
*/
void
DemoGameFeatureUnit::OnBeginFrame()
{
    FeatureUnit::OnBeginFrame();
}

//------------------------------------------------------------------------------
/**
*/
void
DemoGameFeatureUnit::OnDeactivate()
{
    FeatureUnit::OnDeactivate();
}
} // namespace Demo
