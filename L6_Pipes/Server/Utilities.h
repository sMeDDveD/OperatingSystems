#pragma once
#include <string>
#include <vector>


#include "Thread.h"

namespace Utils
{
    void PrintFile(const std::string& filename);
    void PrintBinary(const std::string& filename, int numberOfRecords);
    void WaitForClients(const std::vector<Thread>& clients, bool all = true);
}
