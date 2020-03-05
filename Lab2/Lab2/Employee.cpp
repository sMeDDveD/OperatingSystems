#include "Employee.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

namespace EmployeeIO {
    void Write(const Employee &employee, std::ofstream &output, double salaryPerHour) {
        output << employee.num << " " << employee.name << " " << employee.hours << " "
               << employee.hours * salaryPerHour << std::endl;
    }

    Employee Read(std::istream &input, std::ostream &output) {
        std::string name;
        int num;
        double hours;

        output << "Enter employee's ID: " << std::endl;
        input >> num;

        output << "Enter employee's name: " << std::endl;
        input >> name;

        output << "Enter employee's hours: " << std::endl;
        input >> hours;


        Employee e{};

        strcpy(e.name, name.c_str());
        e.hours = hours;
        e.num = num;
        return e;
    }

}
