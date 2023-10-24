//------------------------------------------------------------------------------
//  movementmanager.cc
//  (C) 2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "application/stdneb.h"
#include "movementmanager.h"
#include "properties/movement.h"
#include "properties/input.h"
#include "graphics/graphicsentity.h"
#include "math/scalar.h"
#include "models/modelcontext.h"
#include "timing/timer.h"
#include "util/random.h"
#include "basegamefeature/managers/timemanager.h"
#include "game/api.h"
#include "basegamefeature/components/basegamefeature.h"

#include "physicsfeature/components/physicsfeature.h"

namespace Demo
{

__ImplementSingleton(MovementManager)

//------------------------------------------------------------------------------
/**
*/
Game::ManagerAPI
MovementManager::Create()
{
    n_assert(!MovementManager::HasInstance());
    Singleton = new MovementManager;

    Game::TimeSource const* const time = Game::TimeManager::GetTimeSource(TIMESOURCE_GAMEPLAY);
    std::function WanderMovementUpdate = [time](Game::World* world, Game::Owner& entity, Game::Transform& t, Movement& move) -> void
    {
        //Add a small random vector to the targets position.
        float const x = move.wanderJitter * (Util::RandomFloatNTP() * move.wanderRadius);
        float const z = move.wanderJitter * (Util::RandomFloatNTP() * move.wanderRadius);
        Math::vec3 const wanderCircle = Math::vec3(x, 0, z);
        move.direction = Math::normalize((move.direction + Math::normalize((wanderCircle + (Math::normalize(move.direction * move.wanderDistance))))));
        t.value.position += (move.direction * move.speed * time->frameTime).vec;

    };

    Game::ProcessorBuilder("MovementManager.WanderUpdateMovement")
        .Func(WanderMovementUpdate)
        .On("OnBeginFrame")
        .Build();

    Game::ManagerAPI api;
    api.OnActivate = &MovementManager::OnActivate;
    api.OnBeginFrame = &MovementManager::OnBeginFrame;
    api.OnFrame = &MovementManager::OnFrame;
    return api;
}

//------------------------------------------------------------------------------
/**
*/
void
MovementManager::Destroy()
{
    n_assert(MovementManager::HasInstance());
    delete Singleton;
}

//------------------------------------------------------------------------------
/**
*/
void
MovementManager::OnActivate()
{
   
}

//------------------------------------------------------------------------------
/**
*/
void
MovementManager::OnBeginFrame()
{
   
}

//------------------------------------------------------------------------------
/**
*/
void
HandlePlayerInput()
{
    //Game::FilterSet playerfilter(
    //    {
    //        Game::GetComponentId("Owner"_atm),
    //        Game::GetComponentId("PlayerInput"_atm)
    //    }
    //);
    //
    //Game::Dataset data = Game::Query(playerfilter);
    //
    //for (auto& tbl : data.tables)
    //{
    //    Game::Entity* owners = (Game::Entity*)tbl.buffers[0];
    //    PlayerInput* pInputs = (PlayerInput*)tbl.buffers[1];
    //   
    //    Game::CategoryId const cid = Game::GetEntityMapping(owners[0]).category;
    //
    //    for (int i = 0; i < tbl.numInstances; i++)
    //    {
    //        
    //    }
    //}
}

//------------------------------------------------------------------------------
/**
*/
void
MovementManager::OnFrame()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
MovementManager::MovementManager()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
MovementManager::~MovementManager()
{
    // empty
}

} // namespace Game


