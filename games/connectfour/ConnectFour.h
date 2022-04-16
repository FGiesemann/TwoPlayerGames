/* *********************************************************** *
 * ConnectFour
 * ConnectFour.h
 * *********************************************************** */

#ifndef CONNECT_FOUR_H
#define CONNECT_FOUR_H

#include <vector>
#include <iostream>
#include "twoplayergames/gameplay/Player.h"
#include "twoplayergames/gameplay/GameStateEvaluator.h"

namespace play::connectfour {

class Board {
public:
    Board(int rows = 6, int columns = 7);

    const play::game::Player& at(int row, int col) const;
    void dropStone(int column, const play::game::Player& player);
    bool canPlay(int column) const;
    bool isFull() const;

    int rows() const { return m_rows; }
    int columns() const { return m_columns; }

    bool checkWin(int column) const;

private:
    std::vector<play::game::Player> m_stones;
    std::vector<int> m_columnHeights;
    int m_rows, m_columns;

    bool isValidCol(int column) const { return column >= 0 && column < m_columns; }
    bool isValidRow(int row) const { return row >= 0 && row < m_rows; }
    bool isColumnFull(int column) const { return m_columnHeights[column] == m_rows; }

    int linearIndex(int row, int col) const;
    int countRun(int row, int col, const play::game::Player& player, int dRow, int dCol) const;

    friend class ConnectFourEvaluator_Streaks;
};

std::ostream& operator<<(std::ostream& ostr, const Board& board);

class GameState {
public:
    static GameState newGame(int rows = 6, int columns = 7);

    GameState dropStone(int column) const;
    const play::game::Player& activePlayer() const { return m_activePlayer; }
    bool isOver() const;
    std::vector<int> availableMoves() const;
    const Board& board() const { return m_board; }
    const play::game::Player& winner() const;
private:
    GameState(int rows, int columns);
    GameState(const Board& board, const play::game::Player& nextPlayer, bool isWinninState);
    GameState(Board&& board, const play::game::Player& nextPlayer, bool isWinningState);

    Board m_board{};
    play::game::Player m_activePlayer{ play::game::Player::Player1 };
    bool m_isWinningState{ false };
};

// ---- Interface to game library

std::vector<int> listLegalMoves(const GameState& game);
int askForMove(const GameState& state);
bool isLegalMove(int col, const GameState& game);
std::ostream& operator<<(std::ostream& ostr, const GameState& game);
GameState applyMove(int col, const GameState& game);

const play::game::Player& getActivePlayer(const GameState& game);
const play::game::Player& getWinner(const GameState& game);
bool isGameOver(const GameState& game);

class ConnectFourEvaluator_Streaks : public play::game::GameStateEvaluator<int, GameState> {
    /* Evaluate game state based on runs of stones of the same player.
     * Adapted from an implementation by prakhar10 [https://github.com/prakhar10/Connect4]
     */
public:
    int evaluateGameState(const GameState& gameState) override;
    
    int lowerBound() const override { return loosingValue; }
    int upperBound() const override { return winningValue; }
private:
    static constexpr int winningValue = 10000;
    static constexpr int loosingValue = -10000;

    int checkForStreak(const GameState& game, const play::game::Player& player, int length);
    int checkVerticalStreak(int row, int col, const Board& board, int length);
    int checkHorizontalStreak(int row, int col, const Board& board, int length);
    int checkDiagonalStreak(int row, int col, const Board& board, int length);
};

}

#endif

