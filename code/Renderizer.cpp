#include "Pre.h"
#include "Core/Main.h"
#include "Gfx/Gfx.h"

#include "shaders.h"
#include "shadow_shaders.h"

#include "SceneObject.h"
#include "Renderizer.h"

using namespace Oryol;
using namespace Tapnik;

Renderizer::Renderizer(  const Oryol::VertexLayout &meshLayout, Oryol::GfxSetup *gfxSetup )
{
    // Main render setup
    this->passAction.Color[0] = glm::vec4( 0.2, 0.2, 0.2, 1.0 );

    // Shadow Setup
    this->shadowShader = Gfx::CreateResource(ShadowShader::Setup());
    
    // TODO get from scene or something
    shadowCamera = {};

    shadowCamera.Setup(glm::vec3(0.0, 0.0, 25.0), glm::radians(45.0f),
                    shadowMapSize, shadowMapSize, 1.0f, 100.0f);
    
    // Doesn't quite work
//    shadowCamera.SetupShadow(glm::vec3(0.0, 0.0, 25.0), 20.0f, shadowMapSize, shadowMapSize, 1.0f, 100.0f );
    
    shadowCamera.Pos = glm::vec3( 11.22, -29.90, 34.06 );
    shadowCamera.Rot = glm::vec2( 0.28, 0.68 );
    
    // This applies the pos/rot
    shadowCamera.MoveRotate( glm::vec3(0.0f), glm::vec2(0.0f));


    TextureSetup shadowMapSetup = TextureSetup::RenderTarget2D(shadowMapSize, shadowMapSize, PixelFormat::RGBA8, PixelFormat::DEPTH);
    this->shadowMap = Gfx::CreateResource(shadowMapSetup);
    
    PassSetup shadowPassSetup = PassSetup::From(this->shadowMap,  // color target
                                                this->shadowMap); // depth target
    
    shadowPassSetup.DefaultAction = PassAction::Clear(glm::vec4(1.0f), 1.0f, 0);
    this->shadowPass = Gfx::CreateResource(shadowPassSetup);
    //this->ColorDrawState.FSTexture[0] = this->shadowMap;
    
    // create shadow pass pipeline state
    auto ps = PipelineSetup::FromLayoutAndShader( meshLayout,this->shadowShader);
    ps.DepthStencilState.DepthWriteEnabled = true;
    ps.DepthStencilState.DepthCmpFunc = CompareFunc::LessEqual;
    ps.RasterizerState.CullFaceEnabled = true;
    ps.RasterizerState.CullFace = Face::Code::Front;
    ps.RasterizerState.SampleCount = 1;
    ps.BlendState.ColorFormat = shadowMapSetup.ColorFormat;
    ps.BlendState.DepthFormat = shadowMapSetup.DepthFormat;
    this->shadowDrawState.Pipeline = Gfx::CreateResource(ps);
    
    // Setup the debug visualization
    auto quadSetup = MeshSetup::FullScreenQuad();
    this->shadowDebugDrawState.Mesh[0] = Gfx::CreateResource(quadSetup);
    Id shd = Gfx::CreateResource(DebugShadowShader::Setup());
    auto psShadowDebug = PipelineSetup::FromLayoutAndShader(quadSetup.Layout, shd);
    psShadowDebug.RasterizerState.SampleCount = gfxSetup->SampleCount;
    this->shadowDebugDrawState.Pipeline = Gfx::CreateResource(psShadowDebug);    
}

void Renderizer::renderScene( Tapnik::Scene *scene, Tapnik::UIAssets *uiAssets )
{
    // the shadow pass
    // this->shadowVSParams.mvp = this->lightProjView;
    if (scene) {
        Gfx::BeginPass( shadowPass );
        
        //    this->shapeRenderer.DrawShadows(this->shadowVSParams);
        scene->drawShadowPass( shadowDrawState);
        Gfx::EndPass();
    }
    
    // Start the main render pass
    Gfx::BeginPass(this->passAction);
    
    //    const ddVec3 boxColor  = { 0.0f, 0.8f, 0.8f };
    //    const ddVec3 boxCenter = { 0.0f, 0.0f, 0.0f };
    //    float boxSize = 1.0f;
    //    dd::box(boxCenter, boxColor, boxSize, boxSize, boxSize );
    //    dd::cross(boxCenter, 1.0f);
    
    if (scene) {
        scene->drawScene( shadowMap );
    }
    
}

void Renderizer::finishRender(Tapnik::UIAssets* uiAssets)
{
    // Draw the shadow debug
    if (debugDrawShadowMap) {
		float aspect = uiAssets->fbWidth / uiAssets->fbHeight;
        shadowDebugFSparams.size = glm::vec2( 0.5f, 0.5f * aspect );
        shadowDebugFSparams.offs = glm::vec2( -0.42, 0.42 );
        this->shadowDebugDrawState.FSTexture[ DebugShadowShader::tex ] = shadowMap;
        Gfx::ApplyDrawState(this->shadowDebugDrawState);    
        Gfx::ApplyUniformBlock(this->shadowDebugFSparams);
        Gfx::Draw();
    }
    
    Gfx::EndPass();
}
