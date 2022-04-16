#include <gtest/gtest.h>
#include "connectfour/ConnectFour.h"

TEST(Board, Stones) {
    using namespace play::connectfour;
    using namespace play::game;

    Board board;
    EXPECT_FALSE(board.isFull());

    EXPECT_TRUE(board.canPlay(0));
    EXPECT_FALSE(board.canPlay(7));

    EXPECT_EQ(board.at(0, 0), Player::None);
    board.dropStone(0, Player::Player1);
    EXPECT_EQ(board.at(0, 0), Player::Player1);
    EXPECT_EQ(board.at(1, 0), Player::None);
    EXPECT_EQ(board.at(0, 1), Player::None);
    board.dropStone(1, Player::Player2);
    EXPECT_EQ(board.at(0, 1), Player::Player2);
    EXPECT_EQ(board.at(0, 0), Player::Player1);
    EXPECT_EQ(board.at(1, 0), Player::None);
    board.dropStone(0, Player::Player1);
    EXPECT_EQ(board.at(0, 0), Player::Player1);
    EXPECT_EQ(board.at(1, 0), Player::Player1);
    EXPECT_EQ(board.at(0, 1), Player::Player2);

    board.dropStone(0, Player::Player1);
    board.dropStone(0, Player::Player1);
    board.dropStone(0, Player::Player1);
    EXPECT_TRUE(board.canPlay(0));
    board.dropStone(0, Player::Player1);
    EXPECT_FALSE(board.canPlay(0));
}

TEST(Board, Full) {
    using namespace play::connectfour;
    using namespace play::game;

    Board board;
    for (int row = 0; row < board.rows(); ++row) {
        for (int col = 0; col < board.columns(); ++col) {
            if (row == board.rows() - 1 && col == board.columns() - 1)
                continue;
            board.dropStone(col, Player::Player1);
            EXPECT_FALSE(board.isFull());
        }
    }
    board.dropStone(board.columns() - 1, Player::Player1);
    EXPECT_TRUE(board.isFull());
}

/*
   . . . . . . .
   . . . . . . .
   . . O . . . .
   . . O . . O X  
   . X O . O X X
   . X X . X O X
*/
play::connectfour::Board setupBoard() {
    using namespace play::connectfour;
    using namespace play::game;
    static constexpr Player X = Player::Player1;
    static constexpr Player O = Player::Player2;

    Board board;
    board.dropStone(1, X);
    board.dropStone(1, X);
    board.dropStone(2, X);
    board.dropStone(2, O);
    board.dropStone(2, O);
    board.dropStone(2, O);
    board.dropStone(4, X);
    board.dropStone(4, O);
    board.dropStone(5, O);
    board.dropStone(5, X);
    board.dropStone(5, O);
    board.dropStone(6, X);
    board.dropStone(6, X);
    board.dropStone(6, X);
    return board;
}

TEST(Board, Winner) {
    using namespace play::connectfour;
    using namespace play::game;

    Board board = setupBoard();
    for (int col = 0; col < board.columns(); ++col) {
        EXPECT_FALSE(board.checkWin(col));
    }   
    board.dropStone(1, Player::Player1);
    for (int col = 0; col < board.columns(); ++col) {
        EXPECT_FALSE(board.checkWin(col));
    }
    board.dropStone(1, Player::Player1);
    EXPECT_TRUE(board.checkWin(1));
    for (int i = 0; i < board.columns(); ++i) {
        if (i == 1)
            continue;
        EXPECT_FALSE(board.checkWin(i));
    }
    board.dropStone(3, Player::Player1);
    EXPECT_TRUE(board.checkWin(3));
    board.dropStone(3, Player::Player2);
    EXPECT_FALSE(board.checkWin(3));
    board.dropStone(3, Player::Player2);
    EXPECT_TRUE(board.checkWin(3));
    board.dropStone(6, Player::Player2);
    EXPECT_FALSE(board.checkWin(6));
}