#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include "employee.h"

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: Reporter.exe binary_file report_file rate\n";
        return 1;
    }

    double rate;
    std::istringstream argument(argv[3]);
    if (!(argument >> rate) || !argument.eof() || !std::isfinite(rate) || rate < 0)
    {
        std::cerr << "Invalid hourly rate.\n";
        return 1;
    }

    std::ifstream input(argv[1], std::ios::binary);
    if (!input)
    {
        std::cerr << "Cannot open binary file.\n";
        return 1;
    }

    std::vector<employee> employees;
    employee person{};
    while (input.read(reinterpret_cast<char*>(&person), sizeof(person)))
    {
        if (person.name[9] != '\0' || !std::isfinite(person.hours) || person.hours < 0)
        {
            std::cerr << "Invalid record.\n";
            return 1;
        }
        employees.push_back(person);
    }
    if (!input.eof() || input.gcount() != 0)
    {
        std::cerr << "Read error or incomplete record.\n";
        return 1;
    }
    input.close();

    std::stable_sort(employees.begin(), employees.end(),
        [](const employee& a, const employee& b)
        {
            return a.num < b.num;
        });

    std::ofstream report(argv[2]);
    if (!report)
    {
        std::cerr << "Cannot create report.\n";
        return 1;
    }

    report << "Отчет по файлу «" << argv[1] << "»\n";
    report << "Номер сотрудника, имя сотрудника, часы, зарплата\n";
    report << std::fixed << std::setprecision(2);
    for (const employee& emp : employees)
        report << emp.num << ", " << emp.name << ", " << emp.hours
               << ", " << emp.hours * rate << '\n';

    report.close();
    return report ? 0 : 1;
}