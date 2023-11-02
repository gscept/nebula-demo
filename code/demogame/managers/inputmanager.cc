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
InputManager::OnActivate()
{
    Ptr<Input::Keyboard> const& keyboard = Input::InputServer::Instance()->GetDefaultKeyboard();
    Ptr<Input::Mouse> const& mouse = Input::InputServer::Instance()->GetDefaultMouse();

    Game::World* world = Game::GetWorld(WORLD_DEFAULT);
    Game::ProcessorBuilder(world, "InputManager.ProcessPlayerInput"_atm)
        .Func([&keyboard, &mouse](Game::World* world, Demo::PlayerInput& playerInput)
            {
                if (playerInput.hasFocus)
                {
                    playerInput.forward = (char)keyboard->KeyPressed(Input::Key::Code::W) - (char)keyboard->KeyPressed(Input::Key::Code::S);
                    playerInput.strafe = (char)keyboard->KeyPressed(Input::Key::Code::Right) - (char)keyboard->KeyPressed(Input::Key::Code::Left);
                }
            })
        .Build();
}

//------------------------------------------------------------------------------
/**
*/
void
ProcessPlayerInput()
{
   
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


