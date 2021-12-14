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


namespace settings
{
    const char *inputFile = "./input.txt";
    constexpr bool debug_on = false;

    constexpr std::size_t unique_n_segments[] = {
        2, 4, 3, 7
    };

    // constexpr int digit1_nsegments = 2;
    // constexpr int digit4_nsegments = 4;
    // constexpr int digit7_nsegments = 3;
    // constexpr int digit8_nsegments = 7;
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


void part1(const std::vector< std::vector<std::string> >& outputs);
void part2(const std::vector< std::vector<std::string> >& codes, const std::vector< std::vector<std::string> >& outputs);

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

    std::vector< std::vector<std::string> > rightSides;

    for (const auto& line : rawData)
    {
        std::string right = line.substr(line.find(" | ") + 3);
        std::stringstream ss{right};
        std::vector<std::string> outputs;
        while (!ss.eof())
        {
            std::string tmp;
            ss >> tmp;
            if (ss)
                outputs.emplace_back(tmp);
        }
        rightSides.emplace_back(outputs);
    }

    std::vector < std::vector<std::string> > leftSides;

    for (const auto& line : rawData)
    {
        std::string right = line.substr(0, line.find(" | "));
        std::stringstream ss{right};
        std::vector<std::string> outputs;
        while (!ss.eof())
        {
            std::string tmp;
            ss >> tmp;
            if (ss)
                outputs.emplace_back(tmp);
        }
        leftSides.emplace_back(outputs);
    }


    part1(rightSides);
    part2(leftSides, rightSides);

    return 0;
}

void part1(const std::vector< std::vector<std::string> >& outputs)
{
    std::cout << "\nPart1\n";

    if constexpr (settings::debug_on)
    {
        for (const auto& line : outputs)
        {
            for (const auto& token : line)
                std::cout << std::quoted(token) << ' ';
            std::cout << '\n';
        }
    }

    std::size_t total = 0;

    for (const auto& vec : outputs)
    {
        total += std::count_if(std::begin(vec), std::end(vec), [](const auto& str){
            for (auto v : settings::unique_n_segments)
                if (str.length() == v)
                    return true;
            return false;
        });
    }

    std::cout << "Count: " << total << '\n';

}


std::string translate(const std::string& pattern, const std::string& key)
{
    static constexpr char lookup[] = "abcdefg";

    std::unordered_map<char, char> map;
    for (auto i = 0; i < 7; ++i)
        map[key[i]] = lookup[i];

    std::string result{pattern};
    for (auto& c : result)
    {
        c = map[c];
    }

    return result;
}

bool check_key_with_pattern(const std::string& key, const std::string& pattern)
{
    std::string translated = translate(pattern, key);

    std::ranges::sort(translated);
    // debug::log << translated << '\n';

    static constexpr const char* meaningful_patterns[] = {
        "abcefg",
        "cf",
        "acdeg",
        "acdfg",
        "bcdf",
        "abdfg",
        "abdefg",
        "acf",
        "abcdefg",
        "abcdfg"
    };
    for (const auto& p : meaningful_patterns)
    {
        if (translated == p)
            return true;
    }
    return false;
}

void part2(const std::vector< std::vector<std::string> >& codes, const std::vector< std::vector<std::string> >& outputs)
{
    std::cout << "\nPart2\n";

    if constexpr (settings::debug_on)
    {
        for (const auto& line : codes)
        {
            for (const auto& token : line)
                std::cout << std::quoted(token) << ' ';
            std::cout << '\n';
        }
    }

    if constexpr (settings::debug_on)
    {
        for (const auto& line : outputs)
        {
            for (const auto& token : line)
                std::cout << std::quoted(token) << ' ';
            std::cout << '\n';
        }
    }

    std::vector<std::string> permutations;

    std::string s = "abcdefg";

    do {
        permutations.emplace_back(s);
    } while(std::next_permutation(std::begin(s), std::end(s)));

    if constexpr (settings::debug_on)
        for (const auto& perm : permutations)
            std::cout << std::quoted(perm) << '\n';
    
    debug::log << permutations.size() << '\n';

    debug::log << check_key_with_pattern("deafgbc", "abd") << '\n';

    std::vector<std::string> keys;

    for (const auto& line : codes)
    {
        bool found = false;
        std::string correct_key;
        do {
            for (const auto& key : permutations)
            {
                bool ok = true;

                for (const auto& pattern : line)
                {
                    if (check_key_with_pattern(key, pattern) == false)
                    {
                        ok = false;
                        break;
                    }
                }
                
                if (ok)
                {
                    found = true;
                    correct_key = key;
                    break;
                }
            }
        } while (!found);


        keys.emplace_back(correct_key);
    }


    static const std::unordered_map<std::string, int> map = {
        {"abcefg", 0},
        {"cf", 1},
        {"acdeg", 2},
        {"acdfg", 3},
        {"bcdf", 4},
        {"abdfg", 5},
        {"abdefg", 6},
        {"acf", 7},
        {"abcdefg", 8},
        {"abcdfg", 9}
    };


    int total = 0;
    std::size_t i = 0;
    for (const auto& key : keys)
    {
        std::stringstream ss;
        debug::log << "Key = " << key << ' ';
        for (const auto& output : outputs.at(i))
        {
            std::string sorted {output};
            std::ranges::sort(sorted);
            auto o = translate(sorted, key);
            std::ranges::sort(o);
            ss << map.at(o);
        }
        debug::log << ss.str() << '\n';
        total += std::stoi(ss.str());
        ++i;
    }

    std::cout << "total = " << total << '\n';
}
