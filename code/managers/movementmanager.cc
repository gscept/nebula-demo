//------------------------------------------------------------------------------
//  movementmanager.cc
//  (C) 2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "application/stdneb.h"
#include "movementmanager.h"
#include "basegamefeature/managers/entitymanager.h"
#include "properties/movement.h"
#include "properties/input.h"
#include "graphics/graphicsentity.h"
#include "math/scalar.h"
#include "models/modelcontext.h"
#include "timing/timer.h"
#include "util/random.h"
#include "basegamefeature/managers/timemanager.h"

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
    Singleton = n_new(MovementManager);

    Game::FilterCreateInfo filterInfo;
    filterInfo.inclusive[0] = Game::GetPropertyId("WorldTransform");
    filterInfo.access[0] = Game::AccessMode::WRITE;
    filterInfo.inclusive[1] = Game::GetPropertyId("Movement");
    filterInfo.access[1] = Game::AccessMode::WRITE;

    filterInfo.numInclusive = 2;

    Game::Filter filter = Game::CreateFilter(filterInfo);

    Game::ProcessorCreateInfo processorInfo;
    processorInfo.async = false;
    processorInfo.filter = filter;
    processorInfo.name = "MovementManager"_atm;
    processorInfo.OnBeginFrame = [](Game::Dataset data)
    {
        Game::TimeSource const* const time = Game::TimeManager::GetTimeSource(TIMESOURCE_GAMEPLAY);
        for (int v = 0; v < data.numViews; v++)
        {
            Game::Dataset::CategoryTableView const& view = data.views[v];
            Math::mat4* const transforms = (Math::mat4*)view.buffers[0];
            Movement* const moves = (Movement*)view.buffers[1];

            for (IndexT i = 0; i < view.numInstances; ++i)
            {
                Math::mat4& t = transforms[i];
                Movement& move = moves[i];

                //Add a small random vector to the targets position.
                float const x = move.wanderJitter * (Util::RandomFloatNTP() * move.wanderRadius);
                float const z = move.wanderJitter * (Util::RandomFloatNTP() * move.wanderRadius);
                Math::vec3 const wanderCircle = Math::vec3(x, 0, z);
                move.direction = Math::normalize((move.direction + Math::normalize((wanderCircle + (Math::normalize(move.direction * move.wanderDistance))))));
                t.position += (move.direction * move.speed * time->frameTime).vec;
            }
        }
    };

    Game::ProcessorHandle pHandle = Game::CreateProcessor(processorInfo);

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
    n_delete(Singleton);
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
    //        Game::GetPropertyId("Owner"_atm),
    //        Game::GetPropertyId("PlayerInput"_atm)
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


