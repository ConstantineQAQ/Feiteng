//
// Created by 王亚宁 on 2023/9/28.
//

#include "log.h"
#include "config.h"

namespace Feiteng {
    LogEvent::LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level, const char *file, int32_t line, uint32_t elapse,
                       uint32_t threadId, uint32_t fiberId, uint64_t time)
            :m_level(level)
            ,m_logger(logger)
            ,m_file(file)
            ,m_line(line)
            ,m_elapse(elapse)
            ,m_threadId(threadId)
            ,m_fiberId(fiberId)
            ,m_time(time)
    {
    }

    void LogEvent::format(const char* fmt, ...) {
        //va_start、va_end成对出现
        va_list al;  //val_list 用于获取不确定个数的参数
        va_start(al, fmt);
        format(fmt, al);
        va_end(al);
    }

/** 将内容都添加到m_ss中 */
    void LogEvent::format(const char* fmt, va_list al) {
        char* buf = nullptr;
        int len = vasprintf(&buf, fmt, al);
        if(len != -1) {
            m_ss << std::string(buf, len);
            free(buf);
        }
    }

    class MessageFormatItem : public LogFormatter::FormatItem{
    public:
        explicit MessageFormatItem(const std::string& str = ""){}
        void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
        {
            os << event->getContent();
        }
    };

    class LevelFormatItem : public LogFormatter::FormatItem{
    public:
        explicit LevelFormatItem(const std::string& str = ""){}
        void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
        {
            os << LogLevel::ToString(level);
        }
    };

    class ElapseFormatItem : public LogFormatter::FormatItem{
    public:
        explicit ElapseFormatItem(const std::string& str = ""){}
        void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
        {
            os << event->getElapse();
        }
    };

    class NameFormatItem : public LogFormatter::FormatItem{
    public:
        explicit NameFormatItem(const std::string& str = ""){}
        void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
        {
            os << event->getLogger()->getName();
        }
    };

    class ThreadIdFormatItem : public LogFormatter::FormatItem{
    public:
        explicit ThreadIdFormatItem(const std::string& str = ""){}
        void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
        {
            os << event->getThreadId();
        }
    };

    class FiberIdFormatItem : public LogFormatter::FormatItem{
    public:
        explicit FiberIdFormatItem(const std::string& str = ""){}
        void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
        {
            os << event->getFiberId();
        }
    };

/** [%d] 时间 */
    class DateTimeFormatItem : public LogFormatter::FormatItem {
    public:
        explicit DateTimeFormatItem(const std::string& format = "%Y-%m-%d %H:%M:%S")
                :m_format(format) {
            if(m_format.empty()) {
                m_format = "%Y-%m-%d %H:%M:%S";
            }
        }

        void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
            struct tm tm;
            time_t time = event->getTime();
            //localtime_r 用于linux平台下获取系统时间
            localtime_r(&time, &tm);
            char buf[64];
            //strftime 根据format中定义的格式化规则，格式化结构tm表示的时间，并把它存储在buf中。
            strftime(buf, sizeof(buf), m_format.c_str(), &tm);
            os << buf;
        }
    private:
        std::string m_format;
    };

    class FilenameFormatItem : public LogFormatter::FormatItem{
    public:
        explicit FilenameFormatItem(const std::string& str = ""){}
        void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
        {
            os << event->getFile();
        }
    };

    class LineFormatItem : public LogFormatter::FormatItem{
    public:
        explicit LineFormatItem(const std::string& str = ""){}
        void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
        {
            os << event->getLine();
        }
    };

    class NewLineFormatItem : public LogFormatter::FormatItem{
    public:
        explicit NewLineFormatItem(const std::string& str = ""){}
        void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
        {
            os << std::endl;
        }
    };

    class StringFormatItem : public LogFormatter::FormatItem{
    public:
        explicit StringFormatItem(const std::string& str) : m_string(str){}
        void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
        {
            os << m_string;
        }
    private:
        std::string m_string;
    };

    class TabFormatItem : public LogFormatter::FormatItem{
    public:
        explicit TabFormatItem(const std::string& str = ""){}
        void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
        {
            os << "\t";
        }
    };

    const char* LogLevel::ToString(LogLevel::Level level) {
        switch(level) {
#define XX(name) \
    case LogLevel::name: \
        return #name;\
        break;

            XX(DEBUG);
            XX(INFO);
            XX(WARN);
            XX(ERROR);
            XX(FATAL);
#undef XX
            default:
                return "UNKNOW";
        }
        return "UNKNOW";
    }

    LogLevel::Level LogLevel::FromString(const std::string &str)
    {
#define XX(level, v) \
    if(str == #v) { \
        return LogLevel::level; \
    }

        XX(DEBUG, debug);
        XX(INFO, info);
        XX(WARN, warn);
        XX(ERROR, error);
        XX(FATAL, fatal);

        XX(DEBUG, DEBUG);
        XX(INFO, INFO);
        XX(WARN, WARN);
        XX(ERROR, ERROR);
        XX(FATAL, FATAL);
#undef XX
        return LogLevel::UNKNOW;
    }

    // %m -- 消息体
    // %p -- level
    // %r -- 启动后的时间
    // %c -- 日志名称
    // %t -- 线程id
    // %n -- 回车换行
    // %d -- 时间
    // %f -- 文件名
    // %l -- 行号
    // %T -- 制表符
    // %F -- 协程id

    Logger::Logger(const std::string &name):m_name(name),m_level(LogLevel::DEBUG) {
        m_formatter.reset(new LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
    }

    void Logger::setFormatter(LogFormatter::ptr val) {
        MutexType::Lock lock(m_mutex);
        m_formatter = val;

        for(auto& i : m_appenders) {
            MutexType::Lock ll(i->m_mutex);
            if(!i->m_hasFormatter) {
                i->m_formatter = m_formatter;
            }
        }
    }
    void Logger::setFormatter(const std::string& val) {
        Feiteng::LogFormatter::ptr new_val(new Feiteng::LogFormatter(val));
        if(new_val->isError()) {
            std::cout << "Logger setFormatter name=" << m_name
                      << " value=" << val << " invalid formatter" << std::endl;
            return;
        }
        //m_formatter = new_val;
        setFormatter(new_val);
    }

    LogFormatter::ptr Logger::getFormatter(){
        return m_formatter;
    }

    void Logger::Log(LogLevel::Level level, LogEvent::ptr event)
    {
        if(level >= m_level)
        {
            auto self = shared_from_this();
            MutexType::Lock lock(m_mutex);
            if(!m_appenders.empty()){
                for(auto& i : m_appenders)
                {
                    i->Log(self, level, event);
                }
            }else if(m_root) {
                m_root->Log(level,event);
            }
        }
    }

    std::string Logger::toYamlString()
    {
        MutexType::Lock lock(m_mutex);
        YAML::Node node;
        node["name"] = m_name;
        if(m_level != LogLevel::UNKNOW) {
            node["level"] = LogLevel::ToString(m_level);
        }
        if(m_formatter) {
            node["formatter"] = m_formatter->getPattern();
        }

        for(auto& i : m_appenders) {
            node["appenders"].push_back(YAML::Load(i->toYamlString()));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }

    void Logger::Debug(LogEvent::ptr event)
    {
        Log(LogLevel::Level::DEBUG, event);
    }
    void Logger::Info(LogEvent::ptr event)
    {
        Log(LogLevel::Level::INFO, event);
    }
    void Logger::Warn(LogEvent::ptr event)
    {
        Log(LogLevel::Level::WARN, event);
    }
    void Logger::Error(LogEvent::ptr event)
    {
        Log(LogLevel::Level::ERROR, event);
    }
    void Logger::Fatal(LogEvent::ptr event)
    {
        Log(LogLevel::Level::FATAL, event);
    }
    void Logger::addAppender(LogAppender::ptr appender)
    {
        MutexType::Lock lock(m_mutex);
        if(!appender->getFormatter()) {
            MutexType::Lock ll(appender->m_mutex);
            appender->m_formatter = m_formatter;
        }
        m_appenders.push_back(appender);
    }
    void Logger::delAppender(LogAppender::ptr appender)
    {
        MutexType::Lock lock(m_mutex);
        for(auto it = m_appenders.begin(); it != m_appenders.end(); ++it)
        {
            if(*it == appender)
            {
                m_appenders.erase(it);
                break;
            }
        }
    }

    void Logger::clearAppenders()
    {
        MutexType::Lock lock(m_mutex);
        m_appenders.clear();
    }

    FileLogAppender::FileLogAppender(const std::string &filename) : m_filename(filename){
        reopen(); // 打开文件
    }

    void FileLogAppender::Log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)
    {
        if(level >= m_level)
        {
            uint64_t now = event->getTime();
            if(now >= (m_lastTime + 3)) {
                reopen();
                m_lastTime = now;
            }
            MutexType::Lock lock(m_mutex);
            if (!(m_filestream << m_formatter->format(logger, level, event))) {
                std::cout << "error" << std::endl;
            }
        }
    }

    bool FileLogAppender::reopen()
    {
        MutexType::Lock lock(m_mutex);
        if(m_filestream) {
            m_filestream.close();
        }
        // std::ios_base::openmode app 每次进行写入操作的时候都会重新定位到文件的末尾
        m_filestream.open(m_filename.c_str(), std::ios::app);
        return m_filestream.is_open();
    }

    std::string FileLogAppender::toYamlString()
    {
        MutexType::Lock lock(m_mutex);
        YAML::Node node;
        node["type"] = "FileLogAppender";
        node["file"] = m_filename;
        if(m_level != LogLevel::UNKNOW) {
            node["level"] = LogLevel::ToString(m_level);
        }
        if(m_formatter && m_hasFormatter) {
            node["formatter"] = m_formatter->getPattern();
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }

    void StdoutLogAppender::Log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)
    {
        if(level >= m_level)
        {
            MutexType::Lock lock(m_mutex);
            std::cout << m_formatter->format(logger, level, event);
        }
    }

    std::string StdoutLogAppender::toYamlString()
    {
        MutexType::Lock lock(m_mutex);
        YAML::Node node;
        node["type"] = "StdoutLogAppender";
        if(m_level != LogLevel::UNKNOW) {
            node["level"] = LogLevel::ToString(m_level);
        }
        if(m_formatter && m_hasFormatter) {
            node["formatter"] = m_formatter->getPattern();
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }

    LogFormatter::LogFormatter(const std::string &pattern) : m_pattern(pattern){
        init();
    }

    void LogAppender::setFormatter(LogFormatter::ptr val) {
        MutexType::Lock lock(m_mutex);
        m_formatter = val;
        if(m_formatter) {
            m_hasFormatter = true;
        } else {
            m_hasFormatter = false;
        }
    }

    LogFormatter::ptr LogAppender::getFormatter() {
        MutexType::Lock lock(m_mutex);
        return m_formatter;
    }

    std::string LogFormatter::format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event){
        std::stringstream ss;
        for(auto& i : m_items)
        {
            i->format(ss, logger, level, event);
        }
        return ss.str();
    }

// %xxx %xxx{xxx} %%
// "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
// 解析日志格式
    void LogFormatter::init() {
        //str, format, type
        std::vector<std::tuple<std::string, std::string, int> > vec;
        std::string nstr;
        for (size_t i = 0; i < m_pattern.size(); ++i) {
            if (m_pattern[i] != '%') {
                nstr.append(1, m_pattern[i]);
                continue;
            }

            if ((i + 1) < m_pattern.size()) {
                if (m_pattern[i + 1] == '%') {
                    nstr.append(1, '%');
                    continue;
                }
            }

            size_t n = i + 1;
            int fmt_status = 0;
            size_t fmt_begin = 0;

            std::string str;
            std::string fmt;
            while (n < m_pattern.size()) {
                if (!fmt_status && (!isalpha(m_pattern[n]) && m_pattern[n] != '{'
                                    && m_pattern[n] != '}')) {
                    str = m_pattern.substr(i + 1, n - i - 1);
                    break;
                }
                if (fmt_status == 0) {
                    if (m_pattern[n] == '{') {
                        str = m_pattern.substr(i + 1, n - i - 1);
                        fmt_status = 1; //解析格式
                        fmt_begin = n;
                        ++n;
                        continue;
                    }
                }
                else if (fmt_status == 1) {
                    if (m_pattern[n] == '}') {
                        fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin - 1);
                        fmt_status = 0;
                        ++n;
                        break;
                    }
                }
                ++n;
                if (n == m_pattern.size()) {
                    if (str.empty()) {
                        str = m_pattern.substr(i + 1);
                    }
                }
            }

            if (fmt_status == 0) {
                if (!nstr.empty()) {
                    vec.push_back(std::make_tuple(nstr, std::string(), 0));
                    nstr.clear();
                }
                vec.push_back(std::make_tuple(str, fmt, 1));
                i = n - 1;
            }
            else if (fmt_status == 1) {
                std::cout << "pattern parse error: " << m_pattern << " - " << m_pattern.substr(i) << std::endl;
                m_error = true;
                vec.push_back(std::make_tuple("<<pattern_error>>", fmt, 0));
            }
        }

        if (!nstr.empty()) {
            vec.push_back(std::make_tuple(nstr, "", 0));
        }
        static std::map<std::string, std::function<FormatItem::ptr(const std::string& str)> > s_format_items = {
#define XX(str, C) \
        {#str, [](const std::string& fmt) { return FormatItem::ptr(new C(fmt));}}

                XX(m, MessageFormatItem),           //m:消息
                XX(p, LevelFormatItem),             //p:日志级别
                XX(r, ElapseFormatItem),            //r:累计毫秒数
                XX(c, NameFormatItem),              //c:日志名称
                XX(t, ThreadIdFormatItem),          //t:线程id
                XX(n, NewLineFormatItem),           //n:换行
                XX(d, DateTimeFormatItem),          //d:时间
                XX(f, FilenameFormatItem),          //f:文件名
                XX(l, LineFormatItem),              //l:行号
                XX(T, TabFormatItem),               //T:Tab
                XX(F, FiberIdFormatItem),           //F:协程id
#undef XX
        };

        for (auto& i : vec) {
            if (std::get<2>(i) == 0) {
                m_items.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
            }
            else {
                auto it = s_format_items.find(std::get<0>(i));
                if (it == s_format_items.end()) {
                    m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
                    m_error = true;
                }
                else {
                    m_items.push_back(it->second(std::get<1>(i)));
                }
            }
        }
    }

    LogEventWrap::LogEventWrap(LogEvent::ptr e) : m_event(e){
    }

    LogEventWrap::~LogEventWrap()
    {
        m_event->getLogger()->Log(m_event->getLevel(), m_event);
    }
    std::stringstream &LogEventWrap::getSS()
    {
        return m_event->getSS();
    }
    LoggerManager::LoggerManager(){
        m_root.reset(new Logger);
        m_root->addAppender(LogAppender::ptr(new StdoutLogAppender));
        m_loggers[m_root->getName()] = m_root;
        init();
    }

    std::string LoggerManager::toYamlString()
    {
        MutexType::Lock lock(m_mutex);
        YAML::Node node;
        for(auto& i : m_loggers)
        {
            node.push_back(YAML::Load(i.second->toYamlString()));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }

    Logger::ptr LoggerManager::getLogger(const std::string &name)
    {
        MutexType::Lock lock(m_mutex);
        auto it = m_loggers.find(name);
        if(it != m_loggers.end()) {
            return it->second;
        }
        Logger::ptr logger(new Logger(name));
        logger->m_root = m_root;
        m_loggers[name] = logger;
        return logger;
    }

    struct LogAppenderDefine {
        int type = 0; // 1 File, 2 Stdout
        LogLevel::Level level = LogLevel::UNKNOW;
        std::string formatter;
        std::string file;

        bool operator==(const LogAppenderDefine oth) const {
            return type == oth.type
                   && level == oth.level
                   && formatter == oth.formatter
                   && file == oth.file;
        }
    };

    struct LogDefine {
        std::string name;
        LogLevel::Level level = LogLevel::UNKNOW;
        std::string formatter;
        std::vector<LogAppenderDefine> appenders;

        bool operator==(const LogDefine& oth) const {
            return name == oth.name
                   && level == oth.level
                   && formatter == oth.formatter
                   && appenders == oth.appenders;
        }

        bool operator<(const LogDefine& oth) const {
            return name < oth.name;
        }
    };

    template<>
    class LexicalCast<std::string, std::set<LogDefine>> {
    public:
        std::set<LogDefine> operator()(const std::string& v) {
            YAML::Node node = YAML::Load(v);
            std::set<LogDefine> vec;
            for(size_t i = 0; i < node.size(); ++i) {
                auto n = node[i];
                if(!n["name"].IsDefined()) {
                    std::cout << "log config error: name is null, " << n
                              << std::endl;
                    continue;
                }
                LogDefine ld;
                ld.name = n["name"].as<std::string>();
                ld.level = LogLevel::FromString(n["level"].IsDefined() ? n["level"].as<std::string>() : "");
                if(n["formatter"].IsDefined()) {
                    ld.formatter = n["formatter"].as<std::string>();
                }
                if(n["appenders"].IsDefined()) {
                    for(size_t x = 0; x < n["appenders"].size(); ++x) {
                        auto a = n["appenders"][x];
                        if(!a["type"].IsDefined()) {
                            std::cout << "log config error: appender type is null, " << a
                                      << std::endl;
                            continue;
                        }
                        std::string type = a["type"].as<std::string>();
                        LogAppenderDefine lad;
                        if(type == "FileLogAppender") {
                            lad.type = 1;
                            if(!a["file"].IsDefined()) {
                                std::cout << "log config error: fileappender file is null, " << a
                                          << std::endl;
                                continue;
                            }
                            lad.file = a["file"].as<std::string>();
                            if(a["formatter"].IsDefined()) {
                                lad.formatter = a["formatter"].as<std::string>();
                            }
                        } else if(type == "StdoutLogAppender") {
                            lad.type = 2;
                        } else {
                            std::cout << "log config error: appender type is invalid, " << a
                                      << std::endl;
                            continue;
                        }
                        ld.appenders.push_back(lad);
                    }
                }
                vec.insert(ld);
            }
            return vec;
        }
    };

    template<>
    class LexicalCast<std::set<LogDefine>, std::string> {
    public:
        std::string operator()(const std::set<LogDefine>& v) {
            YAML::Node node;
            for(auto& i : v) {
                YAML::Node n;
                n["name"] = i.name;
                if(i.level != LogLevel::UNKNOW) {
                    n["level"] = LogLevel::ToString(i.level);
                }

                if(!i.formatter.empty()) {
                    n["formatter"] = i.formatter;
                }

                for(auto& a : i.appenders) {
                    YAML::Node na;
                    if(a.type == 1) {
                        na["type"] = "FileLogAppender";
                        na["file"] = a.file;
                    } else if(a.type == 2) {
                        na["type"] = "StdoutLogAppender";
                    }
                    if(a.level != LogLevel::UNKNOW) {
                        na["level"] = LogLevel::ToString(a.level);
                    }

                    if(!a.formatter.empty()) {
                        na["formatter"] = a.formatter;
                    }

                    n["appenders"].push_back(na);
                }
                node.push_back(n);
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };

    Feiteng::ConfigVar<std::set<LogDefine>>::ptr g_log_defines =
            Feiteng::Config::Lookup("logs", std::set<LogDefine>(), "logs config");

    struct LogIniter {
        LogIniter() {
            g_log_defines->addListener([](const std::set<LogDefine>& old_value,
                                          const std::set<LogDefine>& new_value){
                FEITENG_LOG_INFO(FEITENG_LOG_ROOT()) << "on_logger_conf_changed";
                for(auto& i : new_value) {
                    auto it = old_value.find(i);
                    Feiteng::Logger::ptr logger;
                    if(it == old_value.end()) {
                        //新增logger
                        logger = FEITENG_LOG_NAME(i.name);
                    } else {
                        if(!(i == *it)) {
                            //修改的logger
                            logger = FEITENG_LOG_NAME(i.name);
                        } else {
                            continue;
                        }
                    }
                    logger->setLevel(i.level);
                    //std::cout << "** " << i.name << " level=" << i.level
                    //<< "  " << logger << std::endl;
                    if(!i.formatter.empty()) {
                        logger->setFormatter(i.formatter);
                    }

                    logger->clearAppenders();
                    for(auto& a : i.appenders) {
                        Feiteng::LogAppender::ptr ap;
                        if(a.type == 1) {
                            ap.reset(new FileLogAppender(a.file));
                        } else if(a.type == 2) {
                            ap.reset(new StdoutLogAppender);
                        }
                        ap->setLevel(a.level);
                        if(!a.formatter.empty()) {
                            LogFormatter::ptr fmt(new LogFormatter(a.formatter));
                            if(!fmt->isError()) {
                                ap->setFormatter(fmt);
                            } else {
                                std::cout << "log.name=" << i.name << " appender type=" << a.type
                                          << " formatter=" << a.formatter << " is invalid" << std::endl;
                            }
                        }
                        logger->addAppender(ap);
                    }
                }

                for(auto& i : old_value) {
                    auto it = new_value.find(i);
                    if(it == new_value.end()) {
                        //删除logger
                        auto logger = FEITENG_LOG_NAME(i.name);
                        logger->setLevel((LogLevel::Level)0);
                        logger->clearAppenders();
                    }
                }
            });
        }
    };

    static LogIniter __log_init;

    void LoggerManager::init(){

    }
}