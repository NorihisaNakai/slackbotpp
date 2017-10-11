#ifndef SINGLETON_H
#define SINGLETON_H

#include <functional>
#include <mutex>

class singleton_finalizer{
public:
    static void add_finalizer( std::function<void()> );
    static void finalize();
};

template <typename _T>
class singleton final {
public:
    static _T& get(){
        std::call_once( initflag, create );
        return *instance;
    }

private:
    static void create(){
        instance = new _T;
        singleton_finalizer::add_finalizer( &singleton<_T>::destory );
    }

    static void destory() {
        delete instance;
        instance= nullptr;
    }
    static std::once_flag initflag;
    static _T* instance;
};

template <typename _T> std::once_flag singleton<_T>::initflag;
template <typename _T> _T* singleton<_T>::instance = nullptr;

#endif  //SINGLETON_H