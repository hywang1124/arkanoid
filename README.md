# Arkanoid 打砖块游戏

这是一个使用 C++ 和 raylib 实现的期末项目。玩家通过键盘控制挡板，让小球反弹并击碎所有砖块。

## 运行环境

- Windows
- MSYS2 UCRT64 g++
- raylib

如果使用 MSYS2，可以安装 raylib：

```powershell
pacman -S mingw-w64-ucrt-x86_64-raylib
```

## 构建方法

```powershell
.\build.ps1
```

构建成功后，可执行文件会生成在：

```text
build/arkanoid.exe
```

## 运行方法

```powershell
.\build\arkanoid.exe
```

## 操作方式

启动游戏的 cmd 或 PowerShell 控制台会打印每次键盘按下的操作日志。

- `A` 或 `Left`：向左移动挡板
- `D` 或 `Right`：向右移动挡板
- `Space`：开始游戏或重新开始
- `P`：暂停或继续
- `Esc`：退出游戏
- 隐藏输入 `thereisnospoon`：解锁作弊系统，并显示尼奥折弯勺子的 ASCII Art 画面

作弊系统解锁后：

- `1`：提高所有小球速度
- `2`：生成一个额外小球
- `3`：临时加宽挡板
- `4`：增加一条生命
- `5`：开启临时 Matrix Shield，底部漏球会反弹
- `6`：发动 Spoon Wave，直接破坏前方多个砖块

## 主要功能

- 图形窗口显示
- 挡板控制
- 小球反弹
- 砖块碰撞与破坏
- 分数系统
- 生命值系统
- 胜利和失败状态
- 重新开始
- 隐藏作弊码和 ASCII Art 过场
- 多球、加速、护盾、加宽挡板等作弊功能
- 3 个关卡，后续关卡会改变砖块排列并提高球速

## 项目结构

```text
arkanoid/
  AGENTS.md
  README.md
  build.ps1
  src/
    main.cpp
    Game.cpp
    Game.h
    Paddle.cpp
    Paddle.h
    Ball.cpp
    Ball.h
    Brick.cpp
    Brick.h
    Level.cpp
    Level.h
```
