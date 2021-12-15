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
#include <stack>

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

std::vector<std::string> getRawData(const char* path)
{
    std::vector<std::string> rawData;
    {
        std::fstream f{path};

        while (!f.eof())
        {
            std::string tmp;
            std::getline(f, tmp);
            if (!f.eof() && !tmp.empty())
                rawData.emplace_back(tmp);
        }
    }
    return rawData;
}


static constexpr std::array open = {
    '{','(','[','<'
};
static constexpr std::array close = {
    '}',')',']','>'
};
static const std::unordered_map<char,char> closeToOpenMap = {
    {'}','{'},
    {')','('},
    {']','['},
    {'>','<'}
};

class StackBase
{
protected:
    std::stack<char> data;
public:
    char pop()
    {
        if (!data.empty())
        {
            char top = data.top();
            data.pop();
            return top;
        }
        return '\0';
    }

    std::size_t size() const
    {
        return data.size();
    }
};

class Stack : public StackBase
{
public:
    std::tuple<bool, char> push(char c)
    {
        std::tuple result = {true, 'x'};

        if (auto it = std::ranges::find(open, c); it != std::end(open))
        {
            data.push(c);
        }
        else
        {
            if (data.top() == closeToOpenMap.at(c))
            {
                data.pop();
            }
            else
            {
                result = {false, c};
            }
        }
        return result;
    }
};


std::vector<Stack> part1();
void part2(std::vector<Stack>& incomplete);

int main()
{
    auto incomplete = part1();
    part2(incomplete);
    return 0;
}


std::vector<Stack> part1()
{
    std::vector<Stack> incomplete;

    auto rawData = getRawData(settings::inputFile);

    std::vector<char> illigals;

    for (const auto& line : rawData)
    {
        Stack stack;

        bool ok = true;
        char illigal;

        for (auto c : line)
        {
            const auto [_ok, _illigal] = stack.push(c);
            if (!_ok)
            {
                ok = _ok;
                illigal = _illigal;
                break;
            }
        }
        
        debug::log << line << ':';
        if (!ok)
            debug::log << "corrupted, illigal=" << illigal << '\n';
        else if (stack.size() != 0)
            debug::log << "incomplete\n";
        else
            debug::log << "ok\n";

        if (!ok)
            illigals.emplace_back(illigal);
        
        if (ok && stack.size() != 0)
            incomplete.emplace_back(stack);
            
    }

    static const std::unordered_map<char, int> charToScoreMap = {
        {')', 3},
        {']', 57},
        {'}', 1197},
        {'>', 25137}
    };

    int totalScore = 0;
    for (auto c : illigals)
        totalScore += charToScoreMap.at(c);

    std::cout << "Total score = " << totalScore << '\n';

    return incomplete;
}

void part2(std::vector<Stack>& incomplete)
{
    std::cout << "\nPart2\n";

    std::vector<std::string> completions;
    std::vector<std::size_t> scores;

    static const std::unordered_map<char,char> openToCloseMap = {
        {'{','}'},
        {'(',')'},
        {'[',']'},
        {'<','>'}
    };

    for (auto& stack : incomplete)
    {
        std::stringstream ss;
        while (char c = stack.pop())
            ss << openToCloseMap.at(c);
        completions.emplace_back(ss.str());
    }

    if constexpr (settings::debug_on)
        for (const auto& completion : completions)
            std::cout << completion << '\n';

    const std::unordered_map<char, int> charToScore = {
        {')',1},
        {']',2},
        {'}',3},
        {'>',4}
    };

    auto calcScore = [charToScore](const std::string& completion){
        std::size_t res = 0;
        for (auto c : completion)
        {
            res *= 5;
            res += charToScore.at(c);
        }
        return res;
    };

    for (const auto& completion : completions)
    {
        auto score = calcScore(completion);
        scores.emplace_back(score);
        debug::log << completion << ':' << score << '\n';
    }

    std::ranges::sort(scores);
    debug::log << scores.size() << '\n';
    std::cout << "Middle score = " << scores.at(scores.size() / 2 ) << '\n';
}
