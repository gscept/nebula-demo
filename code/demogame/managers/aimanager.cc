//------------------------------------------------------------------------------
//  inputmanager.cc
//  (C) 2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "application/stdneb.h"
#include "aimanager.h"
#include "game/api.h"
#include "game/processor.h"
#include "properties/spaceship.h"
#include "game/world.h"
#include "util/random.h"
#include "basegamefeature/managers/timemanager.h"
#include "basegamefeature/components/position.h"

namespace Demo
{

__ImplementClass(Demo::AiManager, 'DAiM', Game::Manager);
__ImplementSingleton(AiManager)

//------------------------------------------------------------------------------
/**
*/
void
SteeringBehaviour()
{
    Game::TimeSource* time = Game::Time::GetTimeSource(TIMESOURCE_GAMEPLAY);

    auto func = [time](Game::World* world, Game::Entity const& entity, Game::Position const& position, SpaceShip const& ship, SpaceShipMoveParams& params, AiControlled& ai) {
        params.accelerate = true;
        if (time->ticks % 100 == 0)
        {
            params.boost = Util::FastRandom() % 2;
        }

        //Add a small random vector to the targets position.
        float const x = ai.wanderJitter * Util::RandomFloatNTP();
        float const y = ai.wanderJitter * Util::RandomFloatNTP();
        float const z = ai.wanderJitter * Util::RandomFloatNTP();
        Math::vec3 const wanderSphere = Math::vec3(x, y, z);
        Math::vec3 rotMovement = Math::normalize(wanderSphere) * 5.0f;

        params.rotateX = Math::lerp(params.rotateX, ai.xRotTarget, time->frameTime * 1.0f);
        params.rotateY = Math::lerp(params.rotateY, ai.yRotTarget, time->frameTime * 1.0f);
        params.rotateZ = Math::lerp(params.rotateZ, ai.zRotTarget, time->frameTime * 1.0f);

        // turn back ship if too far outside playable level
        if (position.x > 100.0f || position.x < -100.0f ||
            position.y > 100.0f || position.y < -100.0f ||
            position.z > 100.0f || position.z < -100.0f)
        {
            if (Math::dot(ship.orientation.z_axis(), -Math::normalize(position)) < 0.4f)
            {
                ai.xRotTarget = 1.0f;
                ai.yRotTarget = 0.0f;
                ai.zRotTarget = 0.0f;
            }
            else
            {
                ai.xRotTarget = 0.0f;
                ai.yRotTarget = 0.0f;
                ai.zRotTarget = 0.0f;
            }

            ai.nextSteeringUpdate = time->ticks + 1000;
        }

        if (ai.nextSteeringUpdate < time->ticks)
        {
            if (time->ticks % 100 == 0)
            {
                ai.xRotTarget = 0;
                ai.yRotTarget = 0;
                ai.zRotTarget = 0;
            }

            if (time->ticks % 200 == 0)
            {
                ai.xRotTarget = Math::clamp(rotMovement.x, -1.0f, 1.0f);
            }
            if (time->ticks % 300 == 0)
            {
                ai.yRotTarget = Math::clamp(rotMovement.x * 0.5f, -1.0f, 1.0f);
            }
            if (time->ticks % 500 == 0)
            {
                ai.zRotTarget = Math::clamp(rotMovement.x * 0.5f, -1.0f, 1.0f);
            }
        }
        };

    Game::World* world = Game::GetWorld(WORLD_DEFAULT);
    Game::ProcessorBuilder(world, "AiManager.SteeringBehaviour")
        .Func(func)
        .On("OnBeginFrame")
        // .Async() Cannot be async at the moment, since our random func is not thread safe
        .Build();
}

//------------------------------------------------------------------------------
/**
*/
AiManager::AiManager()
{
    __ConstructSingleton
}

//------------------------------------------------------------------------------
/**
*/
AiManager::~AiManager()
{
    __DestructSingleton
}

//------------------------------------------------------------------------------
/**
*/
void
AiManager::OnActivate()
{
    Game::Manager::OnActivate();
    SteeringBehaviour();
}

//------------------------------------------------------------------------------
/**
*/
void
AiManager::OnDeactivate()
{
    Game::Manager::OnDeactivate();
}

} // namespace Game


