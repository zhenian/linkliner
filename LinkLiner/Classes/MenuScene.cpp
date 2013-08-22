//
//  MenuScene.cpp
//  LinkLinker
//
//  Created by jun on 13-8-11.
//
//

#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "GameLayer.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* Menu::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Menu *layer = Menu::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

Menu::~Menu()
{

	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

Menu::Menu()
:isEnter(false)
{
}

// on "init" you need to initialize your instance
bool Menu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSprite* pSprite = CCSprite::create("bg.png");
    // position the sprite on the center of the screen
    pSprite->setPosition( ccp(size.width/2, size.height/2) );
    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
    CCLabelTTF* label = CCLabelTTF::create("连连看", "Marker Felt", 48);
	label->setPosition(  ccp( size.width /2 , size.height/2 ) );
    this->addChild(label, 1);
	
	CCSprite* button = CCSprite::create("play.png");
    button->setPosition(  ccp(size.width/2, size.height/2 - 80) );
    this->addChild(button, 2);
	this->setTouchEnabled(true);
    
    return true;
}

bool Menu::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(!this->isEnter){
        this->scheduleOnce(schedule_selector(Menu::playGame), 0);
        this->isEnter = true;
    }
    return true;
}
void Menu::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
}

void Menu::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, true);
}

void Menu::playGame(CCObject* pSender)
{
    CCScene *pScene = Game::scene();
    CCTransitionFade* transitionScene = CCTransitionFade::create(1.0, pScene,ccWHITE);
    CCDirector::sharedDirector()->replaceScene(transitionScene);
}

void Menu::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}





















