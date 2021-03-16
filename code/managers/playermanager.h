#pragma once
//------------------------------------------------------------------------------
/**
    @class	Demo::PlayerManager

    (C) 2020 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
#include "core/singleton.h"
#include "game/manager.h"
#include "util/stringatom.h"
#include "renderutil/freecamerautil.h"
#include "game/entity.h"

namespace Demo
{

class PlayerManager
{
    __DeclareSingleton(PlayerManager);
public:
    /// Create the singleton
    static Game::ManagerAPI Create();

    /// Destroy the singleton
    static void Destroy();

private:
    /// constructor
    PlayerManager();
    /// destructor
    ~PlayerManager();

    /// called when attached to game server.
    static void OnActivate();
    /// called once before every rendered frame
    static void OnBeginFrame();
    /// called before the world is cleaned up
    static void OnCleanup(Game::World*);

    Game::Entity playerEntity;

    RenderUtil::FreeCameraUtil freeCamUtil;
};

} // namespace Demo
