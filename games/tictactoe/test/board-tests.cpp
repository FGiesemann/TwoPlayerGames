#include "gtest/gtest.h"
#include "tictactoe/TicTacToe.h"

TEST(Board, Stones) {
    using namespace play::tictactoe;
    using namespace play::game;

    Board b;
    EXPECT_EQ((b[Point{ 0, 0 }]), Player::None);
    EXPECT_TRUE(b.placeMark(Point{ 0,0 }, Player::Player1));
    EXPECT_EQ((b[Point{ 0, 0 }]), Player::Player1);
    
    EXPECT_TRUE(b.placeMark(Point{ 0, 0 }, Player::Player2)); // marks can be overwritten
    EXPECT_EQ((b[Point{ 0, 0 }]), Player::Player2);

    EXPECT_EQ((b[Point{ 1, 0 }]), Player::None);
    EXPECT_TRUE(b.placeMark(Point{ 1, 0 }, Player::Player2));

    EXPECT_TRUE(b.isOnBoard(Point{ 2, 1 }));
    EXPECT_FALSE(b.placeMark(Point{ 3, 0 }, Player::Player2)); // not on board
}

TEST(Board, Winning) {
    using namespace play::tictactoe;
    using namespace play::game;

    {
        Board b;
        EXPECT_EQ(b.detectWinner(), Player::None);

        b.placeMark(Point{ 0, 0 }, Player::Player1);
        b.placeMark(Point{ 0, 1 }, Player::Player1);
        b.placeMark(Point{ 0, 2 }, Player::Player1);
        EXPECT_EQ(b.detectWinner(), Player::Player1);
    }

    {
        Board b;
        b.placeMark(Point{ 0, 1 }, Player::Player2);
        b.placeMark(Point{ 1, 1 }, Player::Player2);
        b.placeMark(Point{ 2, 1 }, Player::Player2);
        EXPECT_EQ(b.detectWinner(), Player::Player2);
    }

    {
        Board b;
        b.placeMark(Point{ 0, 0 }, Player::Player2);
        b.placeMark(Point{ 1, 1 }, Player::Player2);
        b.placeMark(Point{ 2, 2 }, Player::Player2);
        EXPECT_EQ(b.detectWinner(), Player::Player2);
    }

    {
        Board b;
        b.placeMark(Point{ 0, 0 }, Player::Player2);
        b.placeMark(Point{ 0, 1 }, Player::Player2);
        b.placeMark(Point{ 0, 2 }, Player::Player1);
        EXPECT_EQ(b.detectWinner(), Player::None);

        b.placeMark(Point{ 1, 1 }, Player::Player2);
        b.placeMark(Point{ 2, 2 }, Player::Player2);
        EXPECT_EQ(b.detectWinner(), Player::Player2);
    }
}

TEST(Board, Full) {
    using namespace play::tictactoe;
    using namespace play::game;

    Board b;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            EXPECT_FALSE(b.isFull());
            b.placeMark(Point{ row, col }, Player::Player1);
        }
    }
    EXPECT_TRUE(b.isFull());
}
