#pragma once
//------------------------------------------------------------------------------
/**
	@class	Demo::AiManager

	(C) 2023 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
#include "core/singleton.h"
#include "game/manager.h"

namespace Demo
{

class AiManager
{
	__DeclareSingleton(AiManager);
public:
	/// Create the singleton
	static Game::ManagerAPI Create();

	/// Destroy the singleton
	static void Destroy();

private:
	/// constructor
	AiManager();
	/// destructor
	~AiManager();

	/// called when attached to game server.
	static void OnActivate();
};

} // namespace Demo
