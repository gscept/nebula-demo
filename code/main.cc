//------------------------------------------------------------------------------
//  main.cc
//  (C) 2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#define NEBULA_EDITOR_ENABLED

#include "application/stdneb.h"
#include "system/appentry.h"
#include "basegamefeature/basegamefeatureunit.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "physicsfeature/physicsfeatureunit.h"
#include "appgame/gameapplication.h"
#include "demogamefeatureunit.h"
#include "gamestatemanager.h"
#include "profiling/profiling.h"
#include "scripting/python/pythonserver.h"

#ifdef NEBULA_EDITOR_ENABLED
#include "editorfeature/editorfeatureunit.h"
#endif

ImplementNebulaApplication();

using namespace Core;


class NebulaDemoApplication : public App::GameApplication
{
private:
    /// setup game features
	void SetupGameFeatures()
	{
#if NEBULA_ENABLE_PROFILING
        Profiling::ProfilingRegisterThread();
#endif

        this->gameFeature = BaseGameFeature::BaseGameFeatureUnit::Create();
        this->gameServer->AttachGameFeature(gameFeature);

        this->graphicsFeature = GraphicsFeature::GraphicsFeatureUnit::Create();
        this->graphicsFeature->SetCmdLineArgs(this->GetCmdLineArgs());
        this->gameServer->AttachGameFeature(this->graphicsFeature);

		this->physicsFeature = PhysicsFeature::PhysicsFeatureUnit::Create();
        this->gameServer->AttachGameFeature(this->physicsFeature);

        this->demoFeatureUnit = Demo::DemoGameFeatureUnit::Create();
        this->gameServer->AttachGameFeature(this->demoFeatureUnit);

#ifdef NEBULA_EDITOR_ENABLED
        this->scriptserver = Scripting::PythonServer::Create();
        this->scriptserver->Open();

        this->editorFeatureUnit = EditorFeature::EditorFeatureUnit::Create();
        this->gameServer->AttachGameFeature(this->editorFeatureUnit);
#endif
	}
	/// cleanup game features
	void CleanupGameFeatures()
	{
        //this->gameServer->RemoveGameFeature(this->physicsFeature);
        this->gameServer->RemoveGameFeature(this->graphicsFeature);
        this->gameServer->RemoveGameFeature(this->demoFeatureUnit);
		this->gameServer->RemoveGameFeature(this->gameFeature);
		this->gameFeature->CleanupWorld();
        this->gameFeature->Release();
        this->gameFeature = nullptr;
        this->physicsFeature->Release();
        this->physicsFeature = nullptr;
        this->graphicsFeature->Release();
        this->graphicsFeature = nullptr;
        this->demoFeatureUnit->Release();
        this->demoFeatureUnit = nullptr;

#ifdef NEBULA_EDITOR_ENABLED
        this->editorFeatureUnit->Release();
        this->editorFeatureUnit = nullptr;
#endif
	}

	Ptr<BaseGameFeature::BaseGameFeatureUnit> gameFeature;
    Ptr<GraphicsFeature::GraphicsFeatureUnit> graphicsFeature;
    Ptr<PhysicsFeature::PhysicsFeatureUnit> physicsFeature;
    Ptr<Demo::DemoGameFeatureUnit> demoFeatureUnit;

#ifdef NEBULA_EDITOR_ENABLED
    Ptr<Scripting::ScriptServer> scriptserver;
    Ptr<EditorFeature::EditorFeatureUnit> editorFeatureUnit;
#endif
};

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const Util::CommandLineArgs& args)
{
    NebulaDemoApplication gameApp;
	gameApp.SetCompanyName("Gscept");
	gameApp.SetAppTitle("Nebula demo");
    gameApp.SetCmdLineArgs(args);

	if (!gameApp.Open())
	{
		return;
	}

    gameApp.Run();

    Core::SysFunc::Exit(0);
}
