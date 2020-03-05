#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../Lab2/Employee.h"
#include "../Lab2/Employee.cpp"

void MakeReport(
	const std::string& filename,
	const std::string& reportName,
    double costPerHour)
{
	std::ifstream input(filename, std::ios::binary);
	std::ofstream report(reportName);

	report << filename << " - report" << std::endl;

	while (!input.eof())
	{
		Employee current = EmployeeIO::Deserialize(input);
		if (input.gcount() == sizeof(Employee))
		{
			EmployeeIO::Write(current, report, costPerHour); // SOLID TODO
		}
	}
}

int main(int argc, char* argv[])
{
	try
	{
		double costPerHour;

		const std::string filename = argv[1];
		const std::string reportName = argv[2];
		std::stringstream(argv[3]) >> costPerHour;

		MakeReport(filename, reportName, costPerHour);
	}
	catch (const std::exception& e)
	{
		std::cout << "Something went wrong:" << e.what() << std::endl;
		return -1;
	}
	return 0;
}
