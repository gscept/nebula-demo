#pragma once
//------------------------------------------------------------------------------
/**
    Demo::DemoGameFeatureUnit

    (C) 2020 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
#include "core/singleton.h"
#include "game/featureunit.h"
#include "game/manager.h"

namespace Demo
{

class DemoGameFeatureUnit : public Game::FeatureUnit
{
    __DeclareClass(DemoGameFeatureUnit)
        __DeclareSingleton(DemoGameFeatureUnit)
public:
    ///
    DemoGameFeatureUnit();
    ///
    ~DemoGameFeatureUnit();
    ///
    void OnActivate();
    ///
    void OnDeactivate();
    ///
    void OnBeginFrame() override;
};

} // namespace Demo




