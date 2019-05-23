# OpenMC

A simple game like Minecraft built by OpenGL.

## Required

- Windows
- Visual Studio 2019 (支持 Vcpkg，理论上 2017 也支持)
- Python 3 (支持 GLAD 源码生成)
- Git

## Quick Start

运行源码需要安装 Vcpkg。

```
$ git clone https://github.com/Microsoft/vcpkg.git
$ cd vcpkg
$ .\bootstrap-vcpkg.bat
$ .\vcpkg integrate install --triplet x64-windows
```

然后设置`VCPKG_ROOT`环境变量，为 Vcpkg 所在目录。

修改 GLAD 的默认 portfile，改为使用 Core 版本和指定 API 为 4.6。

```
$ vcpkg edit glad
```

```cmake
vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
    OPTIONS
        -DGLAD_NO_LOADER=OFF
        -DGLAD_EXPORT=OFF
        -DGLAD_INSTALL=ON
        -DGLAD_REPRODUCIBLE=ON
        -DGLAD_SPEC="gl" # {gl,egl,glx,wgl}
        -DGLAD_PROFILE="core" # {core,compatibility}
        -DGLAD_API="gl=4.6"
    OPTIONS_DEBUG
        -DGLAD_GENERATOR="c-debug"
)
```

安装依赖库。

```
$ vcpkg install glfw3:x64-windows glad:x64-windows glm:x64-windows imgui:x64-windows assimp:x64-windows stb:x64-windows freetype:x64-windows
```

使用 Visual Studio 启动该仓库内的解决方案，运行即可。

## Specify Triplet

Quick Start 是使用`x64-windows`平台编译运行程序，Vcpkg 支持以下的平台。

```
$ vcpkg help triplet
Available architecture triplets:
  arm-uwp
  arm-windows
  arm64-uwp
  arm64-windows
  x64-linux
  x64-osx
  x64-uwp
  x64-windows
  x64-windows-static
  x86-uwp
  x86-windows
  x86-windows-static
```

如果需要修改平台，先需要安装相应平台的库，然后修改`OpenMC.vcxproj`。

```xml
<PropertyGroup Label="Globals">
  <!-- .... -->
  <VcpkgTriplet Condition="'$(Platform)'=='Win32'">x86-windows-static</VcpkgTriplet>
  <VcpkgTriplet Condition="'$(Platform)'=='x64'">x64-windows-static</VcpkgTriplet>
</PropertyGroup>
```
