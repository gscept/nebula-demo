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
#include "game/world.h"

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

            if (Input::InputServer::Instance()->GetDefaultMouse()->ButtonPressed(Input::MouseButton::Code::RightButton))
                return;
            
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


