#include "Functions.h"

namespace Functions {

	DWORD WINAPI MinMax(LPVOID lpParam)
	{
		auto data = static_cast<Thread::Data<std::vector<int>, std::pair<int, int>>*>(lpParam);

		int min = data->arg[0];
		int max = min;

		int iMin = 0;
		int iMax = iMin;

		for (int i = 0; i < data->arg.size(); i++)
		{
			const int now = data->arg[i];
			if (min > now)
			{
				min = now;
				iMin = i;
			}
			else
			{
				if (max < now)
				{
					max = now;
					iMax = i;
				}
			}

			Sleep(7);
		}

		std::cout << min << " " << max << std::endl;

		data->ret = { iMin, iMax };

		return 0;
	}

	DWORD WINAPI Average(LPVOID lpParam)
	{
		auto data = static_cast<Thread::Data<std::vector<int>, double>*>(lpParam);

		int s = 0;
		for (auto now : data->arg)
		{
			s += now;
			Sleep(12);
		}

		const double result = static_cast<double>(s) / data->arg.size();

		std::cout << result << std::endl;

		data->ret = result;

		return 0;
	}
}