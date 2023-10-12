# 飞腾校企联合育人项目
## 项目简介
使用opencv + Qt + mysql完成一个人脸识别和温度检测的项目
## 项目成员
- 王亚宁
- 高泽能
- 王陈希
## 项目结构
### 1.日志系统
(1) 仿照Log4j

    Logger（定义日志类别）
       |
       |---------Formatter(日志格式)
       |
    Appender(日志输出位置)

### 2.配置系统
Config --> Yaml

Yaml-cpp : github.com/jbeder/yaml-cpp

```cpp
YAML::Node node = YAML::LoadFile("config.yaml");
node.IsMap();
for(auto it = node.begin(); it != node.end(); ++it) {
}

node.IsSequence();
for(size_t i = 0; i < node.size(); ++i) {
}

node.IsScalar();
```
配置系统的原则，约定优于配置，配置文件的格式要简单，易于阅读，易于修改

```cpp
template<T, FromStr, ToStr>
class ConfigVar;

template<F,T>
LexicalCast;

// 容器偏特化，支持vector,list,set,unordered_set
// map,unordered_map 支持key = std::string的偏特化
// Config::Lookup(key) , key相同，但是类型不同的，不会报错，这是一个问题
```

自定义类型，需要实现ConstantineQAQ::LexicalCast<T, std::string>的偏特化，实现后，就可以支持Config解析自定义类型，自定义类型可以和常规STL容器一起使用

配置的事件机制
当一个配置项发生修改的时候，可以反向通知对应的代码，回调

#### 日志系统整合配置系统
![顶层类图](/resources/UML图.png) 
```yaml
logs:
    - name: root 
    - level: debug (debug,info,warn,error,fatal)
    - formatter: "%d%T%p%T%t%m%n"
    - appender:
        - type: (StdoutLogAppender, FileLogAppender)
          level: (debug,...)
          file: /logs/xxx.log
```

```cpp
// 解决动态绑定日志器的问题
ConstantineQAQ::Logger g_logger = 
ConstantineQAQ::LoggerMgr::GetInstance()->getLogger(name);
CONSTANTINEQAQ_LOG_INFO(g_logger) << "xxx log";
```

```cpp
static Logger::ptr g_log = CONSTANTINEQAQ_LOG_NAME("system");
// m_root, m_system没东西时还是用m_root写logger，当m_system有东西时就会换成新的值写logger。
```

```cpp
// 定义LogDefine LogAppenderDefine，偏特化LexicalCast，
// 实现了日志解析
```

使用processon画图，画出类的关系图，类的关系图，类的继承关系图，类的组合关系图，类的依赖关系图

### 3.人脸系统
基本已经开发完成：用到的思路是：
```cpp
/**
 * 单例模式摄像头类
 * 使用宏定义获取摄像头对象，删除摄像头对象，开启摄像头，清除缓存帧
 */
singleton<Camera>

/**
 * 图像信息配置类
 * 使用yaml-cpp从配置文件中读取参数：
 * 例如：
 *      face_config:
        confidence: 70.12
        ssim: 0.92
        face_sum: 10
        face_size: 100
        scale_factor: 1.1
        min_neighbors: 3    
        face_path: /home/kylin/kylin/face
 * 与Face类是一对一的关系
*/
class FaceConfig

/**
 * 图像信息类
 * 调用宏定义从摄像头中获取RawData并进行初步处理，然后一对一交给FaceInfo类
 */
class Face

/**
 * 人脸信息类
 * 继承图像信息类，从RawData中的原始数据中提取人脸信息，包括人脸位置，人脸特征
 */
class FaceInfo : public Face

/**
 * 人脸训练类
 * 一对多的关系，一个人脸训练类可以训练多个人脸
 */
class FaceRecognizer
```

## 项目进度
- 之前完成了大部分的代码开发，但是由于写的过于屎山，所以现在正在重构代码
- 国庆第一天：完成了基本框架的构思和日志系统，配置系统，线程系统以及部分人脸检测和文件系统的编写
- 国庆第二天：完成了数据库的连接以及数据库配置变动的回调，完成了人脸录入和训练的业务
- 国庆第三天：完成了图像信息参数的配置系统开发，完成了人脸训练以及人脸预测的业务逻辑
- 国庆第四天：完成了串口配置以及温度检测类的开发，完成了温度检测的业务逻辑以及串口通信的业务逻辑
- 国庆第五天：修复了之前的各种bug，使用多线程完成了主ui显示画面的同时，新建一个线程进行人脸录入和训练以及预测一条龙的过程。顺便改了改之前不合理的设计。为了使用ui所以需要将build文件夹一起上传。
- 国庆第六天：完成了mainWindow.ui的温度检测和开始签到的不需要与数据库进行交互的业务开发，修复了之前遇到的关于QThread的各种bug。
- 国庆第七天：因为要用到数据库了，所以今天重新写了一下数据库的整体结构，但是今天没写完，并且打算把之前所有配置项都重新设计一下，争取在一周内完成吧！

- 2023-10-13: 完成了数据库系统的编写，想要尝试使用多线程完成主界面的画面显示，预测逻辑和测温逻辑，但是不知道为何主界面的画面显示会导致预测逻辑的卡顿，希望未来版本可以修复这个bug