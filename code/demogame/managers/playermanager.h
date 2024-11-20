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

class PlayerManager : public Game::Manager
{
    __DeclareClass(PlayerManager)
    __DeclareSingleton(PlayerManager)
public:
    PlayerManager();
    virtual ~PlayerManager();

    void OnActivate() override;
    void OnDeactivate() override;
    void OnBeginFrame() override;
    void OnCleanup(Game::World*) override;

    Game::Entity mainCameraEntity;
    RenderUtil::FreeCameraUtil freeCamUtil;
};

} // namespace Demo
