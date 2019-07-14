# PAOGD | Final Report

## 实现功能

### 碰撞检测

碰撞检测是基于AABB包围盒的思想，因为官方MC中，玩家是占据两个方块的，因此我们把玩家原点设置在了上面的那个方块，也就是摄影机的位置（第一人称）。根据AABB包围盒的思想，只需要检测玩家周围的八个方块，再检测下方和上方的方块，一共十个方块。

```c++
class Collision
{
public:
    Collision();
    ~Collision();

    glm::vec3 adjustPosition(glm::vec3);
    bool checkDown(glm::vec3);
    bool checkUp(glm::vec3);
    bool checkPositiveZ(glm::vec3);
    bool checkNegativeZ(glm::vec3);
    bool checkPositiveX(glm::vec3);
    bool checkNegativeX(glm::vec3);

private:
    bool checkPosition(glm::vec3);
};
```

这里需要说明的方法是`checkPosition`，这个方法用于检测目标位置是否为固体方块，这里调用了其他组员提供的接口，这个接口只需要传入一个类型为`glm::vec3`的位置信息数据，就可以返回该位置的方块信息，这里简单说明一下我们的方块信息，其中有一个属性`Type`，这个属性说明了方块的类型，固体，无（空气）或者液体等等。这里检测到该位置方块的类型是不可穿越时，则返回true。因为其他组员搭建的框架已经把方块的坐标标准化，这里需要检测的时候需要调整一下坐标。

```c++
bool Collision::checkPosition(glm::vec3 postion)
{
    postion = adjustPosition(postion);
    BlockData data = Singleton<SpriteRenderer>::GetInstance()->GetBlock(postion);
    return !(data.Type == BlockType::None || data.Type  == BlockType::Liquid);
}
```

### 重力系统

```c++
class Gravity
{
public:
    Gravity();
    ~Gravity();

    float getVelocity();
    void setVelocity(float velocity);
    float UpdateVelocity(float deltaTime);

private:
    bool able;
    float a;
    float velocity;
};

```

重力系统的实现是根据加速度计算公式`v = v0 + at`，代码中的a就是重力加速度，这里根据具体的调试之后调整这个值到合适的值。重力只是简单的计算当前垂直方向上的速度，但是在实现过程发现一个问题，当玩家从比较高的地方跳下来之后，碰撞检测会失效，经过实验发现问题出在了垂直方向上的速度过快问题，解决这个问题的方法就是当速度达到一定值的时候，重力加速度不再作用在垂直方向速度上。

### 惯性模拟

如果你是一位MC的爱好者，那么你一定会仔细的发现到MC中移动是具有惯性的。为了模拟惯性，这里对原始的键盘响应事件进行了修改，原本的方向键处理是每按一次键就增加固定的位移，这样无法实现惯性。在这里使用了速度驱动的方式，在这里我记录下了四个方向的速度（以摄影机的正方向为前方向的前后左右四个方向）。当玩家按下方向键的时候，给对应方向设置一个合适的初速度，然后在循环中通过加速度计算公式`v = v0 + at`对各个方向的速度进行衰减，如果玩家按住方向键，则该方向的速度一直为恒定的，一旦玩家松开按键，那么这个速度就会按照设定的加速度衰减，最终停下，这样就模拟出了惯性的效果。

```c++
void Player::updateVelocity() {
    Position += glm::vec3(Front.x, 0.0f, Front.z) * this->FrontVelocity * deltaTime;
    Position -= glm::vec3(Front.x, 0.0f, Front.z) * this->BackVelocity * deltaTime;
    Position += glm::vec3(Right.x, 0.0f, Right.z) * this->RightVelocity * deltaTime;
    Position -= glm::vec3(Right.x, 0.0f, Right.z) * this->LeftVelocity * deltaTime;

    this->FrontVelocity = this->FrontVelocity - this->acceleration * deltaTime;
    if (this->FrontVelocity < 0.0f) this->FrontVelocity = 0.0f;
    this->BackVelocity = this->BackVelocity - this->acceleration * deltaTime;
    if (this->BackVelocity < 0.0f) this->BackVelocity = 0.0f;
    this->RightVelocity = this->RightVelocity - this->acceleration * deltaTime;
    if (this->RightVelocity < 0.0f) this->RightVelocity = 0.0f;
    this->LeftVelocity = this->LeftVelocity - this->acceleration * deltaTime;
    if (this->LeftVelocity < 0.0f) this->LeftVelocity = 0.0f;
}
```

### 第一人称与第三人称

在这里需要区分玩家与摄影机，第一人称就是把玩家和摄影机重合。上面提及的碰撞检测、重力以及惯性全都是作用在玩家上。第三人称的实现主要使用了玩家的前向量，通过将摄影机固定在玩家的后方即可。

```c++
// 渲染模型
if (camera->perspective == Camera::Perspective::Third) {
    renderer->DrawSprite(ResourceManager::GetModel("jjm2"), 
                         player->Position + glm::vec3(0, -1.5f, 0), 
                         glm::vec3(3.4), 
                         -glm::radians(camera->Yaw) - glm::radians(90.0f), 
                         true);
    camera->Position = player->Position -
        glm::vec3(player->Front.x * 3, player->Front.y * 3, player->Front.z * 3);
}
else if(camera->perspective == Camera::Perspective::First){
    camera->Position = player->Position;
}
```

![background](../Docs/Report/background.gif)