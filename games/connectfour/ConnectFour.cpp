/* *********************************************************** *
 * ConnectFour
 * ConnectFour.cpp
 * *********************************************************** */

#include "ConnectFour.h"

#include <algorithm>
#include <array>

namespace play::connectfour {

Board::Board(int rows, int columns) :
    m_rows{ rows }, m_columns{ columns },
    m_stones(columns * rows, play::game::Player::None),
    m_columnHeights(columns, 0) {
}

int Board::linearIndex(int row, int col) const { 
    return row * m_columns + col; 
}

const play::game::Player& Board::at(int row, int col) const {
    if (isValidRow(row) && isValidCol(col))
        return m_stones[linearIndex(row, col)];
    else
        return play::game::Player::None;
}

bool Board::canPlay(int column) const {
    return isValidCol(column) && !isColumnFull(column);
}

void Board::dropStone(int column, const play::game::Player& player) {
    if (canPlay(column)) {
        auto landingRow = m_columnHeights[column];
        auto stoneIndex = linearIndex(landingRow, column);
        m_stones[stoneIndex] = player;
        ++m_columnHeights[column];
    }
}

bool Board::isFull() const {
    return std::all_of(m_columnHeights.begin(), m_columnHeights.end(), [this](int i) { return i == m_rows; });
}

bool Board::checkWin(int column) const {
    int row = m_columnHeights[column] - 1;
    const auto& player = at(row, column);
    if (player == play::game::Player::None)
        return false;

    int count_left = countRun(row, column - 1, player, 0, -1);
    if (count_left >= 3)
        return true;
    int count_right = countRun(row, column + 1, player, 0, 1);
    if (count_left + count_right >= 3)
        return true;

    int count_down = countRun(row - 1, column, player, -1, 0);
    if (count_down >= 3)
        return true;

    int count_lu = countRun(row + 1, column - 1, player, +1, -1);
    if (count_lu >= 3)
        return true;
    int count_rd = countRun(row - 1, column + 1, player, -1, +1);
    if (count_lu + count_rd >= 3)
        return true;

    int count_ru = countRun(row + 1, column + 1, player, +1, +1);
    if (count_ru >= 3)
        return true;
    int count_ld = countRun(row - 1, column - 1, player, -1, -1);
    if (count_ru + count_ld >= 3)
        return true;

    return false;
}

int Board::countRun(int row, int col, const play::game::Player& player, int dRow, int dCol) const {
    int count{ 0 };
    while (isValidRow(row) && isValidCol(col) && at(row, col) == player) {
        ++count;
        row += dRow;
        col += dCol;
    }
    return count;
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
    for (int col = 0; col < board.columns(); ++col)
        ostr << "  " << col << ' ';
    ostr << '\n';
    for (int row = 0; row < board.rows(); ++row) {
        for (int col = 0; col < board.columns(); ++col)
            ostr << "+---";
        ostr << "+\n";
        for (int col = 0; col < board.columns(); ++col) {
            ostr << "| " << boardMarker(board.at(board.rows() - row - 1, col)) << ' ';
        }
        ostr << "|\n";
    }
    for (int col = 0; col < board.columns(); ++col)
        ostr << "+---";
    ostr << "+\n";
    return ostr;
}

GameState::GameState(const Board& board, const play::game::Player& nextPlayer, bool isWinningState) :
    m_board{ board }, m_activePlayer{ nextPlayer }, m_isWinningState{ isWinningState } {
}

GameState::GameState(Board&& board, const play::game::Player& nextPlayer, bool isWinningState) :
    m_board{ std::move(board) }, m_activePlayer{ nextPlayer }, m_isWinningState{ isWinningState } {
}

GameState GameState::newGame(int rows, int columns) {
    return GameState{rows, columns};
}

GameState::GameState(int rows, int columns) :
    m_board{ rows, columns } {
}

bool GameState::isOver() const {
    return m_isWinningState || m_board.isFull();
}

GameState GameState::dropStone(int column) const {
    if (m_board.canPlay(column)) {
        Board newBoard{ m_board };
        newBoard.dropStone(column, m_activePlayer);
        const auto isWinningState = newBoard.checkWin(column);
        return GameState{ std::move(newBoard), m_activePlayer.other(), isWinningState };
    } else
        return *this;
}

std::vector<int> GameState::availableMoves() const {
    std::vector<int> legalMoves;
    legalMoves.reserve(m_board.columns());
    for (int col = 0; col < m_board.columns(); ++col)
        if (m_board.canPlay(col))
            legalMoves.push_back(col);
    return legalMoves;
}

const play::game::Player& GameState::winner() const {
    if (m_isWinningState)
        return m_activePlayer.other();
    else
        return play::game::Player::None;
}

// ---- Interface to game library

std::vector<int> listLegalMoves(const GameState& game) {
    return game.availableMoves();
}

int askForMove(const GameState& game) {
    std::cout << "Enter column: ";
    int col;
    std::cin >> col;
    return col;
}

bool isLegalMove(int col, const GameState& game) {
    return game.board().canPlay(col);
}

std::ostream& operator<<(std::ostream& ostr, const GameState& game) {
    ostr << game.board();
    return ostr;
}

GameState applyMove(int col, const GameState& game) {
    return game.dropStone(col);
}

const play::game::Player& getActivePlayer(const GameState& game) {
    return game.activePlayer();
}

const play::game::Player& getWinner(const GameState& game) {
    return game.winner();
}

bool isGameOver(const GameState& game) {
    return game.isOver();
}

int ConnectFourEvaluator_Streaks::evaluateGameState(const GameState& game) {
    if (const auto& winner = game.winner(); winner == game.activePlayer())
        return winningValue;
    else if (winner == game.activePlayer().other())
        return loosingValue;
    else {
        std::array<int, 2> player1_streaks, player2_streaks;
        for (int length = 0; length < 2; ++length) {
            player1_streaks[length] = checkForStreak(game, play::game::Player::Player1, 3 - length);
            player2_streaks[length] = checkForStreak(game, play::game::Player::Player2, 3 - length);
        }
        int player1_sum = 10 * player1_streaks[0] + 5 * player1_streaks[1];
        int player2_sum = 10 * player2_streaks[0] + 5 * player2_streaks[1];
        int value = (getActivePlayer(game) == play::game::Player::Player1) ? player1_sum - player2_sum : player2_sum - player1_sum;
        return value;
    }
}

int ConnectFourEvaluator_Streaks::checkForStreak(const GameState& game, const play::game::Player& player, int length) {
    int count = 0;
    for (int row = 0; row < game.board().rows(); ++row) {
        for (int col = 0; col < game.board().columns(); ++col) {
            if (game.board().at(row, col) == player) {
                count += checkVerticalStreak(row, col, game.board(), length);
                count += checkHorizontalStreak(row, col, game.board(), length);
                count += checkDiagonalStreak(row, col, game.board(), length);
            }
        }
    }
    return count;
}

int ConnectFourEvaluator_Streaks::checkVerticalStreak(int row, int col, const Board& board, int length) {
    int count = board.countRun(row, col, board.at(row, col), 1, 0);
    return count >= length ? 1 : 0;
}

int ConnectFourEvaluator_Streaks::checkHorizontalStreak(int row, int col, const Board& board, int length) {
    int count = board.countRun(row, col, board.at(row, col), 0, 1);
    return count >= length ? 1 : 0;
}

int ConnectFourEvaluator_Streaks::checkDiagonalStreak(int row, int col, const Board& board, int length) {
    int count = board.countRun(row, col, board.at(row, col), -1, 1);
    int total = count >= length ? 1 : 0;
    count = board.countRun(row, col, board.at(row, col), 1, 1);
    total += count >= length ? 1 : 0;
    return total;
}

}
