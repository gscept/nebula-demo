//------------------------------------------------------------------------------
//  movementmanager.cc
//  (C) 2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "application/stdneb.h"
#include "math/scalar.h"
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
#include "basegamefeature/components/position.h"
#include "basegamefeature/components/orientation.h"
#include "physicsfeature/components/physicsfeature.h"
#include "game/processor.h"
#include "properties/spaceship.h"
#include "game/world.h"

namespace Demo
{

__ImplementClass(Demo::MovementManager, 'DMvM', Game::Manager);
__ImplementSingleton(MovementManager)

void SimulateSpaceShip()
{
    using namespace Math;
    Game::TimeSource const* const time = Game::Time::GetTimeSource(TIMESOURCE_GAMEPLAY);
    std::function SimulateSpaceShip = [time](Game::World* world, Game::Entity& entity, Game::Position& position, Game::Orientation& orientation, SpaceShip& ship, SpaceShipMoveParams const& move) -> void
        {
            if (move.accelerate)
            {
                if (move.boost)
                    ship.currentSpeed = lerp(ship.currentSpeed, ship.boostSpeed, min(1.0, time->frameTime * 30.0));
                else
                    ship.currentSpeed = lerp(ship.currentSpeed, ship.normalSpeed, min(1.0, time->frameTime * 90.0));
            }
            else
            {
                ship.currentSpeed = 0;
            }

            vec3 desiredVelocity = vec3(0, 0, ship.currentSpeed);
            desiredVelocity = Math::rotate(orientation, desiredVelocity);

            ship.linearVelocity = lerp(ship.linearVelocity, desiredVelocity, time->frameTime * ship.accelerationFactor);

            position += ship.linearVelocity * time->frameTime * 10.0f;

            ship.rotXSmooth = lerp(ship.rotXSmooth, move.rotateX * ship.rotationSpeed * time->frameTime, time->frameTime * ship.rotationSmoothFactor);
            ship.rotYSmooth = lerp(ship.rotYSmooth, move.rotateY * ship.rotationSpeed * time->frameTime, time->frameTime * ship.rotationSmoothFactor);
            ship.rotZSmooth = lerp(ship.rotZSmooth, move.rotateZ * ship.rotationSpeed * time->frameTime, time->frameTime * ship.rotationSmoothFactor);

            quat localOrientation = from_euler(vec3(ship.rotXSmooth, ship.rotYSmooth, ship.rotZSmooth));
            ship.orientation = localOrientation * ship.orientation;
            ship.rotationZ -= ship.rotXSmooth;
            //ship.rotationZ = clamp(ship.rotationZ, -45.0f, 45.0f);
            orientation = { quat(0.0f, 0.0f, ship.rotationZ, 1.0f) * ship.orientation };
            orientation = { Math::normalize(orientation) };
            ship.rotationZ = lerp(ship.rotationZ, 0.0f, time->frameTime * ship.rotationSmoothFactor);
        };

    Game::World* world = Game::GetWorld(WORLD_DEFAULT);
    Game::ProcessorBuilder(world, "MovementManager.SimulateSpaceShip")
        .Func(SimulateSpaceShip)
        .On("OnBeginFrame")
        .Async()
        .Build();
}

//------------------------------------------------------------------------------
/**
*/
MovementManager::MovementManager()
{
    __ConstructSingleton
}

//------------------------------------------------------------------------------
/**
*/
MovementManager::~MovementManager()
{
    __DestructSingleton
}

//------------------------------------------------------------------------------
/**
*/
void
MovementManager::OnActivate()
{
    Game::Manager::OnActivate();

    Game::TimeSource const* const time = Game::Time::GetTimeSource(TIMESOURCE_GAMEPLAY);
    std::function WanderMovementUpdate = [time](Game::World* world, Game::Entity& entity, Game::Position& pos, Movement& move) -> void
    {
        //Add a small random vector to the targets position.
        float const x = move.wanderJitter * (Util::RandomFloatNTP() * move.wanderRadius);
        float const z = move.wanderJitter * (Util::RandomFloatNTP() * move.wanderRadius);
        Math::vec3 const wanderCircle = Math::vec3(x, 0, z);
        move.direction = Math::normalize((move.direction + Math::normalize((wanderCircle + (Math::normalize(move.direction * move.wanderDistance))))));
        pos += (move.direction * move.speed * time->frameTime);
    };

    Game::World* world = Game::GetWorld(WORLD_DEFAULT);
    Game::ProcessorBuilder(world, "MovementManager.WanderUpdateMovement")
        .Func(WanderMovementUpdate)
        .On("OnBeginFrame")
        .Async()
        .Build();

    SimulateSpaceShip();
}

//------------------------------------------------------------------------------
/**
*/
void
MovementManager::OnDeactivate()
{
    Game::Manager::OnDeactivate();
}

} // namespace Game


