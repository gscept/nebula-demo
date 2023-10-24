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
    void OnAttach() override;
    ///
    void OnActivate() override;
    ///
    void OnDeactivate() override;
    ///
    void OnBeginFrame() override;
};

} // namespace Demo




