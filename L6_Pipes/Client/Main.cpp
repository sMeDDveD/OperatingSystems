#include <iostream>

#include "../Server/NamedPipe.h"
#include "../Server/Definitions.h"
#include "../Server/Employee.h"

#include "../Server/NamedPipe.cpp"

ControlMessage::Type GetInput()
{
    int c;
    std::cout << "Choose the option: " << std::endl;
    std::cout << "1. Read " << std::endl;
    std::cout << "2. Write " << std::endl;
    std::cout << "3. Close " << std::endl;
    std::cin >> c;

    if (c > 3 || c < 1)
    {
        std::cout << "Incorrect input" << std::endl;
        std::cout << "Try again" << std::endl;
        return GetInput();
    }

    return ControlMessage::Type(c - 1);
}

void Read(const NamedPipe& pipe)
{
    int id;
    std::cout << "Enter the ID: ";
    std::cin >> id;

    pipe.Write<ControlMessage>({ControlMessage::Type::READ, id});
    const auto answer = pipe.Read<EmployeeMessage>();
    if (answer.status == EmployeeMessage::Status::OK)
    {
        std::cout << "Employee: " << std::endl;
        std::cout << answer.employee << std::endl;
        std::cout << "End?" << std::endl;
        system("pause");
        pipe.Write<ControlMessage>(ControlMessage(ControlMessage::Type::CLOSE, {}));
    }
    else
    {
        std::cout << "Unable to read" << std::endl;
    }
}

void Write(const NamedPipe& pipe)
{
    int id;
    std::cout << "Enter the ID: ";
    std::cin >> id;

    pipe.Write<ControlMessage>({ControlMessage::Type::WRITE, id});
    const auto answer = pipe.Read<EmployeeMessage>();
    if (answer.status == EmployeeMessage::Status::OK)
    {
        std::cout << "Current employee: " << std::endl;
        std::cout << answer.employee << std::endl;

        const auto e = EmployeeIO::Read(std::cin, std::cout);
        pipe.Write<EmployeeMessage>({EmployeeMessage::Status::OK, e});
    }
    else
    {
        std::cout << "Unable to write" << std::endl;
    }
}

int main(int argc, char* argv[])
{
    std::cout << argv[1] << std::endl;
    NamedPipe pipe = NamedPipe::OpenPipe(argv[1]);
    std::cout << "Doing stuff..." << std::endl;
    while (true)
    {
        switch (auto t = GetInput())
        {
        case ControlMessage::Type::READ:
            {
                Read(pipe);
                break;
            }
        case ControlMessage::Type::WRITE:
            {
                Write(pipe);
                break;
            }
        case ControlMessage::Type::CLOSE:
            {
                std::cout << "Session ended" << std::endl;
                return 0;
            }
        }
    }
}
