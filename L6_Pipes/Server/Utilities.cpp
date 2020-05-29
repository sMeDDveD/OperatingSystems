#include "Utilities.h"


#include <algorithm>
#include <iostream>

#include "Employee.h"

namespace Utils {
	void PrintFile(const std::string& filename)
	{
		std::cout << "Employees" << std::endl;
		std::cout << std::ifstream(filename).rdbuf() << std::endl;
	}

	void PrintBinary(const std::string& filename, int numberOfRecords)
	{
		std::ifstream in(filename, std::ios::binary);
		std::cout << "From binary file: " << std::endl;

		for (int i = 0; i < numberOfRecords; ++i)
		{
			Employee employee = EmployeeIO::Deserialize(in);
			std::cout << employee << std::endl;
		}
	}

	void WaitForClients(const std::vector<Thread>& clients, bool all)
	{
		const int n = clients.size();
		std::vector<HANDLE> handles(n);

		std::transform(clients.begin(), clients.end(),
			handles.begin(), [](const Thread& e)
			{
				return e.GetHandle();
			});

		WaitForMultipleObjects(n, handles.data(), all, INFINITE);
	}
}