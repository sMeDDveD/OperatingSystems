#include <iostream>
#include <iterator>
#include <vector>
#include "Thread.h"
#include "Functions.h"

std::vector<int> GetArray()
{
	std::vector<int> arr;
	
	std::cout << "Enter elements: " << std::endl;
	std::copy(std::istream_iterator<int>(std::cin), std::istream_iterator<int>(),
		std::inserter(arr, arr.end()));

	return arr;
}

int main()
{
	auto arr = GetArray();

	try
	{
		Thread::Data<const std::vector<int>&, std::pair<int, int>> minMaxData = {arr};
		Thread::Data<const std::vector<int>&, double> averageData = {arr};
		const Thread min_max(Functions::MinMax, &minMaxData);
		const Thread average(Functions::Average, &averageData);

		min_max.Start();
		average.Start();

		min_max.Join();
		average.Join();

		arr[minMaxData.ret.first] = arr[minMaxData.ret.second] = static_cast<int>(averageData.ret);

		std::copy(arr.cbegin(), arr.cend(),
			std::ostream_iterator<int>(std::cout, " "));

		std::cout << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}

	system("pause");

	return 0;
}