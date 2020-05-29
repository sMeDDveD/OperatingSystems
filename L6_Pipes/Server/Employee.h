#pragma once
#include <iostream>
#include <fstream>
#include <exception>

struct Employee
{
    int num;
    char name[10];
    double hours;
};

inline std::ostream& operator<<(std::ostream& out, const Employee& e)
{
    out << "[ID: " << e.num << ", Name: " << e.name <<
        ", Hours: " << e.hours << "]" << std::endl;
    return out;
}

namespace EmployeeIO
{
    inline bool Serialize(const Employee& e, std::ofstream& out)
    {
        try
        {
            out.write(reinterpret_cast<const char*>(&e), sizeof(Employee));
        }
        catch (const std::exception&)
        {
            return false;
        }
        return true;
    }

    inline Employee Deserialize(std::ifstream& input)
    {
        Employee employee{};
        input.read(reinterpret_cast<char*>(&employee), sizeof(Employee));
        return employee;
    }

    inline Employee Read(std::istream& input, std::ostream& output)
    {
        Employee employee{};

        output << "Enter Employee's ID: " << std::endl;
        input >> employee.num;

        output << "Enter Employee's name: " << std::endl;
        input >> employee.name;

        output << "Enter Employee's hours: " << std::endl;
        input >> employee.hours;

        return employee;
    }
}
