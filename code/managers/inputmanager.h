#pragma once
//------------------------------------------------------------------------------
/**
	@class	Demo::InputManager

	(C) 2020 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
#include "core/singleton.h"
#include "game/manager.h"

namespace Demo
{

class InputManager
{
	__DeclareSingleton(InputManager);
public:
	/// Create the singleton
	static Game::ManagerAPI Create();

	/// Destroy the singleton
	static void Destroy();

private:
	/// constructor
	InputManager();
	/// destructor
	~InputManager();

	/// called when attached to game server.
	static void OnActivate();
    /// called once before every rendered frame
	static void OnBeginFrame();
};

} // namespace Demo
