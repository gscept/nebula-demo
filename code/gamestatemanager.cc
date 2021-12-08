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
#include "basegamefeature/properties/transform.h"
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

#include "materials/materialserver.h"
#include "graphicsfeature/managers/graphicsmanager.h"
#include "game/gameserver.h"
#include "game/api.h"

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
    Singleton = n_new(GameStateManager);

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
    n_delete(Singleton);
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
    auto dood = Graphics::CreateEntity();
    Graphics::RegisterEntity<Models::ModelContext, Visibility::ObservableContext>(dood);
    Models::ModelContext::Setup(dood, "mdl:system/placeholder.n3", "ExampleScene", [dood]()
    {
        Visibility::ObservableContext::Setup(dood, Visibility::VisibilityEntityType::Model);
    });
    Models::ModelContext::SetTransform(dood, Math::translation(Math::vec3(0, 0, 5)));

    //{
    //    Game::EntityCreateInfo info;
    //    info.immediate = false;
    //    info.templateId = Game::GetTemplateId("StaticGroundPlane/dev_ground_plane"_atm);
    //    Game::CreateEntity(info);
    //}
    //{
    //    Game::EntityCreateInfo info;
    //    info.immediate = true;
    //    info.templateId = Game::GetTemplateId("StaticEnvironment/knob_metallic"_atm);
    //    Game::Entity entity = Game::CreateEntity(info);
    //    Game::SetProperty(entity, Game::GetPropertyId("WorldTransform"_atm), Math::scaling(5, 5, 5) * Math::translation({ 5, 0, 5 }));
    //}
    //{
    //    Game::EntityCreateInfo info;
    //    info.immediate = true;
    //    info.templateId = Game::GetTemplateId("StaticEnvironment/knob_plastic"_atm);
    //    Game::Entity entity = Game::CreateEntity(info);
    //    Game::SetProperty(entity, Game::GetPropertyId("WorldTransform"_atm), Math::scaling(5, 5, 5) * Math::translation({ 5, 0, 7 }));
    //}
    //{
    //    Game::EntityCreateInfo info;
    //    info.immediate = true;
    //    info.templateId = Game::GetTemplateId("StaticEnvironment/knob_reflective"_atm);
    //    Game::Entity entity = Game::CreateEntity(info);
    //    Game::SetProperty(entity, Game::GetPropertyId("WorldTransform"_atm), Math::scaling(5, 5, 5) * Math::translation({ 5, 0, 9 }));
    //}
    //
    //for (int i = 0; i < 5; i++)
    //{
    //    Game::EntityCreateInfo info;
    //    info.immediate = true;
    //    info.templateId = Game::GetTemplateId("PhysicsEntity/placeholder_box"_atm);
    //    Game::Entity entity = Game::CreateEntity(info);
    //    Game::SetProperty(entity, Game::GetPropertyId("WorldTransform"_atm), Math::rotationyawpitchroll(0.01f, 0.01f, 0.01f) * Math::translation({ 2, 5.0f + ((float)i * 1.01f), 0 }));
    //}
    //

    Graphics::GraphicsEntityId terrain = Graphics::CreateEntity();
    Terrain::TerrainContext::RegisterEntity(terrain);

    Terrain::TerrainContext::SetupTerrain(terrain,
        "tex:terrain/dirt_aerial_02_disp_8k_PNG_BC4_1.dds",
        "tex:system/black.dds",
        "tex:terrain/dirt_aerial_02_diff_2k.dds");

    Terrain::BiomeSetupSettings biomeSettings =
    {
        0.5f, 900.0f, 64.0f
    };
    Terrain::TerrainContext::CreateBiome(biomeSettings,
        {
            "tex:terrain/brown_mud_leaves_01_diff_2k_PNG_BC7_1.dds",
            "tex:terrain/brown_mud_leaves_01_nor_2k_PNG_BC5_1.dds",
            "tex:terrain/brown_mud_leaves_01_material_2k_PNG_BC7_1.dds"
        },
        {
            "tex:terrain/dirt_aerial_02_diff_2k_PNG_BC7_1.dds",
            "tex:terrain/dirt_aerial_02_nor_2k_PNG_BC5_1.dds",
            "tex:terrain/dirt_aerial_02_material_2k_PNG_BC7_1.dds"
        },
        {
            "tex:terrain/snow_02_albedo_2k_PNG_BC7_1.dds",
            "tex:terrain/snow_02_nor_2k_PNG_BC5_1.dds",
            "tex:terrain/snow_02_material_2k_PNG_BC7_1.dds"
        },
        {
            "tex:terrain/rock_ground_02_albedo_2k_PNG_BC7_1.dds",
            "tex:terrain/rock_ground_02_nor_2k_PNG_BC5_1.dds",
            "tex:terrain/rock_ground_02_material_2k_PNG_BC7_1.dds"
        },
        "tex:system/white.dds"
    );

    float xOffset = -200;
    float zOffset = -200;
    float yOffset = 510;

    for (size_t i = 0; i < 600; i++)
    {
        Game::EntityCreateInfo info;
        info.immediate = true;
        info.templateId = Game::GetTemplateId("MovingEntity/cube"_atm);
        Game::Entity entity = Game::CreateEntity(Game::GetWorld(WORLD_DEFAULT), info);
        Game::SetProperty(Game::GetWorld(WORLD_DEFAULT), entity, Game::GetPropertyId("WorldTransform"_atm), Math::translation({ 0, yOffset + 0.5f, 0 }));
    }

    {
        auto files = IO::IoServer::Instance()->ListFiles("mdl:city", "*", true);
        
        int numFiles = files.size();
        int numWidth = 32;

        auto gameWorld = Game::GetWorld(WORLD_DEFAULT);
        Game::EntityCreateInfo info;
        info.immediate = true;
        info.templateId = Game::GetTemplateId("StaticEnvironment/general"_atm);

        for (int x = 0; x < numWidth; x++)
        {
            for (int y = 0; y < numWidth; y++)
            {
                uint fileIndex = Util::FastRandom() % (numFiles + 3);
                if (fileIndex >= numFiles)
                    continue;

                float rotation = (Util::FastRandom() % 4) * (N_PI / 2.0f);
                Game::Entity entity = Game::CreateEntity(gameWorld, info);
                Game::SetProperty<Math::mat4>(gameWorld, entity, Game::WorldTransform::ID(),
                    Math::scaling(10.0f) *
                    Math::rotationy(rotation) * 
                    Math::translation({ xOffset + (float)x * 13.0f, yOffset, zOffset + (float)y * 13.0f })
                );
                Game::SetProperty<Util::StringAtom>(gameWorld, entity, GraphicsFeature::ModelResource::ID(), files[fileIndex]);
            }
        }
    }

    GraphicsFeature::GraphicsFeatureUnit::Instance()->AddRenderUICallback([]()
    {
        
    });
}

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

    //static Game::Entity entities[4024];
    //static int frameIndex = 0;
    //if (frameIndex % 4 == 0)
    //{
    //    for (size_t i = 0; i < 4024; i++)
    //    {
    //        Game::EntityCreateInfo info;
    //        info.immediate = true;
    //        info.templateId = Game::GetTemplateId("MovingEntity/cube"_atm);
    //        entities[i] = Game::CreateEntity(info);
    //        Game::SetProperty(entities[i], Game::GetPropertyId("WorldTransform"_atm), Math::translation({ 0, 0.5f, 0 }));
    //    }
    //}
    //else if (frameIndex % 2 == 0)
    //{
    //    for (size_t i = 0; i < 4024; i++)
    //    {
    //        Game::DeleteEntity(entities[i]);
    //    }
    //}
    //frameIndex++;
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
