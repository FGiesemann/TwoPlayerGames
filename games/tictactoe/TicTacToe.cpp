/* *********************************************************** *
 * TicTacToe
 * TicTacToe.cpp
 * *********************************************************** */

#include "TicTacToe.h"
#include <iostream>
#include <algorithm>

namespace play::tictactoe {

GameState GameState::newGame() {
    return GameState{};
}

GameState::GameState(const Board& board, const play::game::Player& player) :
    m_board{ board }, m_activePlayer{ player } {
}

bool GameState::isOver() const {
    return m_board.isFull() || winner() != play::game::Player::None;
}

const play::game::Player& GameState::winner() const {
    return m_board.detectWinner();
}

bool GameState::isLegalMove(const Move& move) const {
    return m_board.isOnBoard(move.point()) && m_board[move.point()] == play::game::Player::None;
}

std::vector<Move> GameState::availableMoves() const {
    std::vector<Move> moves;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            if (m_board[Point{ row, col }] == play::game::Player::None)
                moves.emplace_back(Point{ row, col });
        }
    }
    return moves;
}

GameState GameState::applyMove(const Move& move) const {
    if (!isOver() && m_board.isOnBoard(move.point()) && m_board[move.point()] == play::game::Player::None) {
        Board nextBoard{ m_board };
        nextBoard.placeMark(move.point(), m_activePlayer);
        return GameState(nextBoard, m_activePlayer.other());
    } else
        return *this;
}

bool Board::isOnBoard(const Point& p) const {
    return p.row() >= 0 && p.row() < 3 && p.col() >= 0 && p.col() < 3;
}

const play::game::Player& Board::operator[](const Point& p) const {
    if (isOnBoard(p))
        return m_marks[p.linearIndex()];
    else
        return play::game::Player::None;
}

bool Board::placeMark(const Point& point, const play::game::Player& player) {
    if (isOnBoard(point)) {
        m_marks[point.linearIndex()] = player;
        return true;
    } else {
        return false;
    }
}

bool Board::isFull() const {
    return std::none_of(m_marks.begin(), m_marks.end(), [](const play::game::Player& p) { return p == play::game::Player::None; });
}

const play::game::Player& Board::detectWinner() const {
    if (isWinner(play::game::Player::Player1))
        return play::game::Player::Player1;
    else if (isWinner(play::game::Player::Player2))
        return play::game::Player::Player2;
    else
        return play::game::Player::None;
}

bool Board::isWinner(const play::game::Player& player) const {
    for (int i = 0; i < 3; ++i)
        if (playerWinsRow(player, i) || playerWinsCol(player, i))
            return true;
    return playerWinsDiag(player);
}

bool Board::playerWinsRow(const play::game::Player& player, int row) const {
    for (int col = 0; col < 3; ++col)
        if ((*this)[Point{ row, col }] != player)
            return false;
    return true;
}

bool Board::playerWinsCol(const play::game::Player& player, int col) const {
    for (int row = 0; row < 3; ++row)
        if ((*this)[Point{ row, col }] != player)
            return false;
    return true;
}

bool Board::playerWinsDiag(const play::game::Player& player) const {
    bool diag1 = true, diag2 = true;
    for (int i = 0; i < 3; ++i) {
        if ((*this)[Point{ i, i }] != player)
            diag1 = false;
        if ((*this)[Point{ i, 2 - i }] != player)
            diag2 = false;
    }
    return diag1 || diag2;
}

namespace {
char boardMarker(const play::game::Player& player) {
    if (player == play::game::Player::Player1)
        return 'X';
    else if (player == play::game::Player::Player2)
        return 'O';
    else
        return ' ';
}
}

std::ostream& operator<<(std::ostream& ostr, const Board& board) {
    ostr << "    0   1   2\n";
    ostr << "  +---+---+---+\n";
    for (int row = 0; row < 3; ++row) {
        ostr << row << ' ';
        for (int col = 0; col < 3; ++col) {
            ostr << "| " << boardMarker(board[Point{ row, col }]) << ' ';
        }
        ostr << "|\n  +---+---+---+\n";
    }
    return ostr;
}

// ---- Interface to game library

std::vector<Move> listLegalMoves(const GameState& game) { 
    return game.availableMoves(); 
}

bool isLegalMove(const Move& move, const GameState& game) {
    return game.isLegalMove(move);
}

GameState applyMove(const Move& move, const GameState& game) {
    return game.applyMove(move);
}

bool isGameOver(const GameState& game) {
    return game.isOver();
}

play::game::Player getActivePlayer(const GameState& game) {
    return game.activePlayer();
}

const play::game::Player& getWinner(const GameState& game) {
    return game.winner();
}

Move askForMove(const GameState& state) {
    std::cout << "Enter move: <row> <col>: ";
    int row, col;
    std::cin >> row >> col;
    return Move{ { row, col } };
}

std::ostream& operator<<(std::ostream& ostr, const Move& move) {
    const auto& point = move.point();
    ostr << point.row() << ", " << point.col();
    return ostr;
}

std::ostream& operator<<(std::ostream& ostr, const GameState& game) {
    ostr << game.board();
    return ostr;
}

}
