#include "file.h"

namespace Feiteng {

static Feiteng::Logger::ptr g_logger = FEITENG_LOG_NAME("file");

void txtFile::read(const std::string &filename, char separator)
{
    std::ifstream file(filename, std::ifstream::in);
    if (!file) {
        FEITENG_LOG_ERROR(g_logger) << "输入的文件名非法,请检查所给文件名";
    }
    std::string line, path, label;
    std::stringstream liness(line);
    while(getline(file,line)) {
        getline(liness, path, separator);
        getline(liness, label);
    }
}

}
