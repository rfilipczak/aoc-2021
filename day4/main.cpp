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

    constexpr std::size_t rows = 5;
    constexpr std::size_t cols = 5;
}

class Board
{
private:
    struct element
    {
        int value;
        bool marked=false;
    };
    using board_t = std::array< std::array<element, settings::cols>, settings::rows >;
    board_t board;
    bool wonStatus = false;
public:
    void mark(int value)
    {
        for (std::size_t row = 0; row < settings::rows; ++row)
            for (std::size_t col = 0; col < settings::cols; ++col)
                if (board[row][col].value == value)
                    board[row][col].marked = true;
    }

    bool checkBingo()
    {
        for (const auto& row : board)
        {
            if (std::all_of(std::begin(row), std::end(row), [row](const element& el){ return el.marked == true; }))
            {
                wonStatus = true;
                return true;
            }
        }

        for (std::size_t col = 0; col < settings::cols; ++col)
        {
            int markedCount = 0;
            for (std::size_t row = 0; row < settings::rows; ++row)
            {
                if (board[row][col].marked)
                {
                    ++markedCount;
                }
            }
            if (markedCount == settings::rows)
            {
                wonStatus = true;
                return true;
            }
        }

        return false;
    }

    bool won() const { return (wonStatus == true); }

    int sumUnmarked() const
    {
        int sum = 0;
        for (std::size_t row = 0; row < settings::rows; ++row)
            for (std::size_t col = 0; col < settings::cols; ++col)
                if (board[row][col].marked == false)
                    sum += board[row][col].value;
        return sum;
    }

    friend std::istream& operator>>(std::istream& in, Board& board)
    {
        for (std::size_t row = 0; row < settings::rows; ++row)
        {
            for (std::size_t  col = 0; col < settings::cols; ++col)
            {
                int tmp;
                in >> std::ws >> tmp;
                if (in)
                    board.board[row][col].value = tmp;
            }
        }
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const Board& board)
    {
        out << "Board {\n";
        for (const auto& row : board.board)
        {
            out << "    ";
            for (const auto& col : row)
            {
                out << std::setw(3) << col.value << ' ';
            }
            out << '\n';
        }
        out << "}";
        return out;
    }
};

void part1(const std::vector<int>& numbersToDraw, const std::vector<Board>& boards);
void part2(const std::vector<int>& numbersToDraw, const std::vector<Board>& boards);

int main()
{
    std::cout << "Hello aoc-2021 day4\n";

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

    std::vector<int> numbersToDraw;
    {
        std::stringstream ss{rawData[0]};

        while (ss)
        {
            int tmp;
            ss >> tmp;
            if (ss)
                numbersToDraw.emplace_back(tmp);
            ss.ignore(std::numeric_limits<std::streamsize>::max(), ',');
        }
        rawData.erase(std::begin(rawData));
    }

    auto boardsCount = rawData.size() / settings::rows;

    std::vector<Board> boards;
    boards.reserve(boardsCount);

    {
        auto it = std::begin(rawData);
        for (std::size_t  i = 0; i < boardsCount; ++i)
        {
            std::stringstream squashed;

            for (std::size_t row = 0; row < settings::rows; ++row)
            {
                squashed << *it << ' ';
                ++it;
            }

            Board board;
            squashed >> board;
            boards.emplace_back(board);
        }
    }

    part1(numbersToDraw, boards);
    part2(numbersToDraw, boards);

    return 0;
}

void part1(const std::vector<int>& numbersToDraw, const std::vector<Board>& boards)
{
    std::cout << "Part1\n";

    std::vector<Board> _boards{boards};
    bool gameOver = false;

    auto it = std::begin(numbersToDraw);
    Board* winner = nullptr;
    int winningNumber = 0;
    while (!gameOver)
    {
        auto drawnNumber = *it++;
        
        for (auto& board : _boards)
        {
            board.mark(drawnNumber);
            if (board.checkBingo())
            {
                winner = &board;
                winningNumber = drawnNumber;
                gameOver = true;
            }
        }
    }

    std::cout << "Winner=\n" << *winner << '\n';
    std::cout << "Sum unmarked * winning number = " << winner->sumUnmarked() * winningNumber << '\n';
}

void part2(const std::vector<int>& numbersToDraw, const std::vector<Board>& boards)
{
    std::cout << "Part2\n";

    std::vector<Board> _boards{boards};

    std::vector<int> winners;

    bool gameOver = false;
    auto it = std::begin(numbersToDraw);

    std::size_t wonCount = 0;

    int lastDrawn = 0;

    while (!gameOver)
    {
        auto drawnNumber = *it++;

        // std::cout << "draw=" << drawnNumber << '\n';

        for (std::size_t index = 0; auto& board : _boards)
        {
            if (!board.won())
            {
                board.mark(drawnNumber);
                if (board.checkBingo())
                {
                    ++wonCount;
                    winners.emplace_back(index);
                }
            }
            ++index;
        }   
        if (wonCount == _boards.size())
        {
            lastDrawn = drawnNumber;
            gameOver = true;
        }
    }

    auto last = _boards[winners.back()];
    std::cout << "last to win=\n" << last << '\n';
    std::cout << "score=" << last.sumUnmarked() * lastDrawn << '\n';
}
