#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <iomanip>
#include <limits>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <unordered_map>
#include <functional>


namespace settings
{
    const char *inputFile = "./input.txt";
    
    constexpr bool debug_on = false;

    constexpr int rows = 100;
    constexpr int cols = 100;
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

struct Point;
using Basin = std::vector<std::reference_wrapper<Point>>;
using Map = std::array< std::array<Point, settings::cols>, settings::rows >;

std::vector<Point> part1(const std::vector<std::string>& data);
void part2(const std::vector<std::string>& data, const std::vector<Point>& lowPoints);

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

    auto lowPoints = part1(rawData);
    part2(rawData, lowPoints);

    return 0;
}

struct Point
{
    int value, x, y;
    bool added;

    std::vector<std::reference_wrapper<Point>> neighbours(Map& map) const
    {
        std::vector<std::reference_wrapper<Point>> _neighbours;
        if (y - 1 >= 0)
            _neighbours.emplace_back(map[y-1][x]);
        if (x + 1 < settings::cols)
            _neighbours.emplace_back(map[y][x+1]);
        if (y + 1 < settings::rows)
            _neighbours.emplace_back(map[y+1][x]);
        if (x - 1 >= 0)
            _neighbours.emplace_back(map[y][x-1]);
        return _neighbours;
    }
};

void addNeighbours(Map& map, const Point& point, Basin& basin)
{
    if (point.value == 9)
        return;
    
    for (auto& p : point.neighbours(map))
    {
        if (p.get().value != 9 && p.get().added == false)
        {
            p.get().added = true;
            basin.emplace_back(p);
            addNeighbours(map, p, basin);
        }
    }
}

std::vector<Point> part1(const std::vector<std::string>& data)
{
    std::cout << "\nPart1\n";

    const auto cols = static_cast<int>(data.at(0).length());
    const auto rows = static_cast<int>(data.size());
    debug::log << rows << 'x' << cols << '\n';

    if constexpr (settings::debug_on)
        for (const auto& line : data)
            debug::log << line << '\n';


    auto ctoi = [](char c){
        return static_cast<int>(c) - static_cast<int>('0');
    };

    auto isLowPoint = [ctoi, rows, cols](const std::vector<std::string>& data, int row, int col){
        int val = ctoi(data.at(row).at(col));

        if (row-1 >= 0)
            if (val >= ctoi(data.at(row-1).at(col)))
                return false;
        
        if (row+1 < rows)
            if (val >= ctoi(data.at(row+1).at(col)))
                return false;

        if (col-1 >= 0)
            if (val >= ctoi(data.at(row).at(col-1)))
                return false;

        if (col+1 < cols)
            if (val >= ctoi(data.at(row).at(col+1)))
                return false;
                
        return true;
    };

    std::vector<Point> lowestPoints;

    for (int row = 0; row < rows; ++row)
        for (int col = 0; col < cols; ++col)
            if (isLowPoint(data, row, col))
                lowestPoints.emplace_back(ctoi(data.at(row).at(col)), col, row, false);

    if constexpr (settings::debug_on)
        for (auto p : lowestPoints)
            debug::log << p << ' ';
    debug::log << '\n';

    int sumRisks = 0;
    for (const auto& p : lowestPoints)
        sumRisks += (p.value + 1);
    
    std::cout << "Sum: " << sumRisks << '\n';

    return lowestPoints;
}

void part2(const std::vector<std::string>& data, const std::vector<Point>& lowPoints)
{
    std::cout << "\nPart2\n";

    int values[settings::rows][settings::cols];

    auto ctoi = [](char c){return static_cast<int>(c - '0');};

    for (auto row = 0; row < settings::rows; ++row)
        for (auto col = 0; col < settings::cols; ++col)
            values[row][col] = ctoi(data[row][col]);

    Map map;
    for (auto row = 0; row < settings::rows; ++row)
    {
        for (auto col = 0; col < settings::cols; ++col)
        {
            Point p{values[row][col], col, row, false};
            map[row][col] = p;
        }
    }
        
    std::vector<Basin> basins;

    for (int i = 0; auto& p : lowPoints)
    {
        Basin basin;
        addNeighbours(map, p, basin);
        basins.emplace_back(basin);
        ++i;
    }

    std::sort(std::begin(basins), std::end(basins), [](const Basin& a, const Basin& b){
        return a.size() > b.size();
    });

    auto mul3 = [](const auto& basins){
        return basins.at(0).size() * basins.at(1).size() * basins.at(2).size();
    };

    std::cout << "Multiplication of 3 largest basins = " << mul3(basins) << '\n';
}
