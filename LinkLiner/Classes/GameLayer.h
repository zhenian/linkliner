//
//  GameLayer.h
//  LinkLinker
//
//  Created by jun on 13-8-20.
//
//

#ifndef __LinkLinker__GameLayer__
#define __LinkLinker__GameLayer__

#include "cocos2d.h"
class MapNode;

int compare(const void* a, const void* b);
class Game : public cocos2d::CCLayer
{
public:
    Game();
	~Game();
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    void menuCloseCallback(CCObject* pSender);
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void registerWithTouchDispatcher();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(Game);
private:
    cocos2d::CCPoint prePoint;
	int countCleared;
    cocos2d::CCArray *arrayMap;
	int counter;
private:
    
    void initSound();
    void initData();
    void initView();
    cocos2d::CCString* imageFilename(int);
    
    void exitGame(CCObject* pSender);
    cocos2d::CCPoint pointOfView(cocos2d::CCPoint point);
    void showWin();
    void clearNode(cocos2d::CCPoint point);
    int indexFromPoint(cocos2d::CCPoint point);
    bool isSamePoints(cocos2d::CCPoint p1, cocos2d::CCPoint p2);
    bool isValiableNode(cocos2d::CCPoint point);
    bool isEmptyNode(cocos2d::CCPoint point);
    bool canClearTwo(cocos2d::CCPoint pointpre, cocos2d::CCPoint pointcurrent);
    bool match_direct(cocos2d::CCPoint a, cocos2d::CCPoint b);
    bool match_one_corner(cocos2d::CCPoint a, cocos2d::CCPoint b);
    bool match_two_corner(cocos2d::CCPoint a, cocos2d::CCPoint b);
    bool match(cocos2d::CCPoint a,cocos2d::CCPoint b);
};


class MapNode : public cocos2d::CCObject
{
public:
    int order;
    int imgid;
};

#endif /* defined(__LinkLinker__GameLayer__) */