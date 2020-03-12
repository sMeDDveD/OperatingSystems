#pragma once

#include <Windows.h>
#include "Thread.h"
#include <iostream>
#include <vector>

namespace Functions {

	DWORD WINAPI MinMax(LPVOID lpParam);
	DWORD WINAPI Average(LPVOID lpParam);

}
