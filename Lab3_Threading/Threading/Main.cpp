#include <iostream>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include "Thread.h"

DWORD WINAPI PrintMinMax(LPVOID lpParam)
{
	const auto arr = static_cast<std::vector<int>*>(lpParam);

	int min = (*arr)[0];
	int max = min;

	for (auto now : *arr)
	{
		if (min > now)
		{
			min = now;
		}
		else
		{
			if (max < now)
			{
				max = now;
			}
		}

		Sleep(7);
	}

	std::cout << min << " " << max << std::endl;

	return 0;
}

DWORD WINAPI PrintAverage(LPVOID lpParam)
{
	const auto arr = static_cast<std::vector<int>*>(lpParam);

	int s = 0;
	for (auto now : *arr)
	{
		s += now;
		Sleep(12);
	}

	return s / arr->size();
}

std::vector<int> GetArray()
{
	int n;

	std::cout << "Enter array length: " << std::endl;
	std::cin >> n;

	std::vector<int> arr(n);
	std::cout << "Enter elements: " << std::endl;

	for (auto& now : arr)
	{
		std::cin >> now;
	}

	return arr;
}


int main()
{
	auto arr = GetArray();

	try
	{
		const Thread min_max(PrintMinMax, &arr);
		const Thread average(PrintAverage, &arr);

		min_max.Start();
		average.Start();

		min_max.Join();
		average.Join();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}

	system("pause");

	return 0;
}
