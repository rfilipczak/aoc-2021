#include <iostream>
#include <fstream>
#include <vector>
#include <array>


namespace settings
{
    const char *inputFile = "./input.txt";
}

void part1(const std::vector<int>& data);
void part2(const std::vector<int>& data);

int main()
{
    std::cout << "Hello aoc-2021 day1\n";
    
    std::vector<int> data;
    
    {
        std::fstream f{settings::inputFile};

        while (!f.eof())
        {
            int tmp;
            f >> tmp;
            if (!f.eof())
                data.emplace_back(tmp);
        }
    }

    std::cout << "Data size = " << data.size() << '\n';

    part1(data);
    part2(data);

    return 0;
}

void part1(const std::vector<int>& data)
{
    std::cout << "Part1\n";

    std::size_t increaseCount = 0;
    for (std::size_t i = 1; i < data.size(); ++i)
    {
        if (data[i] > data[i-1])
            ++increaseCount;
    }

    std::cout << "Increase count = " << increaseCount << '\n';
}

void part2(const std::vector<int>& data)
{
    std::cout << "Part2\n";

    using window = std::array<int, 3>;
    std::vector<window> windows;
    windows.reserve(data.size() - 2);

    std::size_t i = 0;
    window currentWindow;
    for (auto it = std::begin(data); it != std::end(data); ++it)
    {
        currentWindow[i++] = *it;
        if (i == 3)
        {
            windows.emplace_back(currentWindow);
            i = 0;
            it -= 2;
        }
    }

    std::vector<std::size_t> sums;
    
    for (std::size_t c = 0; const auto& window: windows)
    {
        std::size_t sum = 0;
        for (const auto& v: window)
            sum += v;
        sums.emplace_back(sum);
        ++c;
    }

    std::size_t increaseCount = 0;

    for (std::size_t i = 1; i < sums.size(); ++i)
    {
        if (sums[i] > sums[i-1])
            ++increaseCount;
    }

    std::cout << "Increase count = " << increaseCount << '\n';
}
