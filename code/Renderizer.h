#ifndef RENDERIZER_H
#define RENDERIZER_H

#include "Pre.h"
#include "Core/Main.h"
#include "Gfx/Gfx.h"

#include "UIAssets.h"
#include "DebugDraw.h"
#include "Ray.h"
#include "Camera.h"

#include "shadow_shaders.h"
#include "postproc.h"

namespace Tapnik
{    
    struct Renderizer {
        
        Renderizer( const Oryol::VertexLayout &meshLayout, Oryol::GfxSetup *gfxSetup );
        
        void renderScene( Tapnik::Scene *scene, Tapnik::UIAssets *uiAssets );
        void finishRender( Tapnik::UIAssets *uiAssets );
        
        // Main render stuff
        Oryol::PassAction passAction;
        
        // Shadow stuff
        const int shadowMapSize = 2048;
        Tapnik::Camera shadowCamera;
        Oryol::Id shadowMap;
        Oryol::Id shadowPass;
        Oryol::Id shadowShader;
        Oryol::DrawState shadowDrawState;
        bool debugDrawShadowMap = false;
        
        Oryol::DrawState shadowDebugDrawState;
        DebugShadowShader::vsParams shadowDebugFSparams;

		// Offscreen render pass (pre-color-corrected)
		Oryol::Id mainRenderTarget;
		Oryol::Id mainRenderPass;
		
		// Post draw (color grade, etc)
		Oryol::DrawState postProcDrawState;
		PostProcShader::vsParams postProcFSparams;

        
    };
} // namespace Tapnik
#endif
