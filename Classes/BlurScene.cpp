
#include "BlurScene.h"
#include "renderer/backend/ProgramState.h"
#include "renderer/backend/Device.h"

USING_NS_CC;

namespace rtv {

const char* positionTextureColor_vert = R"(
attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

uniform mat4 u_MVPMatrix;

#ifdef GL_ES
varying lowp vec4 v_fragmentColor;
varying mediump vec2 v_texCoord;
#else
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
#endif

void main()
{
    gl_Position = u_MVPMatrix * a_position;
    v_fragmentColor = a_color;
    v_texCoord = a_texCoord;
}
)";

const char* positionTextureColor_frag = R"(
#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform sampler2D u_texture;

uniform float u_blurFactor;

const float pointRange = 10.0f;
const float jump = 2.0f;

void main()
{
    vec4 color = vec4(0, 0, 0, 0);
    vec2 point;
    int count = 0;

    for(float u = -pointRange; u < pointRange ; u+=jump) {
        for(float v = -pointRange ; v < pointRange ; v+=jump) {
            point.x = v_texCoord.x  + u * u_blurFactor;
            point.y = v_texCoord.y  + v * u_blurFactor;

            // If the point is within the range[0, 1]
            if (point.y >= 0.0f && point.x >= 0.0f &&
                point.y <= 1.0f && point.x <= 1.0f ) {
                ++count;
                color += texture2D(u_texture, point.xy);
            }

        }
    }

    color = color / float(count);

    gl_FragColor = v_fragmentColor * color;
}
)";

Scene* BlurScene::createScene() {
    return BlurScene::create();
}

bool BlurScene::init() {
    if (!ExperimentScene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto sprite = Sprite::create("HelloWorld.png");
    auto program = backend::Device::getInstance()->newProgram(positionTextureColor_vert,
                                                              positionTextureColor_frag);
    auto programState = new backend::ProgramState(program);
    sprite->setProgramState(programState);
    auto uniformLocation = programState->getUniformLocation("u_blurFactor");
    backend::ProgramState::UniformCallback callback = [this](backend::ProgramState *programState,
                                      const backend::UniformLocation &location) {
  
        programState->setUniform(location, &mBlurFactor, sizeof(float));
    };
    
    programState->setCallbackUniform(uniformLocation, callback);

    sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    sprite->setContentSize(sprite->getContentSize()*2);
    
    float x = visibleSize.width/2 - sprite->getContentSize().width/2;
    float y = visibleSize.height/2 - sprite->getContentSize().height/2;
    sprite->setPosition(Vec2(x, y));
    
    this->addChild(sprite);
    
    scheduleUpdate();

    return true;
}

void BlurScene::update(float delta) {
    Scene::update(delta);
    mBlurFactor += mBlurFactorDelta;
    if (std::abs(mBlurFactor) > mBlurFactorMax) {
        mBlurFactorDelta = mBlurFactorDelta * -1;
    }
}


}
