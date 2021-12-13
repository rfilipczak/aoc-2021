#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <bitset>
#include <unordered_map>
#include <algorithm>

namespace settings
{
    const char *inputFile = "./input.txt";
    constexpr std::size_t digits_count = 12;
}

void part1(const std::vector<std::string>& data);
void part2(const std::vector<std::string>& data);

int main()
{
    std::cout << "Hello aoc-2021 day3\n";

    std::vector<std::string> data;

    {
        std::fstream f{settings::inputFile};

        while (!f.eof())
        {
            std::string tmp;
            f >> tmp;
            if (!f.eof())
                data.emplace_back(tmp);
        }
    }

    part1(data);
    part2(data);

    return 0;
}


void part1(const std::vector<std::string>& data)
{
    auto dgts = data[0].length();

    std::string gamma;
    std::string epsilon;

    for (decltype(dgts) i = 0; i < dgts; ++i)
    {
        std::size_t zeroes = 0;
        std::size_t ones = 0;

        for (auto it = std::begin(data); it != std::end(data); ++it)
        {    
            char bit = (*it)[i];

            if (bit == '0')
                ++zeroes;
            else if (bit == '1')
                ++ones;
            else
                assert(0 && "unrecognized bit");
        }

        char mostCommon = (zeroes > ones) ? '0' : '1';

        gamma += mostCommon;
        epsilon += (mostCommon == '0') ? '1' : '0';
    }

    // std::cout << "Gamma = " << gamma << '\n';
    // std::cout << "Epislon = " << epsilon << '\n';

    std::bitset<settings::digits_count> gamma_bitset{gamma};
    std::bitset<settings::digits_count> epsilon_bitset{epsilon};
    auto gamma_value = gamma_bitset.to_ulong();
    auto epsilon_value = epsilon_bitset.to_ulong();

    std::cout << "Gamma = " << gamma_value << '\n';
    std::cout << "Epsilon = " << epsilon_value << '\n';

    std::cout << "Gamma * epsilon = " << gamma_value * epsilon_value << '\n';
}

void part2(const std::vector<std::string>& data)
{
    std::cout << "Part2\n";
    
    std::string oxygen;
    
    {
        enum IN_OR_OUT{ OUT = 0, IN = 1};
        std::vector<int> in_or_out(data.size(), IN);

        auto left = data.size();

        std::size_t current_bit = 0;

        while (left != 1)
        {
            std::size_t zeroes = 0;
            std::size_t ones = 0;

            for (std::size_t index = 0; auto flag : in_or_out)
            {
                if (flag == IN)
                {
                    if (data[index][current_bit] == '0')
                        ++zeroes;
                    else if (data[index][current_bit] == '1')
                        ++ones;
                    else
                        assert(0 && "unrecognized bit");
                }
                ++index;
            }

            if (zeroes == ones)
            {
                for (std::size_t index = 0; auto flag : in_or_out)
                {
                    if (flag == IN)
                    {
                        if (data[index][current_bit] != '1')
                            in_or_out[index] = OUT;
                    }
                    ++index;
                }

                // oxygen += '1';
            }
            else
            {
                char mostCommon = (zeroes > ones) ? '0' : '1';
                // std::cout << "most common=" << mostCommon << '\n';
                
                for (std::size_t index = 0; auto flag : in_or_out)
                {
                    if (flag == IN)
                    {
                        if (data[index][current_bit] != mostCommon)
                            in_or_out[index] = OUT;
                    }
                    ++index;
                }
                // oxygen += mostCommon;
            }

            ++current_bit;
            left = std::count(std::begin(in_or_out), std::end(in_or_out), IN);   
        }

        std::size_t oxygen_index = 0;
        for (std::size_t index = 0; auto flag : in_or_out)
        {
            if (flag == IN)
            {
                oxygen_index = index;
                break;
            }
            ++index;
        }

        oxygen = data[oxygen_index];
    }

    // std::cout << "Oxygen = " << oxygen << '\n';

    std::string co2;

    {
        enum IN_OR_OUT{ OUT = 0, IN = 1};
        std::vector<int> in_or_out(data.size(), IN);

        auto left = data.size();

        std::size_t current_bit = 0;

        while (left != 1)
        {
            std::size_t zeroes = 0;
            std::size_t ones = 0;

            for (std::size_t index = 0; auto flag : in_or_out)
            {
                if (flag == IN)
                {
                    if (data[index][current_bit] == '0')
                        ++zeroes;
                    else if (data[index][current_bit] == '1')
                        ++ones;
                    else
                        assert(0 && "unrecognized bit");
                }
                ++index;
            }

            if (zeroes == ones)
            {
                for (std::size_t index = 0; auto flag : in_or_out)
                {
                    if (flag == IN)
                    {
                        if (data[index][current_bit] != '0')
                            in_or_out[index] = OUT;
                    }
                    ++index;
                }
            }
            else
            {
                char mostCommon = (zeroes > ones) ? '0' : '1';
                
                for (std::size_t index = 0; auto flag : in_or_out)
                {
                    if (flag == IN)
                    {
                        if (data[index][current_bit] == mostCommon)
                            in_or_out[index] = OUT;
                    }
                    ++index;
                }
            }

            ++current_bit;
            left = std::count(std::begin(in_or_out), std::end(in_or_out), IN);   
        }

        std::size_t co2_index = 0;
        for (std::size_t index = 0; auto flag : in_or_out)
        {
            if (flag == IN)
            {
                co2_index = index;
                break;
            }
            ++index;
        }

        co2 = data[co2_index];
    }

    // std::cout << "Co2 = " << co2 << '\n';

    std::bitset<settings::digits_count> oxygen_bitset{oxygen};
    std::bitset<settings::digits_count> co2_bitset{co2};
    auto oxygen_value = oxygen_bitset.to_ulong();
    auto co2_value = co2_bitset.to_ulong();

    std::cout << "Oxygen = " << oxygen_value << '\n';
    std::cout << "CO2 = " << co2_value << '\n';

    std::cout << "Oxygen * CO2 = " << oxygen_value * co2_value << '\n';
}
