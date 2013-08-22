//
//  MenuScene.h
//  LinkLinker
//
//  Created by jun on 13-8-11.
//
//

#ifndef __LinkLinker__MenuScene__
#define __LinkLinker__MenuScene__

#include "cocos2d.h"

class Menu : public cocos2d::CCLayer
{
public:
    Menu();
	~Menu();
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(Menu);
    
    void playGame(CCObject* pSender);
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void registerWithTouchDispatcher();
private:
    bool isEnter;
};


#endif /* defined(__LinkLinker__MenuScene__) */
