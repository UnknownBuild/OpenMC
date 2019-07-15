# 个人报告

## 实现功能

### 游戏框架

#### Window类与依赖注入

OpenMC将所有涉及到窗体和窗体相关外部依赖库相关的逻辑封装在窗体类`Window`中，除了OpenGL相关的部分函数在各个地方都频繁且广泛被调用，因此不考虑封装外，GLAD、GLFW、ImGui等相关库均封装起来。当然，部分涉及到渲染的逻辑十分零散，封装的意义不大，于是选择直接调用。

```c++
// 初始化游戏窗体
Window* window = Singleton<Window>::GetInstance();
window->InitGLFW();
window->CreateWindow("OpenMC", config->Width, config->Height, config->IsFullScreen);
window->InitGLAD();
window->InitImGui();

/* more code */

// 销毁窗体
window->DestroyImGui();
window->DestroyWindow();
```

在`Window`类中存储了GLFW的窗体指针`GLFWWindow`，在键盘鼠标输入、渲染等多个地方均需要使用该值。这里我们根据依赖注入的思想，为每个需要使用窗体指针的类实现绑定函数。(最简单最暴力的依赖注入)

```c++
// 初始化输入层
Input<0> * input = Singleton<Input<0>>::GetInstance();
input->Bind(window);
// 初始化摄像头
Camera* camera = Singleton<Camera>::GetInstance();
camera->Bind(window);
// 初始化玩家
Player* player = Singleton<Player>::GetInstance();
player->Bind(window);
```

然后再使用`Window`类提供的接口`GetWindow`来获取GLFW窗体指针，实现相应的逻辑。

#### Input类与事件委托

由于GLFW的各种回调事件仅能设置一次，且接受的回调事件参数为C类风格函数指针，这在C++到处都是类、类成员函数的情况下使用非常困难，并且我们希望能支持多个事件委托，可以把不同相关的事件逻辑分离开来。

我们这里简单实现一个回调类模板。

```c++
template<class T>
class Callback final {
public:
    Callback& operator +=(T callback) {
        callbacks.push_back(callback);
        return *this;
    }

    std::vector<T>& Get() {
        return callbacks;
    }

private:
    std::vector<T> callbacks;
};
```

然后实现一个输入类，该类负责游戏中所有的事件委托。以下仅为`Input`的部分代码，其多个回调事件处理见项目中源代码。

```c++
typedef std::function<void(double, double)> CursorPosChangedEvent;
typedef std::function<void(int, int, int, int)> KeyClickEvent;
typedef std::function<void(int, int, int)> MouseButtonClickEvent;
typedef std::function<void(double, double)> ScrollChangedEvent;

template<unsigned int ID>
class Input final {
public:
    static void Bind(Window* window) {
        Input<ID>::window = window;
        window->SetCursorPosCallback(&Input::cursorPosCallback);
        window->SetKeyCallback(&Input::keyCallback);
        window->SetMouseButtonCallback(&Input::mouseButtonCallback);
        window->SetScrollCallback(&Input::scrollCallback);
    }

    static Callback<CursorPosChangedEvent> OnCursorPosChanged;
    static Callback<KeyClickEvent> OnKeyClick;
    static Callback<MouseButtonClickEvent> OnMouseButtonClick;
    static Callback<ScrollChangedEvent> OnScrollChanged;
};
```

这里的`Input`类使用了模板来定义，并且其模板参数为一个无符号整型。模板的实现是为了确保能生成多个输入类，在多个场景或者更复杂的情况可以使用。不过这里我们整个游戏都是采用同一个输入类，因此模板实际上可以不实现。且不同输入类之间没有任何联系，不使用简单的类的原因是静态成员函数才能转换为C类风格函数指针，这是对GLFW接口的妥协。

#### Scene类与场景管理

OpenMC采用了场景管理的方式来实现渲染逻辑分离，主要实现了场景管理类和场景基类。

```c++
class SceneManager final {
public:
    ~SceneManager();

    void Run(Window* window);
    void Goto(Scene* scene);

private:
    Scene* scene = NULL;
    std::vector<Scene*> destroyScenes;
};
```

在场景管理类中，`Run`用于场景的运行，而`Goto`用于切换场景。切换场景并没有场景运行的逻辑，因此，我们仅需要在场景中`Goto`另一个场景，即可实现场景的切换。最终场景切换且调用的实际逻辑，是在`Run`中实现的。

```c++
void SceneManager::Run(Window* window) {
    while (!window->IsClose() && scene != NULL) {
        if (destroyScenes.size()) {
            for (auto& s : destroyScenes) {
                delete s;
            }
            destroyScenes.clear();
        }
        scene->Main(window);
    }
}
```

在场景基类中，我们实现了场景的执行流程。场景的`Main`函数由基类实现，其具体执行流程为`Start`、`Update`、`Terminate`，其中`Update`循环执行，而另外两个流程只会在场景开始和场景结束执行。`SetSceneChanging`不应该由场景内部主动调用，应该由场景管理类负责调用。

```c++
class Scene {
public:
    void Main(Window* window);

    virtual void Start() {}
    virtual void Update() {}
    virtual void Terminate() {}

    void SetSceneChanging(bool sceneChanging);

protected:
    Window* window;
    bool sceneExiting = false;

private:
    bool sceneChanging = false;
};
```

### 地图生成

地图生成以区块作为基本单位，即`16*256*16`的方块范围。

地图生成取决于地图种子，相同的地图种子将生成相同的地形。

首先使用地图种子来初始化一个32位无符号整型的伪随机数生成器，然后使用伪随机数生成器来生成出生点坐标、噪声种子等。

```c++
std::default_random_engine engine;
std::uniform_int_distribution<uint32_t> rand;
uint32_t seed;
```

然后使用多个不同参数的基于Perlin噪声的Octave噪声来生成地图，噪声主要用于生成高度图、密度图、概率。

```c++
OctaveNoise<PerlinNoise> depthNoise1;
OctaveNoise<PerlinNoise> depthNoise2;
OctaveNoise<PerlinNoise> thicknessNoise;
OctaveNoise<PerlinNoise> grassNoise;
OctaveNoise<PerlinNoise> flowerNoise;
OctaveNoise<PerlinNoise> treeNoise;
```

地图生成的主要流程如下：

1. 生成地形高度图：使用`depthNoise1`、`depthNoise2`噪声组合生成高度，对较低或较高的高度进行矫正。
2. 生成地形层厚度图：使用`thicknessNoise`生成泥土厚度。
3. 填充方块：填充基岩、圆石、泥土，同时为地表填充草方块。
4. 生成花草：使用`grassNoise`、`flowerNoise`生成花草。
5. 生成树木：使用`treeNoise`生成树木。

```c++
void MapGenerator::genBasicTerrain(Chunk* chunk, int32_t x, int32_t z) {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            // 生成高度图
            float noise1 = depthNoise1.Get((x + i + 0.1f) * 0.01f, (z + j + 0.1f) * 0.01f);
            float noise2 = depthNoise2.Get((x + i + 0.1f) * -0.01f, (z + j + 0.1f) * -0.01f);
            int height = static_cast<int>(noise1 * (noise2 * 32 + 16));
            if (height < 16) {
                height = (height + 16) / 2;
            }
            // 生成厚度图
            float noise3 = thicknessNoise.Get((x + i + 0.1f) * 0.01f, (z + j + 0.1f) * 0.01f);
            int thickness = static_cast<int>(noise3 * 4 + 2);
            for (int k = 0; k < height; k++) {
                if (k < height - thickness) {
                    chunk->SetBlock(x + i, k, z + j, BlockId::Stone);
                } else if (k < height - 1) {
                    chunk->SetBlock(x + i, k, z + j, BlockId::Dirt);
                } else {
                    chunk->SetBlock(x + i, k, z + j, BlockId::GrassBlock);
                }
            }
            // 生成草、花
            if (grassNoise.Get((x + i) * 1.0f, (z + j) * 1.0f) > 0.7) {
                chunk->SetBlock(x + i, height, z + j, BlockId::Grass);
            }
            if (grassNoise.Get((x + i) * 0.1f, (z + j) * 0.1f) > 0.75) {
                chunk->SetBlock(x + i, height, z + j, BlockId::BlueOrchid);
            }
            // 生成树
            if (i >= 2 && i <= 13 && j >= 2 && j <= 13) {
                if (treeNoise.Get(x + i, z + j) > 0.8) {
                    for (int k = 0; k <= 5; k++) {
                        chunk->SetBlock(x + i, height + k, z + j, BlockId::OakLog);
                    }
                    for (int k = 4, step = 2; k <= 6; k++) {
                        if (k == 6) {
                            step = 1;
                        }
                        for (int x0 = i - step; x0 <= i + step; x0++) {
                            for (int z0 = j - step; z0 <= j + step; z0++) {
                                chunk->SetBlock(x + x0, height + k, z + z0, BlockId::OakLeaves);
                            }
                        }
                    }
                }
            }
        }
    }
}
```

## 心得体会

在本次项目开发中，我个人主要是负责项目基本框架的搭建、地图生成还有Helpers等诸多辅助类的实现。

首先是项目构建的本身，接触并学习了Vcpkg，使用其来进行项目依赖的管理以及保证团队环境依赖的统一性。其次在基本框架上，也使用了事件委托、依赖注入等思想来实现架构，学到了不少东西。

在地图生成上，也接触了柏林噪声，以及查阅了不少关于原版或者第三方MineCraft的地图生成、存储等相关的信息。同时在渲染上也跟着Zhenly大佬学习了各种渲染相关知识和渲染优化的知识。
