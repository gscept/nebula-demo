//------------------------------------------------------------------------------
//  gamestatemanager.cc
//  (C) 2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "application/stdneb.h"
#include "gamestatemanager.h"
#include "models/modelcontext.h"
#include "graphics/graphicsentity.h"
#include "visibility/visibilitycontext.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "basegamefeature/managers/entitymanager.h"
#include "basegamefeature/properties/transform.h"
#include "input/inputserver.h"
#include "input/keyboard.h"
#include "dynui/im3d/im3dcontext.h"
#include "imgui.h"
#include "util/random.h"
#include "characters/charactercontext.h"
#include "models/nodes/shaderstatenode.h"
#include "dynui/im3d/im3d.h"
#include "lighting/lightcontext.h"
#include "decals/decalcontext.h"
#include "resources/resourceserver.h"
#include "terrain/terraincontext.h"

#include "materials/materialserver.h"
#include "graphicsfeature/managers/graphicsmanager.h"

#ifdef __WIN32__
#include <shellapi.h>
#elif __LINUX__

#endif

namespace Demo
{

__ImplementSingleton(GameStateManager)

//------------------------------------------------------------------------------
/**
*/
Game::ManagerAPI
GameStateManager::Create()
{
    n_assert(!GameStateManager::HasInstance());
    Singleton = n_new(GameStateManager);

    Game::ManagerAPI api;
    api.OnActivate = &GameStateManager::OnActivate;
    api.OnBeginFrame = &GameStateManager::OnBeginFrame;
    api.OnFrame = &GameStateManager::OnFrame;
    return api;
}

//------------------------------------------------------------------------------
/**
*/
void
GameStateManager::Destroy()
{
    n_assert(GameStateManager::HasInstance());
    n_delete(Singleton);
}

//------------------------------------------------------------------------------
/**
*/
GameStateManager::GameStateManager()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
GameStateManager::~GameStateManager()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
GameStateManager::OnActivate()
{
    {
        Game::EntityCreateInfo info;
        info.immediate = false;
        info.templateId = Game::GetTemplateId("StaticGroundPlane/dev_ground_plane"_atm);
        Game::CreateEntity(info);
    }
    {
        Game::EntityCreateInfo info;
        info.immediate = true;
        info.templateId = Game::GetTemplateId("StaticEnvironment/knob_metallic"_atm);
        Game::Entity entity = Game::CreateEntity(info);
        Game::SetProperty(entity, Game::GetPropertyId("WorldTransform"_atm), Math::scaling(5, 5, 5) * Math::translation({ 5, 0, 5 }));
    }
    {
        Game::EntityCreateInfo info;
        info.immediate = true;
        info.templateId = Game::GetTemplateId("StaticEnvironment/knob_plastic"_atm);
        Game::Entity entity = Game::CreateEntity(info);
        Game::SetProperty(entity, Game::GetPropertyId("WorldTransform"_atm), Math::scaling(5, 5, 5) * Math::translation({ 5, 0, 7 }));
    }
    {
        Game::EntityCreateInfo info;
        info.immediate = true;
        info.templateId = Game::GetTemplateId("StaticEnvironment/knob_reflective"_atm);
        Game::Entity entity = Game::CreateEntity(info);
        Game::SetProperty(entity, Game::GetPropertyId("WorldTransform"_atm), Math::scaling(5, 5, 5) * Math::translation({ 5, 0, 9 }));
    }

    for (int i = 0; i < 5; i++)
    {
        Game::EntityCreateInfo info;
        info.immediate = true;
        info.templateId = Game::GetTemplateId("PhysicsEntity/placeholder_box"_atm);
        Game::Entity entity = Game::CreateEntity(info);
        Game::SetProperty(entity, Game::GetPropertyId("WorldTransform"_atm), Math::rotationyawpitchroll(0.01f, 0.01f, 0.01f) * Math::translation({ 2, 5.0f + ((float)i * 1.0f), 0 }));
    }

    for (size_t i = 0; i < 5; i++)
    {
        Game::EntityCreateInfo info;
        info.immediate = true;
        info.templateId = Game::GetTemplateId("MovingEntity/cube"_atm);
        Game::Entity entity = Game::CreateEntity(info);
        Game::SetProperty(entity, Game::GetPropertyId("WorldTransform"_atm), Math::translation({ 0, 0.5f, 0 }));
    }

    GraphicsFeature::GraphicsFeatureUnit::Instance()->AddRenderUICallback([]()
    {
        
    });
}

//------------------------------------------------------------------------------
/**
*/
void
GameStateManager::OnBeginFrame()
{
    if (Input::InputServer::Instance()->GetDefaultKeyboard()->KeyPressed(Input::Key::Escape))
    {
        Core::SysFunc::Exit(0);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GameStateManager::OnFrame()
{
#if __NEBULA_HTTP__
    if (Input::InputServer::Instance()->GetDefaultKeyboard()->KeyDown(Input::Key::F1))
    {
        // Open browser with debug page.
        Util::String url = "http://localhost:2100";
#ifdef __WIN32__
        ShellExecute(0, 0, url.AsCharPtr(), 0, 0, SW_SHOW);
#elif __LINUX__
        Util::String shellCommand = "open ";
        shellCommand.Append(url);
        system(shellCommand.AsCharPtr());
#else
        n_printf("Cannot open browser. URL is %s\n", url.AsCharPtr());
#endif
    }
#endif
}

} // namespace Game
