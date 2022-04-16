/* *********************************************************** *
 * TicTacToe
 * TicTacToe.h
 * *********************************************************** */

#ifndef TICTACTOE_BOARD_H
#define TICTACTOE_BOARD_H

#include <array>
#include <vector>
#include <memory>

#include "twoplayergames/gameplay/Player.h"

namespace play::tictactoe {

class Point {
public:
    constexpr Point(int row, int col) : m_r{ row }, m_c{ col } {};

    constexpr int row() const { return m_r; }
    constexpr int col() const { return m_c; }

    constexpr int linearIndex() const { return m_r * 3 + m_c; }
private:
    int m_r, m_c;
};

class Move {
public:
    constexpr Move(const Point& p) : m_p{ p } {};
    const Point& point() const { return m_p; }
private:
    Point m_p;
};

class Board {
public:
    bool isOnBoard(const Point& p) const;
    const play::game::Player& operator[](const Point& p) const;
    bool placeMark(const Point& point, const play::game::Player& player);
    bool isFull() const;

    const play::game::Player& detectWinner() const;
private:
    std::array<play::game::Player, 9> m_marks{
        play::game::Player::None, play::game::Player::None, play::game::Player::None,
        play::game::Player::None, play::game::Player::None, play::game::Player::None,
        play::game::Player::None, play::game::Player::None, play::game::Player::None };

    bool isWinner(const play::game::Player& player) const;
    bool playerWinsRow(const play::game::Player& player, int row) const;
    bool playerWinsCol(const play::game::Player& player, int col) const;
    bool playerWinsDiag(const play::game::Player& player) const;

    friend std::ostream& operator<<(std::ostream& ostr, const Board& board);
};

class GameState {
public:
    static GameState newGame();

    bool isOver() const;
    const play::game::Player& winner() const;
    const play::game::Player& activePlayer() const { return m_activePlayer; }
    
    std::vector<Move> availableMoves() const;
    GameState applyMove(const Move& move) const;
    bool isLegalMove(const Move& move) const;
    const Board& board() const { return m_board; }
private:
    GameState() = default;
    GameState(const Board& board, const play::game::Player& next_player);

    Board m_board{};
    play::game::Player m_activePlayer{ play::game::Player::Player1 };
};

// ---- Interface to game library

std::vector<Move> listLegalMoves(const GameState& game);
bool isLegalMove(const Move& move, const GameState& game);
GameState applyMove(const Move& move, const GameState& game);
bool isGameOver(const GameState& game);
play::game::Player getActivePlayer(const GameState& game);
const play::game::Player& getWinner(const GameState& game);
Move askForMove(const GameState& state);
std::ostream& operator<<(std::ostream& ostr, const Move& move);
std::ostream& operator<<(std::ostream& ostr, const GameState& game);

}

#endif
