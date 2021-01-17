//------------------------------------------------------------------------------
//  inputmanager.cc
//  (C) 2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "application/stdneb.h"
#include "inputmanager.h"
#include "input/inputserver.h"
#include "input/mouse.h"
#include "input/keyboard.h"
#include "basegamefeature/managers/entitymanager.h"
#include "properties/input.h"

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
    Singleton = n_new(InputManager);

    Game::ManagerAPI api;
    api.OnActivate = &InputManager::OnActivate;
    api.OnBeginFrame = &InputManager::OnBeginFrame;
    return api;
}

//------------------------------------------------------------------------------
/**
*/
void
InputManager::Destroy()
{
    n_assert(InputManager::HasInstance());
    n_delete(Singleton);
}

//------------------------------------------------------------------------------
/**
*/
void
InputManager::OnActivate()
{
}

//------------------------------------------------------------------------------
/**
*/
void
ProcessPlayerInput()
{
    Ptr<Input::Keyboard> const& keyboard = Input::InputServer::Instance()->GetDefaultKeyboard();
    Ptr<Input::Mouse> const& mouse = Input::InputServer::Instance()->GetDefaultMouse();

    //Game::FilterSet filter(
    //    {
    //        Game::GetPropertyId("PlayerInput"_atm)
    //    },
    //    {
    //        // no excludes
    //    }
    //    );

    //Game::Dataset data = Game::Query(filter);

    //for (auto tbl : data.tables)
    //{
    //    Demo::PlayerInput* pInputs = (Demo::PlayerInput*)tbl.buffers[0];

    //    for (int i = 0; i < tbl.numInstances; i++)
    //    {
    //        auto& playerInput = pInputs[i];
    //        if (playerInput.hasFocus)
    //        {
    //            playerInput.forward = (char)keyboard->KeyPressed(Input::Key::Code::W) - (char)keyboard->KeyPressed(Input::Key::Code::S);
    //            playerInput.strafe = (char)keyboard->KeyPressed(Input::Key::Code::Right) - (char)keyboard->KeyPressed(Input::Key::Code::Left);
    //        }
    //    }
    //}
}

//------------------------------------------------------------------------------
/**
*/
void
InputManager::OnBeginFrame()
{
    ProcessPlayerInput();
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


