#ifndef BlurScene_H
#define BlurScene_H

#include "ExperimentScene.h"

namespace rtv {

class BlurScene: public ExperimentScene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;
        
    CREATE_FUNC(BlurScene);
    
    virtual void update(float delta) override;

    float mBlurFactor{ 0.0f };
    float mBlurFactorMax = 0.005f;
    float mBlurFactorDelta = 0.00005;
};

}

#endif

