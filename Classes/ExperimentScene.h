#ifndef ExperimentScene_H
#define ExperimentScene_H

#include "cocos2d.h"

namespace rtv {

class ExperimentScene: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuBackCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(ExperimentScene);
};

}

#endif

