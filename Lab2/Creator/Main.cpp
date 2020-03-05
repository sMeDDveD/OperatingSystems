#include <string>
#include <sstream>
#include <fstream>
#include <array>

#include "../Lab2/Employee.h"
#include "../Lab2/Employee.cpp"


int main(int argc, char* argv[])
{
	try
	{
		size_t numberOfRecords;

		const std::string filename = argv[1];
		std::stringstream(argv[2]) >> numberOfRecords;

		std::ofstream out(filename, std::ios::binary);

		for (size_t i = 0; i < numberOfRecords; ++i)
		{
			Employee employee = EmployeeIO::Read(std::cin, std::cout);
			EmployeeIO::Serialize(employee, out);
		}

	}
	catch (const std::exception& e)
	{
		std::cout << "Something went wrong: " << e.what() << std::endl;
	}
	return 0;
}
