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

class MovementManager : public Game::Manager
{
	__DeclareClass(MovementManager)
	__DeclareSingleton(MovementManager)
public:
	MovementManager();
	virtual ~MovementManager();

	void OnActivate() override;
	void OnDeactivate() override;
};

} // namespace Game
