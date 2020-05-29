#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <condition_variable>
#include <mutex>

#include "Employee.h"
#include "Definitions.h"
#include "Subprocess.h"
#include "NamedPipe.h"
#include "Record.h"
#include "Thread.h"
#include "Utilities.h"

std::unordered_map<int, Record> mapper;
std::string filename;

std::mutex m;
std::condition_variable cv;

EmployeeMessage CreateAnswerMessageByID(int id)
{
    EmployeeMessage sendMsg{};
    std::ifstream input(filename);
    const int offset = mapper[id].GetOffset();
    input.seekg(offset);

    sendMsg.employee = EmployeeIO::Deserialize(input);
    sendMsg.status = EmployeeMessage::Status::OK;

    input.close();
    return sendMsg;
}

DWORD _stdcall ClientHandler(LPVOID params)
{
    const auto pipe = static_cast<NamedPipe*>(params);
    ControlMessage recievedMsg{};
    EmployeeMessage sendMsg{};
    while (true)
    {
        try
        {
            recievedMsg = pipe->Read<ControlMessage>();
        }
        catch (const std::exception&)
        {
            return -2;
        }

        int id = recievedMsg.id;
        if (recievedMsg.type == ControlMessage::Type::READ)
        {
            {
                std::unique_lock<std::mutex> lk(m);
                cv.wait(lk, [id] { return mapper[id].IsReadable(); });
            }
            if (mapper[id].IsReadable())
            {
                {
                    std::lock_guard<std::mutex> lock(m);
                    mapper[id].SetState(Record::Status::READ);
                    mapper[id].AddReader();
                }
                sendMsg = CreateAnswerMessageByID(id);
                pipe->Write<EmployeeMessage>(sendMsg);
                if (pipe->Read<ControlMessage>().type == ControlMessage::Type::CLOSE)
                {
                    {
                        std::lock_guard<std::mutex> lock(m);
                        mapper[id].ReleaseReader();
                    }
                }
            }
            else
            {
                pipe->Write<EmployeeMessage>(EmployeeMessage(EmployeeMessage::Status::FAILED, {}));
            }
        }
        else if (recievedMsg.type == ControlMessage::Type::WRITE)
        {
            m.lock();
            if (mapper[id].IsWritable())
            {
                mapper[id].SetState(Record::Status::WRITE);
                m.unlock();

                const int offset = mapper[id].GetOffset();
                sendMsg = CreateAnswerMessageByID(id);
                pipe->Write<EmployeeMessage>(sendMsg);

                auto answerMessage = pipe->Read<EmployeeMessage>();
                mapper.erase(id);
                mapper[answerMessage.employee.num] = Record(offset, Record::Status::WRITE);

                std::ofstream output(filename);
                output.seekp(offset);
                EmployeeIO::Serialize(answerMessage.employee, output);
                output.close();

                mapper[answerMessage.employee.num].SetState(Record::Status::FREE);
                cv.notify_all();
            }
            else
            {
                m.unlock();
                pipe->Write<EmployeeMessage>(EmployeeMessage(EmployeeMessage::Status::FAILED, {}));
            }
        }
        else if (recievedMsg.type == ControlMessage::Type::CLOSE)
        {
            return pipe->Disconnect();
        }
    }
}

int main()
{
    std::cout << "I am ready!" << std::endl;

    std::cout << "Enter name of the output file: " << std::endl;
    std::cin >> filename;

    int numberOfRecords;
    std::cout << "Enter number of records: " << std::endl;
    std::cin >> numberOfRecords;


    std::cout << "Enter info about employees: " << std::endl;

    std::ofstream outFile(filename, std::ios::binary);
    for (int i = 0; i < numberOfRecords; ++i)
    {
        const Employee e = EmployeeIO::Read(std::cin, std::cout);
        mapper[e.num] = Record(outFile.tellp());
        EmployeeIO::Serialize(e, outFile);
    }
    outFile.close();

    Utils::PrintBinary(filename, numberOfRecords);

    int numberOfClients;
    std::cout << "Enter number of clients: " << std::endl;
    std::cin >> numberOfClients;

    std::vector<Thread> clientsThreads;
    std::vector<NamedPipe> pipes(numberOfClients);
    for (int i = 0; i < numberOfClients; ++i)
    {

        pipes[i] = NamedPipe(Constants::pipeName, sizeof(EmployeeMessage));

        Subprocess client("Client");
        client.SetArgs(Constants::pipeName);
        client.CreateSubprocess(false);

        if (pipes[i].Connect())
        {
            Thread thread(ClientHandler, &pipes[i]);
            thread.Start();

            clientsThreads.push_back(std::move(thread));
        }
        else
        {
            std::cout << "Unable to connect pipe: " << i << std::endl;
        }
    }

    Utils::WaitForClients(clientsThreads);
    std::cout << std::endl << "Finale: " << std::endl;
    Utils::PrintBinary(filename, numberOfRecords);
    return 0;
}
