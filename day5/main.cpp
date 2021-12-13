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

    constexpr std::size_t rows = 1000;
    constexpr std::size_t cols = 1000;

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

struct Point
{
    int x, y;

    friend std::istream& operator>>(std::istream& in, Point& point)
    {
        in >> point.x;
        in.ignore(1);
        in >> point.y;
        return in;
    }
    friend std::ostream& operator<<(std::ostream& out, const Point& point)
    {
        out << "{x=" << point.x << ", y=" << point.y << '}';
        return out;
    }
    friend bool operator!=(const Point& a, const Point& b)
    {
        return (a.x != b.x || a.y != b.y);
    }
};

struct Vent
{
    Point begin, end;
    enum class Type {vertical, horizontal, diagonal};
    static const char* typeToCStr(Type type)
    {
        switch (type)
        {
        case Type::vertical: return "vertical";
        case Type::horizontal: return "horizontal";
        case Type::diagonal: return "diagonal";
        default:
            assert(0 && "unreachable");
        }
        return "";
    }

    Type type;

    Vent()
        : type{Type::diagonal}
    {
    }

    friend std::istream& operator>>(std::istream& in, Vent& vent)
    {
        in >> std::ws;
        in >> vent.begin;
        in.ignore(4);
        in >> vent.end;
        if (vent.begin.x == vent.end.x)
            vent.type = Type::vertical;
        if (vent.begin.y == vent.end.y)
            vent.type = Type::horizontal;
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const Vent& vent)
    {
        out << "Vent{begin=" << vent.begin << ", end=" << vent.end << ", type=" << typeToCStr(vent.type) << '}';
        return out;
    }
};

struct MapBase
{
    using map_t = std::array< std::array<int, settings::cols>, settings::rows >;
    map_t map;

    MapBase()
    {
        for (auto& row : map)
            for (auto& col : row)
                col = 0;
    }

    virtual void fill(const Vent& vent) = 0;

    int countOverlapping() const
    {
        int count = 0;
        for (const auto& row : map)
            for (const auto& col : row)
                if (col > 1)
                    ++count;
        return count;
    }

    friend std::ostream& operator<<(std::ostream& out, const MapBase& map)
    {
        for (const auto& row : map.map)
        {
            for (const auto& col : row)
            {
                if (col == 0)
                    out << '.';
                else
                    out << col;
            }
            out << '\n';
        }
        return out;
    }

};

struct Map : public MapBase
{
    void fill(const Vent& vent) override
    {
        if (vent.type == Vent::Type::horizontal)
        {
            auto start = std::min(vent.begin.x, vent.end.x);
            auto end = std::max(vent.begin.x, vent.end.x);
            for (int col = start; col <= end; ++col)
            {
                map[vent.begin.y][col] += 1;
            }
        }
        else if (vent.type == Vent::Type::vertical)
        {
            auto start = std::min(vent.begin.y, vent.end.y);
            auto end = std::max(vent.begin.y, vent.end.y);
            for (int row = start; row <= end; ++row)
            {
                map[row][vent.begin.x] += 1;
            }
        }
    }
};

struct Map2 : public MapBase
{
    void fill(const Vent& vent) override
    {
        if (vent.type == Vent::Type::horizontal)
        {
            auto start = std::min(vent.begin.x, vent.end.x);
            auto end = std::max(vent.begin.x, vent.end.x);
            for (int col = start; col <= end; ++col)
            {
                map[vent.begin.y][col] += 1;
            }
        }
        else if (vent.type == Vent::Type::vertical)
        {
            auto start = std::min(vent.begin.y, vent.end.y);
            auto end = std::max(vent.begin.y, vent.end.y);
            for (int row = start; row <= end; ++row)
            {
                map[row][vent.begin.x] += 1;
            }
        }
        else if (vent.type == Vent::Type::diagonal)
        {
            const Point start = vent.begin;
            const Point end = vent.end;
            Point current{start.x, start.y};

            auto movx = [start, end](Point& p){
                if (start.x > end.x)
                    p.x -= 1;
                else
                    p.x += 1;
            };
            auto movy = [start, end](Point& p){
                if (start.y > end.y)
                    p.y -= 1;
                else
                    p.y += 1;
            };

            while (current != end)
            {
                map[current.y][current.x] += 1;
                movx(current);
                movy(current);
            }
            map[current.y][current.x] += 1;
        }
    }
};

void part1(const std::vector<Vent>& vents);
void part2(const std::vector<Vent>& vents);

int main()
{
    std::cout << "Hello aoc-2021 day 5\n";

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

    std::vector<Vent> vents;

    for (const auto& line : rawData)
    {
        std::stringstream ss{line};
        Vent vent;
        ss >> vent;
        vents.emplace_back(vent);
    }

    if constexpr (settings::debug_on)
    {
        for (const auto& vent : vents)
            debug::log << vent << '\n';
    }

    part1(vents);
    part2(vents);

    return 0;
}

void part1(const std::vector<Vent>& vents)
{
    std::cout << "\nPart1\n";

    Map map;

    for (const auto& vent : vents)
    {
        map.fill(vent);
    }

    debug::log << map << '\n';

    std::cout << "Overlapping count = " << map.countOverlapping() << '\n';
}

void part2(const std::vector<Vent>& vents)
{
    std::cout << "\nPart2\n";

    Map2 map;

    for (const auto& vent : vents)
    {
        map.fill(vent);
    }

    debug::log << map << '\n';

    std::cout << "Overlapping count = " << map.countOverlapping() << '\n';
}
