//
//  GameLayer.cpp
//  LinkLinker
//
//  Created by jun on 13-8-20.
//
//

#include "GameLayer.h"
#include "SimpleAudioEngine.h"
#include "MenuScene.h"

#define TAG_START_SPRITE                    100
#define TAG_LABEL_CONNER                    501
#define TAG_LABEL_NUMBER                    502
#define OFFSET_X							-40
#define OFFSET_Y							60
#define SIZE_W								40
#define SIZE_H								40
#define TOTAL_X								10
#define TOTAL_Y								10
#define TOTAL_IMG							16
#define MAX_CLEARED                         24


static int imgMap[64] = {
    1 , 1 , 2 , 2 , 3 , 3 , 4 , 4 ,
    5 , 5 , 5 , 5 , 6 , 6 , 0 , 0 ,
    7 , 7 , 7 , 7 , 8 , 8 , 0 , 0 ,
    9 , 9 , 9 , 9 , 10, 10, 10, 10,
    11, 11, 11, 11, 12, 12, 12, 12,
    13, 13, 13, 13, 14, 14, 14, 14,
	15, 15, 16, 16, 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
};

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* Game::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Game *layer = Game::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


Game::~Game()
{
	if (arrayMap)
	{
		arrayMap->release();
		arrayMap = NULL;
	}
    
    
	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

Game::Game()
:arrayMap(NULL)
,prePoint(CCPointZero)
,counter(0)
,countCleared(0)
{
}

// on "init" you need to initialize your instance
bool Game::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    this->initSound();
    this->initData();
    this->initView();
     
    return true;
}

void Game::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void Game::initSound()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.3f);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("back2.mp3", true);
}

void Game::initData()
{
	prePoint = CCPointMake(-1, -1);
	countCleared = 0;
	counter = 0;
	cocos2d::CCArray *array = cocos2d::CCArray::create();
	srandom((unsigned int)time(NULL));
	for (int i = 0; i < (TOTAL_X - 2) * (TOTAL_Y - 2); ++i) {
		MapNode *mapnode = new MapNode();
        mapnode->autorelease();
		mapnode->order = (int)(CCRANDOM_0_1() * INT_MAX) % (int)(CCRANDOM_0_1() * INT_MAX);
		mapnode->imgid = imgMap[i];
        array->addObject(mapnode);
	}
    
    qsort(array->data->arr, array->data->num, sizeof(long), compare);
    
	this->arrayMap = new cocos2d::CCArray;
	
	for (int x = 0; x < TOTAL_X; ++x) {
		for (int y = 0; y < TOTAL_Y; ++y) {
			if (x == 0 || x == (TOTAL_X - 1) || y == 0 || (y == TOTAL_Y - 1)) {
				MapNode *mapnode = new MapNode();
                mapnode->autorelease();
				mapnode->order = 0;
				mapnode->imgid = 0;
                this->arrayMap->addObject(mapnode);
			} else {
				int i = (y - 1) * (TOTAL_Y - 2) + x - 1;
                this->arrayMap->addObject(array->objectAtIndex(i));
			}
		}
	}
}


void Game::initView()
{
	this->setTouchEnabled(true);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
	CCSprite *background = CCSprite::create("bg.png");
    background->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(background);
    ;
	for (int y = 0; y < TOTAL_Y; ++y) {
		for (int x = 0; x < TOTAL_X; ++x) {
			int index = y * TOTAL_Y + x;
            if(this->imageFilename(index)){
                CCSprite *sprite = CCSprite::create(this->imageFilename(index)->getCString());
                sprite->setScale(0.5f);
                sprite->setPosition(ccp(OFFSET_X + (SIZE_W / 2) + SIZE_W * x, OFFSET_Y + (SIZE_H / 2) + SIZE_H * y));
                this->addChild(sprite, 0, (TAG_START_SPRITE + index));
            }
		}
	}
	
	CCSprite *button = CCSprite::create("play.png");
    button->setPosition(ccp(size.width - 55, 25));
	this->addChild(button);
	
	CCLabelTTF *label = CCLabelTTF::create("进度:0%", "Arial", 20.0);
	label->setPosition(ccp(50, 15));
    this->addChild(label,0,TAG_LABEL_CONNER);
	
	CCLabelTTF *labelnum1 = CCLabelTTF::create("1", "Marker Felt", 64.0);
    labelnum1->setPosition(ccp( size.width /2 , size.height/2 ));
	this->addChild(labelnum1);
    
	CCLabelTTF *labelnum2 = CCLabelTTF::create("2", "Marker Felt", 64.0);
    labelnum2->setPosition(ccp( size.width /2 , size.height/2 ));
    labelnum2->setVisible(false);
	this->addChild(labelnum2);
    
    CCLabelTTF *labelnum3 = CCLabelTTF::create("3", "Marker Felt", 64.0);
    labelnum3->setPosition(ccp( size.width /2 , size.height/2 ));
    labelnum3->setVisible(false);
	this->addChild(labelnum3);
    
    CCLabelTTF *labelnum4 = CCLabelTTF::create("Go", "Marker Felt", 64.0);
    labelnum4->setPosition(ccp( size.width /2 , size.height/2 ));
    labelnum4->setVisible(false);
	this->addChild(labelnum4);
    
    void* ac1a = labelnum1->runAction(cocos2d::CCShow::create());
	void* ac1b = labelnum1->runAction(cocos2d::CCScaleBy::create(0.5, 2.0));
    void* ac1c = labelnum1->runAction(cocos2d::CCHide::create());
    
    void* ac2a = labelnum2->runAction(cocos2d::CCShow::create());
	void* ac2b = labelnum2->runAction(cocos2d::CCScaleBy::create(0.5, 2.0));
    void* ac2c = labelnum2->runAction(cocos2d::CCHide::create());
    
    void* ac3a = labelnum3->runAction(cocos2d::CCShow::create());
	void* ac3b = labelnum3->runAction(cocos2d::CCScaleBy::create(0.5, 2.0));
    void* ac3c = labelnum3->runAction(cocos2d::CCHide::create());
    
    
    void* ac4a = labelnum4->runAction(cocos2d::CCShow::create());
	void* ac4b = labelnum4->runAction(cocos2d::CCScaleBy::create(0.5, 2.0));
    void* ac4c = labelnum4->runAction(cocos2d::CCHide::create());
    
    labelnum1->runAction(cocos2d::CCSequence::create(cocos2d::CCDelayTime::create(0.5),ac1a,ac1b,ac1c,NULL));
    labelnum2->runAction(cocos2d::CCSequence::create(cocos2d::CCDelayTime::create(0.5),ac2a,ac2b,ac2c,NULL));
    labelnum3->runAction(cocos2d::CCSequence::create(cocos2d::CCDelayTime::create(0.5),ac3a,ac3b,ac3c,NULL));
    labelnum4->runAction(cocos2d::CCSequence::create(cocos2d::CCDelayTime::create(0.5),ac4a,ac4b,ac4c,NULL));
     
}

cocos2d::CCString* Game::imageFilename(int index)
{
    cocos2d::CCObject *o = this->arrayMap->objectAtIndex(index);
    int n = ((MapNode *)o)->imgid;
    if (n >= 1 && n <= TOTAL_IMG){
        return cocos2d::CCString::createWithFormat("%d.png",n);
    }else{
        return NULL;
    }
}

cocos2d::CCPoint Game::pointOfView(cocos2d::CCPoint point)
{
	int x = -1;
	int y = -1;
	if (point.x > OFFSET_X && point.x < TOTAL_X * SIZE_W + OFFSET_X)
	{
        x = (point.x - OFFSET_X) / SIZE_W;
    }
    if (point.y > 480 - OFFSET_Y - TOTAL_Y * SIZE_H && point.y < 480 - OFFSET_Y)
    {
        y = (480 - point.y - OFFSET_Y) / SIZE_H;
    }
    return CCPointMake(x, y);
}


void Game::showWin()
{
	CCLabelTTF *label = CCLabelTTF::create("恭喜过关！", "Arial", 36.0);
    
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	label->setPosition(ccp(s.width/2, s.height/2));
    this->addChild(label);
    this->scheduleOnce(schedule_selector(Game::exitGame), 2.0);
}

/*
void makeTransition(cocos2d::CCTime dt)
{
	[[CCDirector sharedDirector] replaceScene:[CCTransitionFade transitionWithDuration:1.0 scene:[IntroLayer scene] withColor:ccWHITE]];
}
 */
int Game::indexFromPoint(cocos2d::CCPoint point)
{
	return point.y * TOTAL_Y + point.x;
}

void Game::clearNode(cocos2d::CCPoint point)
{
	int index = this->indexFromPoint(point);
	MapNode *mapnode = (MapNode *)(this->arrayMap->objectAtIndex(index));
	mapnode->imgid = 0;
}

bool Game::isSamePoints(cocos2d::CCPoint p1, cocos2d::CCPoint p2)
{
	return (p1.x == p2.x && p1.y == p2.y);
}

bool Game::isValiableNode(cocos2d::CCPoint point)
{
	return point.x >= 0 && point.x < TOTAL_X && point.y >= 0 && point.y < TOTAL_Y;
}

bool Game::isEmptyNode(cocos2d::CCPoint point)
{
	int index = this->indexFromPoint(point);
	MapNode *mapnode = (MapNode *)(this->arrayMap->objectAtIndex(index));
	return (mapnode->imgid == 0);
}

#pragma mark link

bool Game::match_direct(cocos2d::CCPoint a, cocos2d::CCPoint b)
{
	if (!(a.x == b.x || a.y == b.y)) {
		return false;
	}
	
	int i;
	bool match_x = false;
	if(a.x == b.x) {
		match_x = true;
		if(a.y > b.y) {
			for(i = a.y - 1; i > b.y; --i) {
				CCPoint point = CCPointMake(a.x, i);
                if(!this->isValiableNode(point) || !this->isEmptyNode(point)){
					match_x = false;
				}
			}
		}
		if(b.y > a.y) {
			for(i = b.y - 1; i > a.y; --i) {
				CCPoint point = CCPointMake(a.x, i);
				if(!this->isValiableNode(point) || !this->isEmptyNode(point)) {
					match_x = false;
				}
			}
		}
	}
	
	bool match_y = false;
	if(a.y == b.y) {
		match_y = true;
		if(a.x > b.x) {
			for(i = a.x - 1; i > b.x; --i) {
				CCPoint point = CCPointMake(i, a.y);
				if(!this->isValiableNode(point) || !this->isEmptyNode(point)) {
					match_y = false;
				}
			}
		}
		if(b.x > a.x) {
			for(i = b.x - 1; i > a.x; --i) {
				CCPoint point = CCPointMake(i, a.y);
				if(!this->isValiableNode(point) || !this->isEmptyNode(point)) {
					match_y = false;
				}
			}
		}
	}
	
	return match_x || match_y;
}

bool Game::match_one_corner(cocos2d::CCPoint a, cocos2d::CCPoint b)
{
	CCPoint point = CCPointMake(b.x, a.y);

    if( this->isValiableNode(point) && this->isEmptyNode(point) && this->match_direct(a, point) && this->match_direct(b, point)){
        return true;
    }
	
	point = CCPointMake(a.x, b.y);
	if( this->isValiableNode(point) && this->isEmptyNode(point) && this->match_direct(a, point) && this->match_direct(b, point)){
        return true;
    }
	
	return false;
}

bool Game::match_two_corner(cocos2d::CCPoint a, cocos2d::CCPoint b)
{
	for(int i = a.x - 1; i >= 0; --i) {
		CCPoint point = CCPointMake(i, a.y);        
        if (!this->isValiableNode(point) || !this->isEmptyNode(point)) {
            break;
        } else {
            if (this->match_one_corner(point, b)) {
                return true;
            }
        }
	}
	
	for(int i = a.x + 1; i < TOTAL_X; ++i) {
		CCPoint point = CCPointMake(i, a.y);        
        if (!this->isValiableNode(point) || !this->isEmptyNode(point)) {
            break;
        } else {
            if (this->match_one_corner(point, b)) {
                return true;
            }
        }
	}
	
	for(int i = a.y - 1; i >= 0; --i) {
		CCPoint point = CCPointMake(a.x ,i);
		if (!this->isValiableNode(point) || !this->isEmptyNode(point)) {
            break;
        } else {
            if (this->match_one_corner(point, b)) {
                return true;
            }
        }
	}
	
	for(int i = a.y + 1; i < TOTAL_Y; ++i) {
		CCPoint point = CCPointMake(a.x ,i);
		if (!this->isValiableNode(point) || !this->isEmptyNode(point)) {
            break;
        } else {
            if (this->match_one_corner(point, b)) {
                return true;
            }
        }
	}
	
	return false;
}

bool Game::match(cocos2d::CCPoint a,cocos2d::CCPoint b)
{
    if (this->match_direct(a, b)) {
        return true;
    }
    if (this->match_one_corner(a, b)) {
        return true;
    }
    if (this->match_two_corner(a, b)) {
        return true;
    }
	
	return false;
}


bool Game::canClearTwo(cocos2d::CCPoint pointpre, cocos2d::CCPoint pointcurrent)
{
	bool bMatch = false;
	int pre = this->indexFromPoint(pointpre);
	int current = this->indexFromPoint(pointcurrent);
	int p = ((MapNode *)(this->arrayMap->objectAtIndex(pre)))->imgid;
	int c = ((MapNode *)(this->arrayMap->objectAtIndex(current)))->imgid;
	
    if (p == c && this->match(pointcurrent, pointpre)) {
		bMatch = true;
	}
	
	return bMatch;
}


bool Game::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint ptouch = pTouch->getLocationInView();
	if (ptouch.x > 250 && ptouch.y > 420) {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
        this->schedule(schedule_selector(Game::exitGame), 0);
        return true;
	}
	
	CCPoint pointcurrent = this->pointOfView(pTouch->getLocationInView());
    
    if (this->isValiableNode(pointcurrent) == false) {
        return false;
    }
	
	if (this->isEmptyNode(pointcurrent)) {
		return false;
	}
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("choose.wav");
    
	if (this->isSamePoints(pointcurrent, prePoint)) {
		return false;
	}
	
	CCSprite *spritecurrent = (CCSprite *)this->getChildByTag(TAG_START_SPRITE + this->indexFromPoint(pointcurrent));
    
	spritecurrent->setScale(0.6);
	
	if (this->isValiableNode(prePoint)) {
		CCSprite *spritepre = (CCSprite *)this->getChildByTag(TAG_START_SPRITE + this->indexFromPoint(prePoint));
		if (this->canClearTwo(prePoint, pointcurrent)) {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("disappear1.wav");
            this->clearNode(prePoint);
            this->clearNode(pointcurrent);
            spritepre->setVisible(false);
            spritecurrent->setVisible(false);
			if (++countCleared >= MAX_CLEARED) {
                CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("win.mp3");
                this->showWin();
			}
			CCLabelTTF *label = (CCLabelTTF *)this->getChildByTag(TAG_LABEL_CONNER);
            cocos2d::CCString *str = CCString::createWithFormat("进度:%d%%", (int)(countCleared * 100 / MAX_CLEARED));
            label->setString(str->getCString());
		} else {
			spritepre->setScale(0.5);
		}
	}
	
	prePoint = pointcurrent;
    return true;
}

void Game::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, true);
}

void Game::exitGame(CCObject* pSender)
{
    CCScene *pScene = Menu::scene();
    CCTransitionFade* transitionScene = CCTransitionFade::create(1.0, pScene,ccWHITE);
    CCDirector::sharedDirector()->replaceScene(transitionScene);
}



int compare(const void* a, const void* b){
    MapNode *self = (MapNode *)a;
    MapNode *other = (MapNode *)b;
	if (self->order > other->order) {
		return 1;
	} else if (self->order == other->order) {
		return 0;
	} else {
		return -1;
	}
}





