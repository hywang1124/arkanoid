# Arkanoid / Breakout 打砖块游戏

这是一个使用 C++17 和 raylib 实现的期末项目。玩家通过键盘控制屏幕底部的挡板，让小球反弹并击碎砖块，在生命值用完前完成 3 个关卡。

项目重点展示了类与对象、函数拆分、`std::vector`、碰撞检测、状态管理和图形窗口实时输入等 C++ 技术。

## 运行环境

- Windows
- MSYS2 UCRT64 g++
- raylib
- PowerShell

如果使用 MSYS2，可以安装 raylib：

```powershell
pacman -S mingw-w64-ucrt-x86_64-raylib
```

## 构建方法

在项目根目录执行：

```powershell
.\build.ps1
```

构建成功后，可执行文件会生成在：

```text
build/arkanoid.exe
```

`build.ps1` 会编译 `src/` 下的所有 `.cpp` 文件，并链接 raylib、OpenGL、GDI 和 Windows 多媒体库。

## 运行方法

```powershell
.\build\arkanoid.exe
```

启动后会打开 raylib 图形窗口。启动游戏的 cmd 或 PowerShell 控制台会打印键盘输入日志，便于调试和发表时说明操作。

## 操作方式

### 基本操作

- `A` 或 `Left`：向左移动挡板
- `D` 或 `Right`：向右移动挡板
- `Space`：开始游戏，或在胜利/失败后重新开始
- `P`：暂停或继续
- `Esc`：退出游戏

### 隐藏功能

在游戏窗口中输入：

```text
thereisnospoon
```

输入成功后会解锁作弊系统，并显示短暂的文字提示。

作弊系统解锁后可以使用：

- `1`：提高所有小球速度
- `2`：生成一个额外小球
- `3`：临时加宽挡板
- `4`：增加一条生命
- `5`：开启临时 Safety Shield，底部漏球会反弹
- `6`：发动 Spoon Wave，直接破坏前方多个砖块

## 游戏规则

- 玩家控制挡板接住小球。
- 小球碰到墙壁、挡板或砖块后会反弹。
- 击破砖块可以获得分数。
- 部分砖块有耐久度，需要击中多次才能破坏。
- 漏接所有小球后减少 1 条生命。
- 生命值归零时游戏失败。
- 清除当前关卡全部砖块后进入下一关。
- 通过第 3 关后游戏胜利。

## 主要功能

- raylib 图形窗口显示
- 实时键盘输入
- 挡板移动和边界限制
- 小球自动移动和反弹
- 墙壁、挡板、砖块碰撞检测
- 砖块网格生成
- 砖块耐久度
- 分数系统
- 生命值系统
- 开始、游戏中、暂停、胜利、失败状态
- 3 个关卡
- 关卡越高，球速越快
- 多球、加速、护盾、加宽挡板等隐藏扩展功能
- PowerPoint 发表资料

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
  slides/
    Arkanoid_Final_Presentation.pptx
```

说明：

- 当前项目使用 `build.ps1` 构建，没有使用 CMake。
- 当前项目没有外部图片、音效或字体资源。
- `slides/Arkanoid_Final_Presentation.pptx` 是最终发表资料。

## 类设计

### Game

`Game` 是游戏的核心管理类，负责：

- 初始化窗口和主循环
- 管理游戏状态
- 处理输入
- 更新游戏对象
- 处理碰撞
- 管理分数、生命和关卡
- 绘制 HUD、提示文字和作弊面板

### Paddle

`Paddle` 负责：

- 保存挡板位置和大小
- 根据键盘输入移动
- 防止挡板离开屏幕
- 提供碰撞用的矩形范围
- 支持临时加宽

### Ball

`Ball` 负责：

- 保存小球位置、速度、半径和速度值
- 根据 `deltaTime` 更新位置
- X/Y 方向反弹
- 标准化速度，保持移动稳定
- 支持速度倍率调整

### Brick

`Brick` 负责：

- 保存砖块的位置、颜色、分数和耐久度
- 绘制砖块
- 被击中后减少耐久度
- 耐久度为 0 时进入破坏状态

### Level

`Level` 负责：

- 管理当前关卡
- 生成砖块布局
- 调整砖块排列和耐久度
- 设置不同关卡的小球速度
- 判断是否是最终关卡

## 游戏状态

游戏使用 `GameState` 枚举管理流程：

- `Start`：开始画面或关卡开始前
- `Playing`：游戏进行中
- `Paused`：暂停
- `Win`：全部通关
- `GameOver`：游戏失败

只有在 `Playing` 状态下，挡板、小球、碰撞和砖块清除逻辑才会持续更新。

## C++ 技术点

发表时可以重点说明：

- 类与对象
- 封装
- 头文件和源文件分离
- `std::vector` 管理多个砖块和多个小球
- 枚举管理游戏状态
- 函数拆分游戏逻辑
- 条件判断和循环
- 碰撞检测
- `deltaTime` 控制移动速度
- 使用 raylib 实现图形窗口、绘图和实时输入

## Demo 建议流程

1. 启动程序，展示开始画面。
2. 按 `Space` 开始游戏。
3. 使用 `A` / `D` 或左右方向键移动挡板。
4. 击碎几个砖块，展示分数增加。
5. 故意漏接一次，展示生命减少。
6. 按 `P` 展示暂停和继续。
7. 如时间允许，输入 `thereisnospoon` 展示隐藏功能。
8. 简短说明 3 个关卡和耐久度砖块。

## 发表资料

PowerPoint 文件位于：

```text
slides/Arkanoid_Final_Presentation.pptx
```

PPT 使用日文，内容包括项目概要、游戏规则、类设计、重要实现、遇到的问题、解决方法、Demo 和总结。

提交前请确认标题页中的成员姓名和学生 ID 已补全。

## 提交前检查

- `.\build.ps1` 可以成功构建。
- `build/arkanoid.exe` 可以正常启动。
- 挡板移动正常。
- 小球反弹正常。
- 砖块可以被破坏。
- 分数可以增加。
- 生命值可以减少。
- 暂停、胜利、失败状态正常显示。
- PPT 标题页包含成员姓名和学生 ID。
- ZIP 中不要包含 `build/`、临时文件或无关草稿。
