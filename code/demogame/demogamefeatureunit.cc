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
    Game::World* world = Game::GetWorld(WORLD_DEFAULT);
    world->RegisterType<Demo::PlayerInput>();
    world->RegisterType<Demo::Movement>();
    world->RegisterType<Demo::ShotSpawn>();
    world->RegisterType<Demo::CameraFollow>();
    world->RegisterType<Demo::SpaceShip>();
    world->RegisterType<Demo::SpaceShipMoveParams>();
    world->RegisterType<Demo::AiControlled>();
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
