#ifndef DARKSTAR_APP_H
#define DARKSTAR_APP_H
//------------------------------------------------------------------------------
//  DarkStarApp.h
//------------------------------------------------------------------------------
#ifdef __APPLE__
#include <unistd.h>
#endif

#include "Pre.h"

#include "Core/Main.h"
#include "Core/Types.h"
#include "Core/Time/Clock.h"

#include "Gfx/Gfx.h"

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shaders.h"

#include "IO/IO.h"
#include "LocalFS/LocalFileSystem.h"

#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_wavstream.h"

#include "Camera.h"
#include "SceneObject.h"
#include "Renderizer.h"
#include "GameSettings.h"
#include "DebugTools.h"
#include "DebugDraw.h"
#include "UIAssets.h"


class DarkStarApp : public Oryol::App {
    
public:
    Oryol::AppState::Code OnRunning();
    Oryol::AppState::Code OnInit();
    Oryol::AppState::Code OnCleanup();
    
private:
    void handleInputDebug();
    void mainMenuUI();

    void nextCamera();
    
    glm::mat4 computeMVP(const glm::mat4& proj,
                         float rotX, float rotY, const glm::vec3& pos);
    
    Oryol::Id renderPass;
    Oryol::DrawState offscrDrawState;
    Oryol::DrawState mainDrawState;
    //OffscreenShader::params offscrVSParams;
    //MainShader::params mainVSParams;
    glm::mat4 view;
    glm::mat4 offscreenProj;
    glm::mat4 displayProj;
    float angleX = 0.0f;
    float angleY = 0.0f;
    
	GameSettings cfg;
    Oryol::GfxSetup gfxSetup;
    Oryol::PassAction passAction;
        
    // Debug mode stuff
	DebugTools* debugTools;
    Tapnik::Camera gameCamera;
    //Tapnik::Camera dbgCamera;
    Tapnik::DebugDrawRenderer *dbgDraw;
    int activeCameraIndex = 0;
    
    // Gameplay stuff
    Oryol::TimePoint startTimePoint;
    Oryol::TimePoint lastTimePoint;
    Oryol::Duration frameDtRaw;
    Oryol::Duration leftoverTime;

    // Game stuff
    // Note to self -- split these into another file if not messing with the main game loop structure much
    Tapnik::Scene *gameScene;
    void onSceneLoaded();
    void fixedUpdate( Oryol::Duration fixedDt );
    void dynamicUpdate( Oryol::Duration frameDt );
    void finalizeTransforms( glm::mat4 matViewProj );
    void draw();
    void interfaceScreens( Tapnik::UIAssets *uiAssets );

	Tapnik::Renderizer* renderizer;
    
    Tapnik::UIAssets *uiAssets;
    SoLoud::Soloud soloud;
    
    // TODO: wrap up all this 
    Oryol::Buffer sfxJumpData;
    SoLoud::Wav sfxJump;
    
    int musicPlaying = 0;
    Oryol::Buffer musicData;
    SoLoud::WavStream music;
};
#endif
