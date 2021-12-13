#include <iostream>
#include <cassert>
#include <vector>
#include <fstream>


namespace settings
{
    const char *inputFile = "./input.txt";
}




struct Command
{
    enum class Type
    {
        Forward,
        Down,
        Up
    };

    Type type;
    int amount;

    friend std::ostream& operator<<(std::ostream& out, const Command& command)
    {
        out << "Command { type = ";
        switch (command.type)
        {
        case Type::Forward:
            out << "forward";
            break;
        case Type::Down:
            out << "down";
            break;
        case Type::Up:
            out << "up";
            break;
        default:
            assert(0 && "unreachable");
            break;
        }
        out << " , amount = " << command.amount << " }";
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Command& command)
    {
        std::string type;
        int value;

        in >> type;
        in >> value;

        if (!in)
            return in;

        if (type == "forward")
            command.type = Command::Type::Forward;
        else if (type == "down")
            command.type = Command::Type::Down;
        else if (type == "up")
            command.type = Command::Type::Up;
        else
            assert(0 && "unrecognized command type");
        
        command.amount = value;

        return in;
    }
};

struct Position
{
    int horizontal = 0;
    int depth = 0;
};

struct Submarine
{
    Position pos;

    void move(Command command)
    {
        switch (command.type)
        {
        case Command::Type::Forward:
            pos.horizontal += command.amount;
            break;
        case Command::Type::Down:
            pos.depth += command.amount;
            break;
        case Command::Type::Up:
            pos.depth -= command.amount;
            break;
        default:
            assert(0 && "unreachable\n");
            break;
        }
    }
};


struct Submarine2
{
    Position pos;
    int aim = 0;

    void move(Command command)
    {
        switch (command.type)
        {
        case Command::Type::Forward:
            pos.horizontal += command.amount;
            pos.depth += aim * command.amount;
            break;
        case Command::Type::Down:
            aim += command.amount;
            break;
        case Command::Type::Up:
            aim -= command.amount;
            break;
        default:
            assert(0 && "unreachable\n");
            break;
        }
    }
};

void part1(const std::vector<Command>& commands);
void part2(const std::vector<Command>& commands);

int main()
{
    std::cout << "Hello aoc-2021 day2\n";

    std::vector<Command> commands;    
    {
        std::fstream f{settings::inputFile};

        while (!f.eof())
        {
            Command tmp;
            f >> tmp;
            if (!f.eof())
                commands.emplace_back(tmp);
        }
    }
    
    part1(commands);
    part2(commands);

    return 0;
}

void part1(const std::vector<Command>& commands)
{
    std::cout << "Part1\n";

    Submarine submarine;

    for (const auto& command: commands)
    {
        // std::cout << command << '\n';
        submarine.move(command);
    }

    std::cout << "Pos { horizontal = " << submarine.pos.horizontal << " , depth = " << submarine.pos.depth << " }\n";
    std::cout << "horizontal * depth = " << submarine.pos.horizontal * submarine.pos.depth << '\n';
}

void part2(const std::vector<Command>& commands)
{
    std::cout << "Part2\n";

    Submarine2 submarine;
    for (const auto& command: commands)
    {
        // std::cout << command << '\n';
        submarine.move(command);
    }
    std::cout << "Pos { horizontal = " << submarine.pos.horizontal << " , depth = " << submarine.pos.depth << " }\n";
    std::cout << "horizontal * depth = " << submarine.pos.horizontal * submarine.pos.depth << '\n';
}
