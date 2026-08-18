//------------------------------------------------------------------------------
//  gamestatemanager.cc
//  (C) 2020-2024 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "application/stdneb.h"
#include "gamestatemanager.h"
#include "input/inputserver.h"
#include "input/keyboard.h"


#ifdef __WIN32__
#include <shellapi.h>
#elif __LINUX__

#endif

namespace Demo
{

__ImplementClass(Demo::GameStateManager, 'DGSM', Game::Manager);
__ImplementSingleton(GameStateManager)

//------------------------------------------------------------------------------
/**
*/
GameStateManager::GameStateManager()
{
    __ConstructSingleton
}

//------------------------------------------------------------------------------
/**
*/
GameStateManager::~GameStateManager()
{
    __DestructSingleton
}

//------------------------------------------------------------------------------
/**
*/
void
GameStateManager::OnActivate()
{
    Game::Manager::OnActivate();
}

//------------------------------------------------------------------------------
/**
*/
void
GameStateManager::OnDeactivate()
{
    Game::Manager::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
*/
void
GameStateManager::OnBeginFrame()
{
    Game::Manager::OnBeginFrame();
}

//------------------------------------------------------------------------------
/**
*/
void
GameStateManager::OnFrame()
{
#if __NEBULA_HTTP__
    if (Input::InputServer::Instance()->GetDefaultKeyboard()->KeyDown(Input::Key::F1))
    {
        // Open browser with debug page.
        Util::String url = "http://localhost:2100";
#ifdef __WIN32__
        ShellExecute(0, 0, url.AsCharPtr(), 0, 0, SW_SHOW);
#elif __LINUX__
        Util::String shellCommand = "open ";
        shellCommand.Append(url);
        system(shellCommand.AsCharPtr());
#else
        n_printf("Cannot open browser. URL is %s\n", url.AsCharPtr());
#endif
    }
#endif
}

} // namespace Game
