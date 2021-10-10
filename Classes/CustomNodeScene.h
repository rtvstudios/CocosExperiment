#ifndef CustomNodeScene_H
#define CustomNodeScene_H

#include "ExperimentScene.h"

namespace rtv {

class CustomNodeScene: public ExperimentScene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;
        
    CREATE_FUNC(CustomNodeScene);
};

}

#endif

