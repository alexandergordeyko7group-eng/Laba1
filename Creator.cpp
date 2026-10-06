#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "employee.h"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: Creator.exe file count\n";
        return 1;
    }

    int count;
    std::istringstream argument(argv[2]);
    if (!(argument >> count) || count <= 0 || !argument.eof())
    {
        std::cerr << "Invalid count.\n";
        return 1;
    }

    std::ofstream file(argv[1], std::ios::binary);
    if (!file)
    {
        std::cerr << "Cannot create file.\n";
        return 1;
    }

    for (int i = 0; i < count; ++i)
    {
        employee person{};
        std::string name;
        std::cout << "Employee " << i + 1 << " (number name hours): ";
        if (!(std::cin >> person.num >> name >> person.hours) ||
            name.size() > 9 || !std::isfinite(person.hours) || person.hours < 0)
        {
            std::cerr << "Invalid data. Name must contain at most 9 bytes.\n";
            return 1;
        }

        std::copy(name.begin(), name.end(), person.name);
        file.write(reinterpret_cast<const char*>(&person), sizeof(person));
        if (!file)
        {
            std::cerr << "Write error.\n";
            return 1;
        }
    }

    file.close();
    return file ? 0 : 1;
}