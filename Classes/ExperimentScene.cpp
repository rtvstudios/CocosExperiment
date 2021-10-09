
#include "ExperimentScene.h"
#include "CatalogScene.h"

USING_NS_CC;

namespace rtv {

Scene* ExperimentScene::createScene()
{
    return ExperimentScene::create();
}

bool ExperimentScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto closeItem = MenuItemImage::create("CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(ExperimentScene::menuBackCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float x = origin.x + visibleSize.width/2 - closeItem->getContentSize().width/2;
    float y = origin.y + visibleSize.height/2 - closeItem->getContentSize().height/2;
    closeItem->setPosition(Vec2(x,y));

    auto menu = Menu::create(closeItem, nullptr);
    this->addChild(menu, 1);

    return true;
}

void ExperimentScene::menuBackCallback(Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f,
                                                                 CatalogScene::create(),
                                                                 Color3B::WHITE));
}


}
