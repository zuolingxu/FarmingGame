环境配置
一、配置 cocos
1. 从该网站下载https://github.com/cocos2d/cocos2d-x中的stable version
2. 解压后在解压目录下打开cmd输入python download-deps.py安装扩展包
	——不会使用命令行以及文件路径请看：https://www.bilibili.com/video/BV13r421t77p/?p=2（如果不会书写文件路径必须要学）
	——需要python2，使用python --version查看版本，如果当前版本为python3.x，安装并学习miniconda，或卸载系统python并重新下载并安装python2.x
3. 然后运行setup.py
4. 在cmd中尝试cocos指令，成功即正常运行

二、项目配置
1. 从github拉取本项目，使用git clone <url>l指令
	——通过下载github桌面端（直接拉取，操作简单）和git桌面端（使用命令行）
	——不会使用git先学习：https://www.bilibili.com/video/BV13r421t77p?p=7以及https://missing-semester-cn.github.io/2020/version-control/
2. 在项目根目录下使用cmd命令初始化子模块 git submodule update --init --recursive 目前应该没什么效果
3. 新建一个文件夹build（为了简洁），进入build文件夹，运行cmake .. -G "Visual Studio 17 2022" -A win32指令
4. 运行完上述文件后，会得到.sln文件，即可打开VS进行后续编译工作

三、项目编译
1. 将FarmingGame设为启动项，直接编译即可

四、源代码书写与修改
1. 直接在VS中编辑即可，生成的sln相当于目录，在VS中修改的代码保存后会直接反应到代码文件中，所有的自己写的文件都放到class中。
2. 假如增加文件，或者重命名文件，需要在CMakeLists中添加源文件的文件名
	——不会使用cmake请查看该文档：https://docs.cocos2d-x.org/cocos2d-x/v4/zh/installation/CMake-Guide.html 的 常见问题-如何添加 C/C++ 源码? 部分
3. 然后重新配置项目（重新进行项目配置）

**可以参考https://www.bilibili.com/video/BV1g34y1n7HQ/但请以本文档中的步骤为准


cocos2d-x学习
一、HelloWorld项目理解
1. 掌握基本的阅读代码以及cocos lib的文件能力（VS中按住ctrl单击即可查看定义进行溯源）
	——https://www.bilibili.com/video/BV1JosjeKEfe，从p2开始看
2. 简单理解单例模式（Cocos很多单例模式的类）
	——https://www.bilibili.com/video/BV1af4y1y7sS/
二、官方文档及代码
1. 掌握cocos基本的组织架构，基本概念必须全部阅读，进阶内容选择自己需要的看，文档必须看
	——https://docs.cocos2d-x.org/cocos2d-x/v4/zh/basic_concepts/
2. 打开cocos2d-x文件所在文件夹，进入cocos2d-x-4.0\tests\cpp-tests，按照项目配置的步骤2、3编译项目，查看各种动画（一共六十多项，鼠标滚轮可以上下滑动），可以查看对应源代码，代码可以直接copy到项目中。


代码规范
一、git上传规范
1. 不要将本地的.idea/.vs/debug等文件上传！只上传你写的源代码！
2. 对每个commit的说明要认真写
3. 所有的源代码都放到class中，所有的资源文件都放到resource中

二、数据储存规范
A. 文本数据储存规范
1. 由于xml学习成本较高，本项目使用Json作为数据存储规范
	——Json相关知识：https://www.bilibili.com/video/BV1We411y7wn/
2. C++原生不支持Json，需要使用外部扩展库：rapidjson，外部扩展库cocos2d-x自带
	——学习：https://rapidjson.org/zh-cn/md_doc_tutorial_8zh-cn.html，只需要看教程部分即可，学会从document读取和写入。
	——rapidjson使用C风格字符串处理json文件，但是C风格字符串是不安全的，请使用string类处理、储存。
	——对于设定书写json文档（用户文档），统一使用copy-string策略，确保用户数据不会因对象生命周期的限制而发生错误，即每次都提供分配器（提供doucument的就可以了），分配器不要混用，假如valueA要存入doucumentA，那么就不要用documentB的allocator。
	——因为硬盘io时间远大于内存读取时间，能够使用同一文档的情况下请避免多次读取（比如：植物类的全部信息只在第一次创建对象时加载）。为此，创建了JsonMananger类，统一管理所有的json文件，具体接口功能说明请查看头文件。禁止将json文件命名为UsrArchive和UsrConfig，该二者有特殊含义。
3. 一个AI生成Json样例就在Resource文件夹下
4. 避免使用任何形式的直接在源代码中给出魔法常量，即没有任何定义直接给出的数据。

B. 场景数据储存规范
1. 首先，安装cocos dashboard，然后再dashboard中安装cocos creator 2.4.7
	——学习： https://docs.cocos2d-x.org/cocos2d-x/v4/zh/editors_and_tools/creator_to_cocos2dx.html
2. 严格按照教程配置，三个导出选项可以选择：Export Resource Dynamically Loaded

C. 地图数据储存规范
1. 地图数据使用tiled： https://www.mapeditor.org/ 生成
	——学习： https://www.bilibili.com/video/BV1P54y1F78b/

三、命名规范
1. 除说明文档以外的所有文件夹、源代码、资源文件必须以英文命名，以类的类型结尾，命名要符合含义，如果不会英文请进行查询，如：场景的源代码必须以Scene结尾
2. 不同的功能的源代码文件只能在该功能指定的文件夹下
