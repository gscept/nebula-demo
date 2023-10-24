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

    Game::Filter filter = Game::FilterBuilder().Including({ {Game::AccessMode::WRITE,  Game::GetComponentId<PlayerInput>() } }).Build();

    Game::ProcessorCreateInfo processorInfo;
    processorInfo.async = false;
    processorInfo.filter = filter;
    processorInfo.name = "InputManager.ProcessPlayerInput"_atm;
    processorInfo.OnBeginFrame = [&](Game::World*, Game::Dataset data)
    {
        for (int v = 0; v < data.numViews; v++)
        {
            Game::Dataset::EntityTableView const& view = data.views[v];
            Demo::PlayerInput* pInputs = (Demo::PlayerInput*)view.buffers[0];
            for (IndexT i = 0; i < view.numInstances; ++i)
            {
                auto& playerInput = pInputs[i];
                if (playerInput.hasFocus)
                {
                    playerInput.forward = (char)keyboard->KeyPressed(Input::Key::Code::W) - (char)keyboard->KeyPressed(Input::Key::Code::S);
                    playerInput.strafe = (char)keyboard->KeyPressed(Input::Key::Code::Right) - (char)keyboard->KeyPressed(Input::Key::Code::Left);
                }
            }
        }
    };
    Game::ProcessorHandle pHandle = Game::CreateProcessor(processorInfo);
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


