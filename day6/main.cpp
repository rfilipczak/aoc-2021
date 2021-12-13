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
    constexpr int newLanternFishTimer = 8;
    constexpr int afterSpawnTimer = 6;
    constexpr int exampleHowManyDays = 18;
    constexpr int part1howManyDays = 80;
    constexpr int part2howManyDays = 256;
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


class Lanternfish
{
private:
    int timer;
public:
    Lanternfish(int init)
        : timer{init}
    {
    }
    bool update()
    {
        bool spawn = false;
        if (timer == 0)
            spawn = true;
        timer -= 1;
        if (timer < 0)
            timer = settings::afterSpawnTimer;
        return spawn;
    }
    friend std::ostream& operator<<(std::ostream& out, const Lanternfish& fish)
    {
        out << fish.timer;
        return out;
    }
};

class School
{
private:
    std::vector<Lanternfish> school;
public:
    School(const std::string& input)
    {
        std::stringstream ss{input};
        while (!ss.eof())
        {
            int tmp;
            ss >> tmp;
            if (ss)
            {
                school.emplace_back(tmp);
                ss.ignore(1);
            }
        }
    }

    void update()
    {
        int toAdd = 0;
        for (auto& fish : school)
        {
            if (fish.update())
            {
                ++toAdd;
            }
        }
        for (int i = 0; i < toAdd; ++i)
            school.emplace_back(settings::newLanternFishTimer);
    }

    auto count() const
    {
        return school.size();
    }

    friend std::ostream& operator<<(std::ostream& out, const School& school)
    {
        for (const auto& fish : school.school)
            out << fish << ',';
        return out;
    }
};


class SchoolState
{
private:
    std::array<std::size_t, 9> timerCounts = {0,0,0,0,0,0,0,0,0};
public:
    SchoolState(const std::string& input)
    {
        std::stringstream ss{input};
        while (!ss.eof())
        {
            std::size_t tmp;
            ss >> tmp;
            if (ss)
            {
                timerCounts.at(tmp) += 1;
                ss.ignore(1);
            }
        }
    }

    void update()
    {
        std::size_t toAddAt6 = 0;
        std::size_t toAddAt8 = 0;
        if (timerCounts.at(0) != 0)
        {
            auto amount = timerCounts.at(0);
            toAddAt6 = amount;
            toAddAt8 = amount;
            timerCounts.at(0) -= amount;
        }

        for (std::size_t i = 1; i < 9; ++i)
        {
            if (timerCounts.at(i) != 0)
            {
                std::size_t amount = timerCounts.at(i);
                timerCounts.at(i) -= amount;
                timerCounts.at(i-1) += amount;
            }
        }

        timerCounts.at(6) += toAddAt6;
        timerCounts.at(8) += toAddAt8;
    }

    std::size_t count() const
    {
        std::size_t res = 0;
        for (auto count : timerCounts)
            res += count;
        return res;
    }

    friend std::ostream& operator<<(std::ostream& out, const SchoolState& school)
    {
        for (const auto& fish : school.timerCounts)
            out << fish << ',';
        return out;
    }
};

void part1(const School& initial_school);
void part2(const std::string& raw_input);

int main()
{
    std::cout << "Hello aoc-2021 day 6\n";

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

    School school{rawData.at(0)};
    debug::log << school << '\n';

    part1(school);
    part2(rawData.at(0));

    return 0;
}

void part1(const School& initial_school)
{
    std::cout << "\nPart1\n";

    School school{initial_school};

    debug::log << "Initial state: " << school << '\n';
    for (int i = 0; i < settings::part1howManyDays; ++i)
    {
        school.update();
        debug::log << "After " << std::setw(2) << i+1 << " days: " << school << '\n';
    }

    std::cout << "After " << settings::part1howManyDays << " days = " << school.count() << '\n';
}

void part2(const std::string& raw_input)
{
    std::cout << "\nPart2\n";

    SchoolState school{raw_input};

    debug::log << "Initial state: " << school << '\n';
    
    for (int i = 0; i < settings::part2howManyDays; ++i)
    {
        school.update();
        debug::log << "After " << std::setw(2) << i+1 << " days: " << school << '\n';
    }

    std::cout << "After " << settings::part2howManyDays << " days = " << school.count() << '\n';
}
