#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <iomanip>
#include <limits>
#include <cassert>


namespace settings
{
    const char *inputFile = "./input.txt";
    constexpr bool debug_on = false;

}

namespace debug
{
    struct _log
    {
        template <typename T>
        friend _log& operator<<(_log& out, const T& t)
        {
            if constexpr (settings::debug_on)
                std::cout << t;
            return out;
        }
    };
    static _log log;
}


int median(const std::vector<int>& values)
{
    auto values_copy{values};
    std::sort(std::begin(values_copy), std::end(values_copy));
    if (values_copy.size() % 2 != 0)
    {
        return values_copy.at(values_copy.size() / 2);
    }
    else
    {
        auto sum = values_copy.at(values_copy.size() / 2) + values_copy.at(values_copy.size() / 2 - 1);
        return sum / 2;
    }
    return 0;
}

int calc_fuel(const std::vector<int>& positions, int destination)
{
    int sum = 0;
    for (auto pos : positions)
        sum += std::max(pos, destination) - std::min(pos, destination);
    return sum;
}

int calc_fuel_x(const std::vector<int>& positions, int destination)
{
    int sum = 0;
    for (auto pos : positions)
    {
        auto steps = std::max(pos, destination) - std::min(pos, destination);
        sum += static_cast<int>( static_cast<double>(1 + steps) / 2.0 * steps );
    }
    return sum;
}

void part1(const std::vector<int>& initialPositions);
void part2(const std::vector<int>& initialPositions);

int main()
{
    std::cout << "Hello aoc-2021 day 7\n";

    std::vector<std::string> rawData;
    {
        std::fstream f{settings::inputFile};

        while (!f.eof())
        {
            std::string tmp;
            std::getline(f, tmp);
            if (!f.eof() && !tmp.empty())
                rawData.emplace_back(tmp);
        }
    }

    debug::log << rawData.at(0) << '\n';

    std::vector<int> initialPositions;
    
    {
        std::stringstream ss{rawData.at(0)};
        while (!ss.eof())
        {
            int tmp;
            ss >> tmp;
            if (ss)
            {
                initialPositions.emplace_back(tmp);
                ss.ignore(1);
            }
        }
    }

    for (auto pos : initialPositions)
        debug::log << pos << ' ';
    debug::log << '\n';

    debug::log << initialPositions.size() << '\n';

    part1(initialPositions);
    part2(initialPositions);

    return 0;
}

void part1(const std::vector<int>& initialPositions)
{
    std::cout << "\nPart1\n";
    std::cout << calc_fuel(initialPositions, median(initialPositions)) << '\n';
}

void part2(const std::vector<int>& initialPositions)
{
    std::cout << "\nPart2\n";

    std::vector<int> fuelRequirenments;

    int topPos = -1;
    for (auto pos : initialPositions)
        if (pos > topPos)
            topPos = pos;

    for (int i = 0; i <= topPos; ++i)
    {
        fuelRequirenments.emplace_back(calc_fuel_x(initialPositions, i));
    }

    if constexpr (settings::debug_on)
    {
        for (auto v : fuelRequirenments)
            debug::log << v << ' ';
        debug::log << '\n';
    }

    auto best = std::numeric_limits<int>::max();
    for (auto v : fuelRequirenments)
        if (v < best)
            best = v;

    std::cout << best << '\n';
}
