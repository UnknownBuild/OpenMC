# 我的世界

## 项目介绍以及实现结果 

《我的世界》是一款3D的第一人称沙盒游戏，所呈现的世界并不是华丽的画面与特效，而是注重在游戏性上面。

玩家在游戏中做着「建设」与「破坏」两件事，但是透过像乐高一样的积木来组合与拼凑，轻而易举的就能制作出小木屋、城堡甚至城市，但是若再加上玩家的想像力，空中之城、地底都市都一样能够实现。

本次期末项目将使用OpenGL复刻《我的世界》。



## 开发环境以及使用到的第三方库

使用`Vcpkg`管理第三方库

```
glfw3:x64-windows 
glad:x64-windows 
glm:x64-windows 
imgui:x64-windows 
assimp:x64-windows 
stb:x64-windows 
freetype:x64-windows 
rapidjson:x64-windows
```

##  实现功能列表

### Basic

- Camera Roaming
- Simple lighting and shading
- Texture mapping
- Shadow mapping (discarded)
- Model import

### Bonus

- Sky Box
- Display Text

- Complex Lighting
  - G-Buffer and Deferred Shading (*discarded*)
  - SSAO (*discarded*)
  - Custom AO
  - Multiple Light Sources
  - Custom Block Lighting (doing)
- Anti-Aliasing
- Selective Block Rendering
- Instancing
- Face Culling
- Blending
- Fog View
- Gravity System and Collision Detection (todo)
- Map generation base on Perlin Noise (doing)
- Map System



## 对实现的功能点做简单介绍

### Camera Roaming

封装了一个 Camera 类

- 支持自定义视角轨迹

![menu](Report/menu.gif)

- 支持自由视角

![free](Report/free.gif)



### Light

#### 多光源

整个场景有一个平行光和多个点光源

![1560568737288](Report/1560568737288.png)

主要实现方法就是在着色器中计算多个点光源并将其叠加起来

```clike
uniform PointLight pointLights[NR_POINT_LIGHTS];

void main() {
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	// 计算定向光源
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	// 计算点光源
	for (int i = 0; i < pointCount; i++) {
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}

	FragColor = vec4(result, 1.0);
}
```

#### 点光源的衰减

对于单个点光源的计算，按照以下的公式实现了距离的衰减

![1560568934005](Report/1560568934005.png)

主要的效果为光在近距离时亮度很高，但随着距离变远亮度迅速降低，最后会以更慢的速度减少亮度。

![1560568967312](Report/1560568967312.png)

这里通过根据不同的光照强度定义点光源的常数项、一次项和二次项来实现。

```clike
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
```



#### 自定义光强

由于简单的光源对于一些遮挡住的情况不能正确显示，这里参考了我的世界中的光照实现，把光强分为16个等级。

首先从最上方注入垂直光源

![img](Report/bd4002983e6abd558de532f3c16f40f4_hd.jpg)

然后对其进行16次迭代扩散

![img](Report/f64541b91c100e1a74eb8dba7b4ec092_hd.jpg)

这样每个方块的表面都会有一个光强值。

这里暂时只实现了垂直光的注入，扩散和点光源的注入有待实现。

![Snipaste_2019-06-14_21-49-57](Report/Snipaste_2019-06-14_21-49-57.jpg)

### Shadow

对于阴影的实现，我们考虑了很多种方法。

首先是传统的阴影映射，通过光源视角的映射来绘制阴影。然而使用这种方法对于一些透明或者不规则的方块，如玻璃、火把、树叶、水面等没有办法正确实现他们的阴影，因此我们抛弃了这一种方法。

从图中可以看到，玻璃是半透明的方块，草是部分透明的方块，这些方块的阴影的实现都会很奇怪。

![Snipaste_2019-06-14_21-51-08](Report/Snipaste_2019-06-14_21-51-08.jpg)

然后，我们通过G缓冲实现了延迟着色法，并且使用了**屏幕空间环境光遮蔽(Screen-Space Ambient Occlusion, SSAO)**的技术，用了屏幕空间场景的深度而不是真实的几何体数据来确定遮蔽量。

![Snipaste_2019-06-02_13-26-17](Report/Snipaste_2019-06-02_13-26-17.jpg)

实验发现这种方法带来的性能下降比较多，明显的一点是帧数大幅度下降了，而且其效果也一般般。

然后，我们了解了我的世界中环境阴影的实现，实现了一种自定义的环境光遮蔽

![img](Report/846f377b94c632f8b61fe9c5d4c91fc2_hd.jpg)

根据边上的方块的数量，对其进行光线的衰减。

这里通过着色器中的内建变量`gl_VertexID`来分别渲染正方体的不同面的遮挡效果。

```clike
if (gl_VertexID >= 0 && gl_VertexID < 4) {
  lightColor = vec4(lightColor.xyz * aoFront[gl_VertexID % 4] ,lightColor.a);
}
if (gl_VertexID >= 4 && gl_VertexID < 8) {
  lightColor = vec4(lightColor.xyz * aoBack[gl_VertexID % 4] ,lightColor.a);
}
if (gl_VertexID >= 8 && gl_VertexID < 12) {
  lightColor = vec4(lightColor.xyz * aoLeft[gl_VertexID % 4] ,lightColor.a);
}
if (gl_VertexID >= 12 && gl_VertexID < 16) {
  lightColor = vec4(lightColor.xyz * aoRight[gl_VertexID % 4] ,lightColor.a);
}
if (gl_VertexID >= 16 && gl_VertexID < 20) {
  lightColor = vec4(lightColor.xyz * aoBottom[gl_VertexID % 4] ,lightColor.a);
}
if (gl_VertexID >= 20 && gl_VertexID < 24) {
  lightColor = vec4(lightColor.xyz * aoTop[gl_VertexID % 4] ,lightColor.a);
}
```

在没有开启AO之前，一些被遮挡的方块的光线不能正确显示，而使用之后就可以看出来正常很多，效果对比：

| 未开启AO                                   | 开启AO                                                       |
| ------------------------------------------ | ------------------------------------------------------------ |
| ![1560575829359](Report/1560575829359.png) | ![Snipaste_2019-06-15_00-34-35](Report/Snipaste_2019-06-15_00-34-35.jpg) |

由于AO只有在一开始生成方块的时候需要计算，因此性能还是挺好的。

但是由于分区块渲染的原因，有些部分的AO渲染有待优化。



### Sky Box

![skybox](Report/skybox.gif)

天空盒通过立方体贴图实现

![img](Report/cubemaps_skybox.png)

通过六个面的填空图像合成一个天空盒，在渲染的时候，我们需要天空盒显示在所有物体的后面，一个方法是一开始先渲染天空盒，但是在一般情况下，天空盒只是有少部分是可见的。因此这里使用了提前深度测试来丢弃片段，最后才渲染天空盒。

在着色器中，把天空盒的w分量调成`1`

```clike
void main() {
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
```

然后需要把深度测试的方式改成`GL_LEQUAL`，表示在片段深度值小于等于缓冲区的深度值时通过测试，这样天空盒就会被认为是最后面的

```c++
glDepthFunc(GL_LEQUAL);
this->skyShader->Use();
glBindVertexArray(skyboxVAO);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_CUBE_MAP, this->skyBox->ID);
glDrawArrays(GL_TRIANGLES, 0, 36);
glBindVertexArray(0);
glDepthFunc(GL_LESS);
```

最后把深度测试方式调回默认的`GL_LESS`



### Block Render

在我的世界里面，绝大多数对象都是由**方块+纹理**组成的，因此纹理是这里比较重要的一个部分。

#### 自定义方块

由于游戏中方块对象比较多，而且每个方块又有着各自的纹理以及渲染方式，因此我们将方块的数据抽取到`json`文件中，通过读取`json`文件来获取方块的纹理、类型和渲染方式等信息。

```json
 [
  {
    "id": 0,
    "name": "air",
    "type": 0,
    "render": 12,
    "textures": [],
    "colors": [],
    "animation": 0,
    "light": 0
  },
  {
    "id": 1,
    "name": "stone",
    "type": 2,
    "render": 2,
    "textures": ["stone.png"],
    "colors": [[0.8, 0.8, 0.8, 1.0]],
    "animation": 0,
    "light": 0
  },
  {
    "id": 2,
    "name": "grass_block",
    "type": 2,
    "render": 3,
    "textures": ["grass_block_side.png", "grass_block_top.png"],
    "colors": [[0.57, 0.73, 0.37, 1.0], [0.61, 0.45, 0.37, 1.0]],
    "animation": 0,
    "light": 0
  },
  {
    "id": 3,
    "name": "dirt",
    "type": 2,
    "render": 12,
    "textures": ["dirt.png"],
    "colors": [],
    "animation": 0,
    "light": 0
  },
   ...
]
```

只需要修改这个文件，就可以轻松地添加新的方块。

```c++
class BlockData final {
public:
    BlockId Id;
    std::string Name;
    BlockType Type;
    RenderType Render;                  // 渲染类型
    std::vector<Texture2D> Textures;    // 材质
    std::vector<glm::vec4> Colors;      // 颜色
    uint8_t Light;                      // 自身亮度
    uint8_t Animation;                  // 动画速度 (0-5)
};
```

这样，在场景中只需提供方块ID以及位置，就可以轻松地进行渲染

```c++
// 渲染火把
renderer->DrawBlock(BlockId::Torch, torchPosition);
```

每个方块都有方块类型和渲染类型的信息，这些信息决定了碰撞、光照以及纹理的渲染

```c++
enum class BlockType : uint8_t {
    None = 0,           // 无实体
    Liquid = 1,         // 液体
    Solid = 2,          // 固体
    Face = 3,           // 单面
    TransSolid = 4,     // 透明固体
    TransFace = 5,      // 透明单面
};

enum class RenderType : uint8_t {
    None = 0,              // 不渲染
    ThreeTexture = 1,       // 三纹理方块(上、四周、下)
    NoiseTexture = 2,       // 噪声 + 颜色
    SideTexture = 3,        // 上下噪声，四周纹理
    CenterCrossTexture = 4, // 中心交叉纹理
    TorchTexture = 5,       // 火把
    FireTexture = 6,        // 火焰
    CustomTexture = 7,      // 六面自定义方块 (前后左右上下)
    DirCustomTexture = 8,   // 具有方向的方块 （前、侧边、上下）
    DoorTexture = 9,        // 门
    GlassTexture = 10,      // 玻璃
    LiquidTexture = 11,     // 液体表面
    OneTexture = 12,         // 单纹理方块
    OffsetTexture = 13,
    OffsetSideTexture = 14, 
};
```

然后根据类型进行渲染

```c++
  switch (type) {
    case RenderType::OneTexture: // 单纹理方块
        if (colors.size() > 0) {
            shader->SetInteger("hasColor", true);
            shader->SetVector4f("material.color", colors[0]);
        }
        textures[0].Bind();
        glBindVertexArray(this->quadVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, count);
        break;
    case RenderType::ThreeTexture: // 三纹理方块(上、四周、下)
        textures[1].Bind();
        glBindVertexArray(this->quadVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0, count);

        textures[0].Bind();
        glBindVertexArray(this->topVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);

        textures[2].Bind();
        glBindVertexArray(this->bottomVAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);
        break;
     // ...
  }
```

目前暂时支持我的世界中20+种方块的渲染

![1560573977460](Report/1560573977460.png)

#### 动态纹理

对于一些动态贴图的方块，我们也做了支持

火焰

![fire](Report/fire.gif)

水面

![water](Report/water.gif)

海晶灯

![more](Report/more.gif)

主要的实现方法是直接划分贴图中的数据，提取出多张纹理

```c++
for (int i = 0; i < count; i++) {
    Texture2D texture;
    texture.Image_Format = format;
    texture.Internal_Format = format;
    texture.Generate(sWidth, sHeight, image + (nrChannels * sWidth * sHeight) * i);
    textures.push_back(texture);
}
```
然后在渲染的时候，根据`json`文件中的速度信息，选择不同的纹理进行渲染。

```c++
if (block->data.Animation != 0) {
  frame = (this->renderFrame / block->data.Animation) % block->data.Textures.size();
}
```



### Model

对于我的世界中一些生物，我们直接加载模型进行展示

![1560574646144](Report/1560574646144.png)



### Display Text

对于文字的渲染，这里使用了`Freetype`这个库

其实现原理是首先加载字体文件，然后创建 `128` 个英文字符的纹理，因此这里的文字渲染只支持英文和标点。

```c++
// 初始化字体
void ResourceManager::InitFont(const GLchar* path) {
  // ...

  for (GLubyte c = 0; c < 128; c++) {
    // ... 创建字体纹理
    Characters.insert(std::pair<GLchar, Character>(c, character));
  }
  // ...
}
```

然后渲染的时候，绑定指定的纹理即可。

```c++
void SpriteRenderer::RenderText(std::string text, glm::vec2 position, GLfloat scale, glm::vec4 color) {
    fontShader->Use();
    fontShader->SetVector4f("textColor", color);
    fontShader->SetInteger("text", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(ResourceManager::fontVAO);
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        // ...
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, ResourceManager::fontVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        position.x += (ch.Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
```

这样就可以实现了：

![1560574958308](Report/1560574958308.png)

需要注意的是，对于字体的边缘部分，我们需要通过着色器对其进行抛弃。

```clike
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec4 textColor;

void main() {
    if (texture(text, TexCoords).r < 0.1) {
        discard; // 抛弃边缘透明部分
    }
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = textColor * sampled;
}
```

### Anti-Aliasing

由于这个游戏中都是一些方块组成的，因此有些地方的锯齿现象比较明显，因此使用了抗锯齿的技术。

![img](Report/anti_aliasing_rasterization_samples_filled.png)

这里的反锯齿主要是通过多次采样渲染，在`glfw`中，只需要在创建窗口的时候调节`GLFW_SAMPLES`采样数并且开启多采样即可。

```c++
glfwWindowHint(GLFW_SAMPLES, 4);
glEnable(GL_MULTISAMPLE);
```

这样就可以得到更平滑的边缘：

未开启抗锯齿

![1560576613868](Report/1560576613868.png)

开启反锯齿

![1560576655597](Report/1560576655597.png)



### Selective Block Rendering

选择性可视区块渲染

// to write



### Instancing

实例化数组

// to write



### Face Culling

面剔除

// to write



### Blending

混合

// to write



### Fog View

迷雾

// to write



### Gravity System and Collision Detection

重力系统和物理碰撞

// TODO

### Map generation

使用柏林噪声生成地图

// TODO



### Map System

加载和存储地图

// TODO



## 遇到的问题和解决方案 



## 小组成员分工



















