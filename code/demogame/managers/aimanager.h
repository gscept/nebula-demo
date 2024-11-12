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

class AiManager : public Game::Manager
{
	__DeclareClass(AiManager);
	__DeclareSingleton(AiManager);
public:
	AiManager();
	virtual ~AiManager();

	void OnActivate() override;
	void OnDeactivate() override;
};

} // namespace Demo
