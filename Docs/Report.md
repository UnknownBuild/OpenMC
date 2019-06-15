# 我的世界

## 项目介绍以及实现结果 







## 开发环境以及使用到的第三方库



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



##  实现功能列表(Basic与Bonus) 

### Basic：

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



## 对实现的功能点做简单介绍(Bonus主要介绍实现原理)，加结果截图 

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





### Sky Box

![skybox](Report/skybox.gif)

天空盒主要通过立方体贴图实现





## 遇到的问题和解决方案 



## 小组成员分工



















