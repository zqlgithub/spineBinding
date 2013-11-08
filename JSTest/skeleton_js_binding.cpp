//
//  skeleton_js_binding.cpp
//  JSTest
//
//  Created by zql on 13-11-8.
//
//

#include "skeleton_js_binding.h"
#include <spine-cocos2dx.h>
#include "js_manual_conversions.h"
#include "cocos2d_specifics.hpp"

JSClass* jsb_skeleton_class;
JSObject* jsb_skeleton_prototype;

JSBool js_skeleton_setAnimation(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = NULL;
    cocos2d::extension::CCSkeletonAnimation* cobj = NULL;
    obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    // 获取 js 绑定的实际对象 通过 proxy->ptr
    cobj = (cocos2d::extension::CCSkeletonAnimation*)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
    if (argc == 2) {
        jsval *argvp = JS_ARGV(cx,vp);
        JSBool ok = JS_TRUE;
        const char* arg1; JSBool arg2;
        
        ok &= jsval_to_charptr(cx, *argvp++, &arg1);
        ok &= JS_ValueToBoolean(cx, *argvp++, &arg2);
        JSB_PRECONDITION(ok, "Error processing arguments");
        
        cobj->setAnimation(arg1, arg2);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return ok;
    }
    
    JS_ReportError(cx, "wrong number of arguments");
    return JS_FALSE;
}

void js_skeleton_finalize(JSFreeOp *fop, JSObject *obj) {
    // 析构函数实现，如果在构造函数做了什么，如开辟内存空间，那么需要在这里做些收尾工作
    //    CCLOGINFO("jsbindings: finalizing JS object %p (LsLeafsoar)", obj);
}

JSBool js_skeleton_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    cocos2d::CCLog("js ls lsleafsoar create ..");
    if (argc == 2) {
        jsval *argvp = JS_ARGV(cx,vp);
        JSBool ok = JS_TRUE;
        const char* arg1; const char* arg2;
        
        ok &= jsval_to_charptr(cx, *argvp++, &arg1);
        ok &= jsval_to_charptr(cx, *argvp++, &arg2);
        
        cocos2d::extension::CCSkeletonAnimation* ret =
            cocos2d::extension::CCSkeletonAnimation::createWithFile(arg1, arg2);
        jsval jsret;
        do {
            if (ret) {
                js_proxy_t *proxy = js_get_or_create_proxy<cocos2d::extension::CCSkeletonAnimation>(cx, ret);
                jsret = OBJECT_TO_JSVAL(proxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return ok;
    }
    JS_ReportError(cx, "wrong number of arguments");
    return JS_FALSE;
}

JSBool js_skeleton_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    cocos2d::CCLog("js skeletonanimation constructor ..");
    if (argc == 2) {
        jsval *argvp = JS_ARGV(cx,vp);
        JSBool ok = JS_TRUE;
        const char* arg1; const char* arg2;
        
        ok &= jsval_to_charptr(cx, *argvp++, &arg1);
        ok &= jsval_to_charptr(cx, *argvp++, &arg2);
        JSB_PRECONDITION(ok, "Error processing arguments");
        
        // 调用 C++ 构造函数
        cocos2d::extension::CCSkeletonAnimation* cobj =
            new cocos2d::extension::CCSkeletonAnimation(arg1, arg2);
        cocos2d::CCObject* _ccobj = dynamic_cast<cocos2d::CCObject*>(cobj);
        // 默认使用原有的内存管理方式
        if (_ccobj){
            _ccobj->autorelease();
        }
        TypeTest<cocos2d::extension::CCSkeletonAnimation> t;
        js_type_class_t *typeClass;
        uint32_t typeId = t.s_id();
        HASH_FIND_INT(_js_global_type_ht, &typeId, typeClass);
        assert(typeClass);
        JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
        JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(obj));
        // 构造 js 端对象，将 cobj 实际对象存入
        js_proxy_t* p = jsb_new_proxy(cobj, obj);
        JS_AddNamedObjectRoot(cx, &p->obj, "cocos2d::extension::CCSkeletonAnimation");
        
        return JS_TRUE;
    }
    
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 0);
    return JS_FALSE;
}

void js_register_skeleton(JSContext* cx, JSObject* global)
{
    jsb_skeleton_class = (JSClass *)calloc(1, sizeof(JSClass));
    
    jsb_skeleton_class->name = "skeletonAnim";
    jsb_skeleton_class->addProperty = JS_PropertyStub;
    jsb_skeleton_class->delProperty = JS_PropertyStub;
    jsb_skeleton_class->getProperty = JS_PropertyStub;
    jsb_skeleton_class->setProperty = JS_StrictPropertyStub;
    jsb_skeleton_class->enumerate = JS_EnumerateStub;
    jsb_skeleton_class->resolve = JS_ResolveStub;
    jsb_skeleton_class->convert = JS_ConvertStub;
    
    jsb_skeleton_class->finalize = js_skeleton_finalize;
    jsb_skeleton_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);
    
    static JSPropertySpec properties[] = {
        {0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
    };
    
    static JSFunctionSpec funcs[] = {
        JS_FN("setAnimation", js_skeleton_setAnimation, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };
    
    static JSFunctionSpec st_funcs[] = {
        JS_FN("create", js_skeleton_create, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };
    
    jsb_skeleton_prototype = JS_InitClass(
                                            cx, global,
                                            NULL, // parent proto
                                            jsb_skeleton_class,
                                            js_skeleton_constructor, 0, // 这里绑定的是构造函数的实现，也就是用 js new 操作符创建的对象
                                            properties,
                                            funcs,      // 函数绑定
                                            NULL, // no static properties
                                            st_funcs);      // 静态函数绑定
    
    JSBool found;
    JS_SetPropertyAttributes(cx, global, "Leafsoar", JSPROP_ENUMERATE | JSPROP_READONLY, &found);
    
    TypeTest<cocos2d::extension::CCSkeletonAnimation> t;
    js_type_class_t *p;
    uint32_t typeId = t.s_id();
    HASH_FIND_INT(_js_global_type_ht, &typeId, p);
    if (!p) {
        p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
        p->type = typeId;
        p->jsclass = jsb_skeleton_class;
        p->proto = jsb_skeleton_prototype;
        p->parentProto = NULL;
        HASH_ADD_INT(_js_global_type_ht, type, p);
    }
    
}

void register_all_spine(JSContext* cx, JSObject* obj) {
    jsval nsval;
    JSObject *ns;
    JS_GetProperty(cx, obj, "spine", &nsval);
    if (nsval == JSVAL_VOID) {
        ns = JS_NewObject(cx, NULL, NULL, NULL);
        nsval = OBJECT_TO_JSVAL(ns);
        JS_SetProperty(cx, obj, "spine", &nsval);
    } else {
        JS_ValueToObject(cx, nsval, &ns);
    }
    obj = ns;
    
    js_register_skeleton(cx, obj);
}