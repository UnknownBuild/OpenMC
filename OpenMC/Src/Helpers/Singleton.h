#ifndef __SINGLETON_H__
#define __SINGLETON_H__

template <class T>
class Singleton final {
public:
    static T* GetInstance() {
        static T instance;
        return &instance;
    }

private:
    Singleton() {}
};

#endif
