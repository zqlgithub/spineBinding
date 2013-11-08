//
//  TestCObj.cpp
//  JSTest
//
//  Created by zql on 13-11-7.
//
//

#include "TestCObj.h"

namespace ls
{
    bool Leafsoar::init(){
        bool bRef = false;
        do {
            cocos2d::CCLog("leafsoar init ...");
            bRef = true;
        } while (0);
        return bRef;
    }
    
    void Leafsoar::functionTest(){
        cocos2d::CCLog("function Test");
    }
    
    
}