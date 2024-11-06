//------------------------------------------------------------------------------
//  gamestatemanager.cc
//  (C) 2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "application/stdneb.h"
#include "gamestatemanager.h"
#include "models/modelcontext.h"
#include "graphics/graphicsentity.h"
#include "visibility/visibilitycontext.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "basegamefeature/components/basegamefeature.h"
#include "basegamefeature/components/position.h"
#include "basegamefeature/components/orientation.h"
#include "basegamefeature/components/velocity.h"
#include "physicsfeature/components/physicsfeature.h"
#include "physicsfeature/managers/physicsmanager.h"
#include "physics/actorcontext.h"
#include "input/inputserver.h"
#include "input/keyboard.h"
#include "dynui/im3d/im3dcontext.h"
#include "imgui.h"
#include "util/random.h"
#include "characters/charactercontext.h"
#include "models/nodes/shaderstatenode.h"
#include "dynui/im3d/im3d.h"
#include "lighting/lightcontext.h"
#include "decals/decalcontext.h"
#include "resources/resourceserver.h"
#include "terrain/terraincontext.h"
#include "coregraphics/legacy/nvx2streamreader.h"
#include "coregraphics/primitivegroup.h"
#include "properties/movement.h"

#include "graphicsfeature/managers/graphicsmanager.h"
#include "game/gameserver.h"
#include "game/api.h"
#include "managers/playermanager.h"

#ifdef __WIN32__
#include <shellapi.h>
#elif __LINUX__

#endif

namespace Demo
{

__ImplementSingleton(GameStateManager)

//------------------------------------------------------------------------------
/**
*/
Game::ManagerAPI
GameStateManager::Create()
{
    n_assert(!GameStateManager::HasInstance());
    Singleton = new GameStateManager;

    Game::ManagerAPI api;
    api.OnActivate = &GameStateManager::OnActivate;
    api.OnBeginFrame = &GameStateManager::OnBeginFrame;
    api.OnFrame = &GameStateManager::OnFrame;
    return api;
}

//------------------------------------------------------------------------------
/**
*/
void
GameStateManager::Destroy()
{
    n_assert(GameStateManager::HasInstance());
    delete Singleton;
}

//------------------------------------------------------------------------------
/**
*/
GameStateManager::GameStateManager()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
GameStateManager::~GameStateManager()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
GameStateManager::OnActivate()
{
     auto gameWorld = Game::GetWorld(WORLD_DEFAULT);
    Game::EntityCreateInfo playerInfo;
    playerInfo.immediate = true;
    playerInfo.templateId = Game::GetTemplateId("PlayerSpaceShip/default"_atm);
    gameWorld->CreateEntity(playerInfo);
    
    Game::EntityCreateInfo terrainInfo;
    terrainInfo.immediate = true;
    terrainInfo.templateId = Game::GetTemplateId("OnlyCollider/terrain"_atm);
    Game::Entity terrain = gameWorld->CreateEntity(terrainInfo);
    
    for (int i = 0; i < 100; i++)
    {
        const float span = 100.0f;
        Game::EntityCreateInfo playerInfo;
        playerInfo.immediate = true;
        playerInfo.templateId = Game::GetTemplateId("AiSpaceShip/default"_atm);
        Game::Entity entity = gameWorld->CreateEntity(playerInfo);
        gameWorld->SetComponent<Game::Position>(
            entity, 
            Math::vec3(Util::RandomFloatNTP() * span,
                       Util::RandomFloatNTP() * span,
                       Util::RandomFloatNTP() * span)
        );

    }
    
    {
        Game::TemplateId asteroidTemplates[] = {
            Game::GetTemplateId("StaticEnvironment/asteroid_1"_atm),
            Game::GetTemplateId("StaticEnvironment/asteroid_2"_atm),
            Game::GetTemplateId("StaticEnvironment/asteroid_3"_atm),
            Game::GetTemplateId("StaticEnvironment/asteroid_4"_atm),
            Game::GetTemplateId("StaticEnvironment/asteroid_5"_atm),
            Game::GetTemplateId("StaticEnvironment/asteroid_6"_atm),
        };
        constexpr size_t numTemplateTypes = sizeof(asteroidTemplates) / sizeof(Game::TemplateId);
    
        Game::EntityCreateInfo info;
        info.immediate = false;
        
        // Setup asteroids near
        for (int i = 0; i < 100; i++)
        {
            info.templateId = asteroidTemplates[Util::FastRandom() % numTemplateTypes];
            Game::Entity asteroid = gameWorld->CreateEntity(info);
            
            float span = 200.0f;
            Math::vec3 translation = Math::vec3(
                Util::RandomFloatNTP() * span,
                Util::RandomFloatNTP() * span,
                Util::RandomFloatNTP() * span
            );
            
            Math::vec3 rotationAxis = normalize(translation);
            float rotation = translation.x;
            Math::quat const orient = Math::rotationquataxis(rotationAxis, rotation);
            gameWorld->SetComponent<Game::Position>(asteroid, { translation });
            gameWorld->SetComponent<Game::Orientation>(asteroid, { orient });
            GraphicsFeature::Model model = gameWorld->GetComponent<GraphicsFeature::Model>(asteroid);
            int a = 0;
        }
    
        // Setup asteroids far
        for (int i = 0; i < 50; i++)
        {
            info.templateId = asteroidTemplates[Util::FastRandom() % numTemplateTypes];
            Game::Entity asteroid = gameWorld->CreateEntity(info);
    
            float span = 80.0f;
            Math::vec3 translation = Math::vec3(
                Util::RandomFloatNTP() * span,
                Util::RandomFloatNTP() * span,
                Util::RandomFloatNTP() * span
            );
    
            Math::vec3 rotationAxis = normalize(translation);
            float rotation = translation.x;
            Math::quat const orient = Math::rotationquataxis(rotationAxis, rotation);
            gameWorld->SetComponent<Game::Position>(asteroid, { translation });
            gameWorld->SetComponent<Game::Orientation>(asteroid, { orient });
        }
    }
}

Util::Queue<Game::Entity> entities;

//------------------------------------------------------------------------------
/**
*/
void
GameStateManager::OnBeginFrame()
{
    if (Input::InputServer::Instance()->GetDefaultKeyboard()->KeyPressed(Input::Key::Escape))
    {
        Core::SysFunc::Exit(0);
    }
    
    
    if (Input::InputServer::Instance()->GetDefaultMouse()->ButtonPressed(Input::MouseButton::Code::LeftButton))
    {
        /*
        auto gameWorld = Game::GetWorld(WORLD_DEFAULT);
        for (size_t i = 0; i < 1; i++)
        {
            //auto cam = PlayerManager::Instance()->mainCameraEntity;
            //GraphicsFeature::Camera camera = gameWorld->GetComponent<GraphicsFeature::Camera>(cam);
            //Math::mat4 camTransform = camera.localTransform;// gameWorld->GetComponent(cam, camera);

            Math::mat4 camTransform = Math::inverse(GraphicsFeature::CameraManager::GetLocalTransform(GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultViewHandle()));
            Game::EntityCreateInfo info;
            info.immediate = false;
            info.templateId = Game::GetTemplateId("PhysicsEntity/placeholder_box"_atm);
            Game::Entity entity = gameWorld->CreateEntity(info);
            entities.Enqueue(entity);
            Math::vec3 pos = (camTransform.position).vec;// -(camTransform.z_axis * (3.0f + Math::rand(-1.0f, 2.0f)))).vec;
            gameWorld->SetComponent<Game::Position>(entity, pos);
            gameWorld->SetComponent<Game::Velocity>(entity, (camTransform.z_axis * -(20.0f + Math::rand(-10.0f, 10.0f))).vec);
            gameWorld->SetComponent<Game::AngularVelocity>(entity, Math::vec3(Math::rand(-5.0f, 5.0f), Math::rand(-5.0f, 5.0f), Math::rand(-5.0f, 5.0f)));
        }
        */
    }
    
    static int frameIndex = 0;
    if (frameIndex % 10 == 0)
    {
        int limit = 100;
        if (Input::InputServer::Instance()->GetDefaultKeyboard()->KeyDown(Input::Key::F2))
        {
            limit = 0;
        }
        while (entities.Size() > limit)
        {
            Game::GetWorld(WORLD_DEFAULT)->DeleteEntity(entities.Dequeue());
        }
    }
    frameIndex++;

    //ImGui::Begin("WORLD_DEBUG");
    //Game::WorldRenderDebug(Game::GetWorld(WORLD_DEFAULT));
    //ImGui::End();
}

//------------------------------------------------------------------------------
/**
*/
void
GameStateManager::OnFrame()
{
#if __NEBULA_HTTP__
    if (Input::InputServer::Instance()->GetDefaultKeyboard()->KeyDown(Input::Key::F1))
    {
        // Open browser with debug page.
        Util::String url = "http://localhost:2100";
#ifdef __WIN32__
        ShellExecute(0, 0, url.AsCharPtr(), 0, 0, SW_SHOW);
#elif __LINUX__
        Util::String shellCommand = "open ";
        shellCommand.Append(url);
        system(shellCommand.AsCharPtr());
#else
        n_printf("Cannot open browser. URL is %s\n", url.AsCharPtr());
#endif
    }
#endif
}

} // namespace Game
