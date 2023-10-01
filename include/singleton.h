//
// Created by 王亚宁 on 2023/9/28.
//

#ifndef FEITENG_SINGLETON_H
#define FEITENG_SINGLETON_H

#include <memory>

namespace Feiteng {
    // 单例模式获取实例
    template<class T, class X = void, int N = 0>
    class Singleton{
    public:
        static T* GetInstance(){
            static T v;
            return &v;
        }
    };

    // 单例模式获取实例的智能指针
    template<class T, class X = void, int N = 0>
    class SingletonPtr{
    public:
        static std::shared_ptr<T> GetInstance(){
            static std::shared_ptr<T> v(new T);
            return v;
        }
    };
}

#endif //FEITENG_SINGLETON_H
