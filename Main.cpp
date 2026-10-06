#include <windows.h>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "employee.h"

bool runProcess(std::string command)
{
    STARTUPINFOA startup{};
    startup.cb = sizeof(startup);
    PROCESS_INFORMATION process{};

    if (!CreateProcessA(nullptr, command.data(), nullptr, nullptr, FALSE,
                        0, nullptr, nullptr, &startup, &process))
    {
        std::cerr << "Cannot start process. Error: " << GetLastError() << '\n';
        return false;
    }

    DWORD waitResult = WaitForSingleObject(process.hProcess, INFINITE);
    DWORD exitCode = 1;
    BOOL gotCode = GetExitCodeProcess(process.hProcess, &exitCode);
    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);

    if (waitResult != WAIT_OBJECT_0 || !gotCode || exitCode != 0)
    {
        std::cerr << "Child process failed.\n";
        return false;
    }
    return true;
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    std::string binaryName;
    int count;
    std::cout << "Binary file name: ";
    std::getline(std::cin >> std::ws, binaryName);
    std::cout << "Number of employees: ";
    if (!(std::cin >> count) || count <= 0 || binaryName.empty())
    {
        std::cerr << "Invalid input.\n";
        return 1;
    }

    std::string creatorCommand = "Creator.exe \"" + binaryName + "\" " + std::to_string(count);
    if (!runProcess(creatorCommand))
        return 1;

    std::ifstream binary(binaryName, std::ios::binary);
    if (!binary)
    {
        std::cerr << "Cannot open binary file.\n";
        return 1;
    }

    std::cout << "\nBinary file contents:\n";
    employee person{};
    while (binary.read(reinterpret_cast<char*>(&person), sizeof(person)))
        std::cout << person.num << ' ' << person.name << ' ' << person.hours << '\n';
    binary.close();

    std::string reportName;
    double rate;
    std::cout << "\nReport file name: ";
    std::getline(std::cin >> std::ws, reportName);
    std::cout << "Hourly rate: ";
    if (!(std::cin >> rate) || !std::isfinite(rate) || rate < 0 || reportName.empty())
    {
        std::cerr << "Invalid input.\n";
        return 1;
    }
    if (reportName == binaryName)
    {
        std::cerr << "Use a different file name for the report.\n";
        return 1;
    }

    std::ostringstream rateText;
    rateText << std::setprecision(17) << rate;
    std::string reporterCommand = "Reporter.exe \"" + binaryName + "\" \"" +
                                  reportName + "\" " + rateText.str();
    if (!runProcess(reporterCommand))
        return 1;

    std::ifstream report(reportName);
    if (!report)
    {
        std::cerr << "Cannot open report.\n";
        return 1;
    }
    std::cout << '\n' << report.rdbuf();
    return 0;
}