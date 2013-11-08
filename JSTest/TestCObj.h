//
//  TestCObj.h
//  JSTest
//
//  Created by zql on 13-11-7.
//
//

#ifndef __JSTest__TestCObj__
#define __JSTest__TestCObj__

#include <iostream>
#include "cocos2d.h"

namespace ls
{
    
    class Leafsoar: public cocos2d::CCObject
    {
    public:
        static cocos2d::CCScene* scene();
        virtual bool init();
        
        void functionTest();
        
        CREATE_FUNC(Leafsoar);
    };
}




#endif /* defined(__JSTest__TestCObj__) */
