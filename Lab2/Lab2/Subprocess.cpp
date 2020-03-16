#include "Subprocess.h"


#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include <wait.h>

Subprocess::Subprocess(std::string processName) : processName(std::move(processName)) {
}

void Subprocess::SetArrayArgs(const std::initializer_list<std::string> &list) {
    argv.push_back(const_cast<char * >(("./" + processName).c_str()));
    for (const auto &arg : list) {
        argv.push_back(const_cast<char *>(arg.c_str()));
    }
    // std::transform(list.begin(), list.end(), std::back_inserter(argv), mem_fn(&std::string::c_str)); TODO
    argv.push_back(nullptr);
}

void Subprocess::CreateSubprocess(bool waiting) {
    pid_t pid = fork();

    if (pid == -1) {
        throw std::runtime_error("Unable to fork");
    } else {
        if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);

            if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
                throw std::runtime_error(processName + ": error");
            }
        } else {
            if (execv(processName.c_str(), argv.data()) == -1) {
                throw std::runtime_error("Unable to exec");
            }
            exit(0);
        }
    }
}


std::string Subprocess::GetErrorDescription() {
    return "Something went wrong";
}
