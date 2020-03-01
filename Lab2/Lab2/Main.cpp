#include <iostream>
#include <string>
#include <fstream>

#include "Employee.h"
#include "Subprocess.h"

void PrintReport(const std::string& filename)
{
	std::cout << "Report:" << std::endl;
	std::cout << std::ifstream(filename).rdbuf() << std::endl;
}

void PrintBinary(const std::string& filename, size_t numberOfRecords)
{
	std::ifstream in(filename, std::ios::binary);
	std::cout << "From binary file:" << std::endl;

	for (size_t i = 0; i < numberOfRecords; i++)
	{
		Employee employee = EmployeeIO::Deserialize(in);
		std::cout << employee.num << " " << employee.name
			<< " " << employee.hours << std::endl;
	}
}


int main()
{
	std::string filename;
	std::string reportName;
	size_t numberOfRecords;
	double cost;

	std::cout << "Enter the name of the binary file:" << std::endl;
	std::cin >> filename;

	std::cout << "Enter the number of records: " << std::endl;
	std::cin >> numberOfRecords;

	Subprocess creator("Creator");
	creator.SetArgs(filename, numberOfRecords);
	try
	{
		creator.CreateSubprocess();
		PrintBinary(filename, numberOfRecords);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << "Enter the name of the report file:" << std::endl;
	std::cin >> reportName;

	std::cout << "Enter the salary per hour:" << std::endl;
	std::cin >> cost;

	Subprocess reporter("Reporter");
	reporter.SetArgs(filename, reportName, cost);
	try
	{
		reporter.CreateSubprocess();
		PrintReport(reportName);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	system("pause");
	return 0;
}
