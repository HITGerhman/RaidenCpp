cocos2d x，雷電
![PixPin_2025-12-27_16-53-53](https://github.com/user-attachments/assets/71728c6b-f62b-430c-8918-e5e4108fec79)

🛠️ 技术栈
引擎：Cocos2d-x v4.0

语言：C++ 11

构建工具：CMake 3.10+

IDE：Visual Studio 2019/2022 或 VS Code (配合 C++ 插件)

脚本：Python 2.7 (用于 Cocos 命令行工具)
💻 如何运行 (Windows)
环境准备
安装 Visual Studio 2019/2022（需勾选 "使用 C++ 的桌面开发"）。

安装 CMake 并添加到系统环境变量。

确保已安装 Python 2.7。

编译步骤
方法 A：使用 Visual Studio (推荐)

在项目根目录创建一个 build 文件夹。

使用 CMake 生成工程文件：

cd build
cmake .. -G "Visual Studio 16 2019" -A x64
# 或者如果你是 VS 2022:
# cmake .. -G "Visual Studio 17 2022" -A x64
打开生成的 .sln 文件，将 RaidenCpp 设为启动项目，点击运行。

方法 B：使用 VS Code

安装 C/C++ 和 CMake Tools 插件。

用 VS Code 打开项目根目录。

在底部状态栏选择 Kit 为 Visual Studio Community 2019/2022 Release - amd64。

点击底部的 Build 按钮，完成后点击 Run。
