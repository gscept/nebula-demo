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
    Game::RegisterComponent<Demo::PlayerInput>();
    Game::RegisterComponent<Demo::Movement>();
    Game::RegisterComponent<Demo::ShotSpawn>();
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
}

//------------------------------------------------------------------------------
/**
*/
void
DemoGameFeatureUnit::OnBeginFrame()
{
    FeatureUnit::OnBeginFrame();
    #if NEBULA_ENABLE_PROFILING
        Profiling::ProfilingNewFrame();
    #endif
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
