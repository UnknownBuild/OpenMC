#pragma

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
