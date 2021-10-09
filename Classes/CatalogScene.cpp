/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "CatalogScene.h"
#include "BlurScene.h"

USING_NS_CC;

namespace rtv {

Scene* CatalogScene::createScene()
{
    return CatalogScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool CatalogScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(CatalogScene::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width/2 - closeItem->getContentSize().width/2;
        float y = origin.y + visibleSize.height/2 - closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    Vector<MenuItem*> menuItems;
    menuItems.pushBack(closeItem);
    
    int CatelogCount = 1;
    
    static const Color3B MenuItemColor = Color3B::BLACK;
    
    Vec2 itemStartPos = Vec2::ZERO;
    float deltaHeight = 0;
    
    if (CatelogCount-- > 0) {
        auto *blurScene = MenuItemFont::create("Blur Scene", [](Ref *) {
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f,
                                                                         BlurScene::create(),
                                                                         Color3B::WHITE));
        });
        auto contentSize = blurScene->getContentSize();
        deltaHeight = contentSize.height * 1.25f;
        itemStartPos.set(0, (deltaHeight * (CatelogCount+1))/2);
        blurScene->setPosition(itemStartPos);
        menuItems.pushBack(blurScene);
        blurScene->setColor(MenuItemColor);
    }
    
    if (CatelogCount-- > 0) {
        auto *customMaterialScene = MenuItemFont::create("Material");
        itemStartPos.set(0, itemStartPos.y - deltaHeight);
        customMaterialScene->setPosition(itemStartPos);
        menuItems.pushBack(customMaterialScene);
        customMaterialScene->setColor(MenuItemColor);
    }

    auto menu = Menu::createWithArray(menuItems);
    this->addChild(menu, 1);

    return true;
}

void CatalogScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

}

