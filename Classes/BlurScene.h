#ifndef BlurScene_H
#define BlurScene_H

#include "ExperimentScene.h"

namespace rtv {

class BlurScene: public ExperimentScene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
        
    CREATE_FUNC(BlurScene);
};

}

#endif

