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
## 项目进度
- 之前完成了大部分的代码开发，但是由于写的过于屎山，所以现在正在重构代码
- 国庆第一天：完成了基本框架的构思和日志系统，配置系统，线程系统以及部分人脸检测和文件系统的编写
- 国庆第二天：完成了数据库的连接以及数据库配置变动的回调，完成了人脸录入和训练的业务