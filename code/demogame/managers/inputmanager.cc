//------------------------------------------------------------------------------
//  inputmanager.cc
//  (C) 2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "application/stdneb.h"
#include "inputmanager.h"
#include "input/inputserver.h"
#include "input/mouse.h"
#include "input/keyboard.h"
#include "properties/input.h"
#include "game/api.h"
#include "memdb/table.h"
#include "memdb/filterset.h"
#include "memdb/database.h"
#include "game/processor.h"
#include "properties/spaceship.h"
#include "properties/projectile.h"
#include "game/world.h"
#include "playermanager.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "physicsfeature/components/physicsfeature.h"

#include "audiofeature/components/audiofeature.h"

namespace Demo
{

__ImplementSingleton(InputManager)

//------------------------------------------------------------------------------
/**
*/
Game::ManagerAPI
InputManager::Create()
{
    n_assert(!InputManager::HasInstance());
    Singleton = new InputManager;

    Game::ManagerAPI api;
    api.OnActivate = &InputManager::OnActivate;
    return api;
}

//------------------------------------------------------------------------------
/**
*/
void
InputManager::Destroy()
{
    n_assert(InputManager::HasInstance());
    delete Singleton;
}

//------------------------------------------------------------------------------
/**
*/
void
PollSpaceShipInput()
{
    Ptr<Input::Keyboard> const& keyboard = Input::InputServer::Instance()->GetDefaultKeyboard();

    using namespace Math;
    std::function PollInput = [keyboard](
        Game::World* world,
        Game::Entity const& entity,
        PlayerInput const& playerInput,
        SpaceShipMoveParams& params
        )
        {
            if (!playerInput.hasFocus)
                return;

            //if (Input::InputServer::Instance()->GetDefaultMouse()->ButtonPressed(Input::MouseButton::Code::RightButton))
            //    return;
            
            params.accelerate = keyboard->KeyPressed(Input::Key::Code::W);
            params.boost = keyboard->KeyPressed(Input::Key::Code::LeftShift);
            params.rotateX = keyboard->KeyPressed(Input::Key::Code::Left) ?  1.0f : keyboard->KeyPressed(Input::Key::Code::Right) ? -1.0f : 0.0f;
            params.rotateY = keyboard->KeyPressed(Input::Key::Code::Up)   ?  1.0f : keyboard->KeyPressed(Input::Key::Code::Down)  ? -1.0f : 0.0f;
            params.rotateZ = keyboard->KeyPressed(Input::Key::Code::A)    ? -1.0f : keyboard->KeyPressed(Input::Key::Code::D)     ?  1.0f : 0.0f;

            if (keyboard->KeyDown(Input::Key::Code::Space))
            {
                if (!world->HasComponent<AudioFeature::PlayAudioEvent>(entity))
                {
                    world->AddComponent<AudioFeature::PlayAudioEvent>(entity);
                }
            }
            if (Input::InputServer::Instance()->GetDefaultMouse()->ButtonDown(Input::MouseButton::Code::LeftButton))
            {
                Game::EntityCreateInfo info;
                info.immediate = false;
                info.templateId = Game::GetTemplateId("Projectiles/boxshot"_atm);
                Game::Entity pentity = world->CreateEntity(info);
                Math::vec3 pos = world->GetComponent<Game::Position>(entity);
                auto orient = world->GetComponent<Game::Orientation>(entity);
                Math::vec3 forward = Math::rotate(orient, Math::vec3(0.0f, 0.0f, 1.0f));
                auto const& projectile = world->GetComponent<Demo::Projectile>(pentity);
                float speed = projectile.speed;
                pos += (forward * (5.0f + Math::rand(-1.0f, 2.0f))).vec;
                world->SetComponent<Game::Position>(pentity, pos);
                world->SetComponent<Game::Velocity>(pentity, (forward * (speed + Math::rand(-10.0f, 10.0f))).vec);
                world->SetComponent<Game::AngularVelocity>(pentity, Math::vec3(Math::rand(-5.0f, 5.0f), Math::rand(-5.0f, 5.0f), Math::rand(-5.0f, 5.0f)));
                __RegisterMsg(PhysicsFeature::ContactEventMessage, [](Game::Entity entity, const Physics::ContactEvent& event)
                {
                    Game::World* world = Game::GetWorld(WORLD_DEFAULT);
                    if (world->HasInstance(entity))
                    {
                        if (!world->HasComponent<Demo::PlayerInput>(entity))
                        {
                            //auto const& projectile = world->GetComponent<Demo::Projectile>(entity);
                            //world->DeleteEntity(entity);
                        }
                    }
                });
            }
        };

    Game::World* world = Game::GetWorld(WORLD_DEFAULT);
    Game::ProcessorBuilder(world, "InputManager.PollSpaceShipInput")
        .Func(PollInput)
        .On("OnBeginFrame")
        .Order(99)
        .Build();
}

//------------------------------------------------------------------------------
/**
*/
void
InputManager::OnActivate()
{
    PollSpaceShipInput();
}

//------------------------------------------------------------------------------
/**
*/
InputManager::InputManager()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
InputManager::~InputManager()
{
    // empty
}


} // namespace Game


