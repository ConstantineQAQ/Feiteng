#ifndef FILE_H
#define FILE_H

#include <memory>
#include <fstream>
#include <string>
#include <sstream>
#include "log.h"

namespace Feiteng
{

class File {
public:
    typedef std::shared_ptr<File> ptr;
    virtual void read() = 0;
    virtual void write() = 0;
    virtual void remove() = 0;
};

class txtFile : public std::enable_shared_from_this<txtFile>{
public:
    typedef std::shared_ptr<txtFile> ptr;
    void read(const std::string& filename,char separator = ';');
    void write() {}
    void remove() {}
    std::string getPath() const { return m_path; }
    bool pathEmpty() const {return m_path.empty();}
    std::string getLabel() const { return m_label; }
    bool labelEmpty() const {return m_label.empty();}
private:
    std::string m_path; // 人脸照片路径
    std::string m_label; // 人脸标签
};

class xmlFile {
public:
    typedef std::shared_ptr<xmlFile> ptr;
    void read();
    void write();
    void remove();
};
} // namespace Feiteng


#endif // FILE_H
