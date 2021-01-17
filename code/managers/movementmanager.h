#pragma once
//------------------------------------------------------------------------------
/**
	@class	Demo::MovementManager

	(C) 2020 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
#include "core/singleton.h"
#include "game/manager.h"

namespace Demo
{

class MovementManager
{
	__DeclareSingleton(MovementManager);
public:
	/// Create the singleton
	static Game::ManagerAPI Create();

	/// Destroy the singleton
	static void Destroy();

private:
	/// constructor
	MovementManager();
	/// destructor
	~MovementManager();

	/// called when attached to game server.
	static void OnActivate();
    /// called once before every rendered frame
    static void OnBeginFrame();
	/// called once every frame
	static void OnFrame();
};

} // namespace Game
