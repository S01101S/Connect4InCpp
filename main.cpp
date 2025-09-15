#include <iostream>
#include <vector>
#include <chrono>

class Connect4Game
{

private:
    std::vector<std::vector<char>> board;
    unsigned long long int xArray = 0;
    unsigned long long int oArray = 0;
    unsigned long long int wholeBoard = 0;
    int currentPlayer = 0;
    int position = -1;

    bool winCheckConditions(unsigned long long int boardToCheck, int shift1, int shift2) const
    {
        unsigned long long int firstPair = boardToCheck & (boardToCheck >> shift1);
        unsigned long long int secondPair = firstPair & (firstPair >> shift2);

        return secondPair != 0;
    }


    bool horizontalCheck(unsigned long long int boardToCheck) const
    {
        return winCheckConditions(boardToCheck, 7, 14);
    }


    bool verticalCheck(unsigned long long int boardToCheck) const
    {
        return winCheckConditions(boardToCheck, 1, 2);
    }

    bool diagonalLeft(unsigned long long int boardToCheck) const
    {
        return winCheckConditions(boardToCheck, 6, 12);
    }

    bool diagonalRight(unsigned long long int boardToCheck) const
    {
        return winCheckConditions(boardToCheck, 8, 16);
    }

public:

    Connect4Game()
    {
        board = std::vector<std::vector<char>>(6, std::vector<char>(7, '*'));
    }


    bool winCheckWithLoops() const
    {
        char piece = (currentPlayer == 0) ? 'X' : 'O';

        for (int row=0; row<6; row++)
        {
            for (int col=0; col<=3; col++)
            {
                if (board[row][col] == piece && board[row][col+1] == piece && board[row][col+2] == piece && board[row][col+3] == piece)
                {
                    return true;
                }
            }
        }

        for (int col=0; col<7; col++)
        {
            for (int row=0; row<=2; row++)
            {
                if (board[row][col] == piece && board[row+1][col] == piece && board[row+2][col] == piece && board[row+3][col] == piece)
                {
                    return true;
                }
            }
        }

        for (int row=3; row<6; row++)
        {
            for (int col=0; col<=3; col++)
            {
                if (board[row][col] == piece && board[row-1][col+1] == piece && board[row-2][col+2] == piece && board[row-3][col+3] == piece)
                {
                    return true;
                }
            }
        }

        for (int row=0; row<=2; row++)
        {
            for (int col=0; col<=3; col++)
            {
                if (board[row][col] == piece && board[row+1][col+1] == piece && board[row+2][col+2] == piece && board[col+3][row+3] == piece)
                {
                    return true;
                }
            }
        }

        return false;
    }

    void resetGame()
    {
        xArray = 0;
        oArray = 0;
        wholeBoard = 0;
        currentPlayer = 0;
        position = -1;
        board = std::vector<std::vector<char>>(6, std::vector<char>(7, '*'));
    }

    void displayBoard() const
    {
        for (int i=5; i>=0; i--)
        {
            for (int j=0; j<7; j++)
            {
                std::cout << " " << board[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void updateBoard()
    {
        int row = position%7;
        int col = position/7;

        if (row < 6 && col < 7)
        {
            if (currentPlayer == 0)
            {
                board[row][col] = 'X';
            }
            else
            {
                board[row][col] = 'O';
            }
        }
    }

    bool winCheck() const
    {
        unsigned long long int boardToCheck = (currentPlayer == 0) ? xArray : oArray;

        return horizontalCheck(boardToCheck) || verticalCheck(boardToCheck) || diagonalLeft(boardToCheck) || diagonalRight(boardToCheck);
    }

    void displayWinMessage(int winner) const
    {
        if (winner == 0)
        {
            std::cout << "Player 1 wins" << std::endl;
        }
        else if (winner == 1)
        {
            std::cout << "Player 2 wins" << std::endl;
        }
        else
        {
            std::cout << "Draw" << std::endl;
        }
    }

    void run()
    {
        bool gameOver = false;
        int columnNumber = 0;
        char playAgain = 'Y';

        displayBoard();

        while (playAgain == 'Y' || playAgain == 'y')
        {

            int moveCount = 0;
            int winner = -1;

            while (!gameOver)
            {
                if (currentPlayer == 0)
                {
                    std::cout << "Player 1's turn" << std::endl;
                }
                else
                {
                    std::cout << "Player 2's turn" << std::endl;
                }

                wholeBoard = xArray | oArray;
                std::cout << "Pick a column between 1-7: ";
                std::cin >> columnNumber;

                int columnChecker = (columnNumber - 1) * 7;

                for (int i=columnChecker; i<(columnChecker+6); i++)
                {
                    if ((1 & (wholeBoard >> i)) == 0)
                    {
                        position = i;
                        break;
                    }
                    else
                    {
                        position = -1;
                    }
                }

                if (position == -1)
                {
                    std::cout << "Invalid Position" << std::endl;
                }

                if (position != -1)
                {
                    updateBoard();
                    displayBoard();
                    moveCount++;

                    if (currentPlayer == 0)
                    {
                        xArray = xArray | (1ULL << position);

                        if (winCheck())
                        {
                            winner = currentPlayer;
                            gameOver = true;
                        }
                        else if (moveCount == 42)
                        {
                            gameOver = true;
                        }
                        else
                        {
                            currentPlayer = 1;
                        }
                    }
                    else if (currentPlayer == 1)
                    {
                        oArray = oArray | (1ULL << position);

                        if (winCheck())
                        {
                            winner = currentPlayer;
                            gameOver = true;
                        }
                        else if (moveCount == 42)
                        {
                            gameOver = true;
                        }
                        else
                        {
                            currentPlayer = 0;
                        }
                    }
                }
            }

            std::cout << "------------------------------" << std::endl;
            displayWinMessage(winner);
            std::cout << "------------------------------" << std::endl;

            std::cout << "Would you like to play again? (Y/N)" << std::endl;
            std::cin >> playAgain;

            if (playAgain == 'Y')
            {
                gameOver = false;
                resetGame();
                displayBoard();
            }
            else
            {
                break;
            }
        }

        std::cout << "Thank you for playing." << std::endl;
    }


};



void runBenchmark(Connect4Game& game)
{
    const long long ITERATIONS = 20000000;
    volatile bool result = false;

    std::cout << "--- Running Benchmark (" << ITERATIONS << " iterations) ----" << std::endl;

    auto startBitboard = std::chrono::high_resolution_clock::now();

    for (long long i=0; i<ITERATIONS; i++)
    {
        result |= game.winCheck();
    }

    auto endBitboard = std::chrono::high_resolution_clock::now();
    auto bitBoardDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endBitboard - startBitboard).count();

    std::cout << "Bitboard method took: " << bitBoardDuration << " ms" << std::endl;

    auto startLoop = std::chrono::high_resolution_clock::now();

    for (long long i=0; i<ITERATIONS; i++)
    {
        result |= game.winCheckWithLoops();
    }

    auto endLoop = std::chrono::high_resolution_clock::now();
    auto loopDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endLoop - startLoop).count();

    std::cout << "Nested-loop method took: " << loopDuration << " ms" << std::endl;

    if (bitBoardDuration > 0)
    {
        double speedup = static_cast<double>(loopDuration)/bitBoardDuration;
        std::cout << "The bitboard method is roughly " << speedup << " times faster." << std::endl;
    }
}




int main()
{
    Connect4Game game;
    runBenchmark(game);
    return 0;
}