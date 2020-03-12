#include <iostream>
#include <vector>
#include "Thread.h"
#include "Functions.h"

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
		Thread::Data<std::vector<int>, std::pair<int, int>> minMaxData = {arr};
		Thread::Data<std::vector<int>, double> averageData = {arr};
		const Thread min_max(Functions::MinMax, &minMaxData);
		const Thread average(Functions::Average, &averageData);

		min_max.Start();
		average.Start();

		min_max.Join();
		average.Join();

		arr[minMaxData.ret.first] = arr[minMaxData.ret.second] = static_cast<int>(averageData.ret);

		for (auto now : arr)
		{
			std::cout << now << " ";
		}

		std::cout << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}

	system("pause");

	return 0;
}
