#ifndef UTIL_H
#define UTIL_H

#include <sys/syscall.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <vector>
#include <string>

namespace Feiteng {
    // 获取Linux系统下的线程id
    pid_t GetThreadId();
    // 获取当前协程id
    uint32_t GetFiberId();
    void Backtrace(std::vector<std::string>& bt, int size = 64, int skip = 1);
    std::string BacktraceToString(int size = 64, int skip = 2, const std::string& prefix = "");
}

#endif // UTIL_H