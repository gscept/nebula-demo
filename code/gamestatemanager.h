#pragma once
//------------------------------------------------------------------------------
/**
    @class  Demo::GameStateManager

    (C) 2020 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
#include "core/singleton.h"
#include "game/manager.h"
#include "coregraphics/mesh.h"
#include "coregraphics/memorymeshpool.h"
#include "renderutil/mouserayutil.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "input/inputserver.h"
#include "io/ioserver.h"
#include "graphics/cameracontext.h"
#include "models/nodes/primitivenode.h"
#include "models/modelcontext.h"
#include "imgui.h"
#include "dynui/im3d/im3d.h"
#include "dynui/im3d/im3dcontext.h"
#include "timing/timer.h"

namespace Demo
{

class GameStateManager
{
    __DeclareSingleton(GameStateManager);
public:
    /// Create the singleton
    static Game::ManagerAPI Create();

    /// Destroy the singleton
    static void Destroy();

private:
    /// constructor
    GameStateManager();
    /// destructor
    ~GameStateManager();

    /// called when attached to game server.
    static void OnActivate();
    /// called once before every rendered frame
    static void OnBeginFrame();
    /// called once every frame
    static void OnFrame();
};

} // namespace Demo
