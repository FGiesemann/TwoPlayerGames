/* *********************************************************** *
 * MCTSPlayer.h
 * *********************************************************** */

#ifndef AGENT_MCTS_PLAYER_H
#define AGENT_MCTS_PLAYER_H

#include <random>
#include <cmath>
#include <algorithm>
#include <numeric>

#include "RandomPlayer.h"
#include "../random_selection.h"
#include "../gameplay/Player.h"

namespace play::agent {

namespace {

template<class GameState, class Move>
class MCTSNode {
public:
    MCTSNode(const GameState& state, MCTSNode* parent) : parent{ parent },
        state{ state }, availableMoves{ listLegalMoves(state) }
    {
        /* List of legal moves is shuffled here, so that we can pick them in sequential order later.
         * With the counter evaluatedMoves we count how many moves from the beginning of the (shuffeled) list
         * have already been evaluated. If evaluatedMoves == availableMoves.size(), all children of the node 
         * have been visited.
         */
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(availableMoves.begin(), availableMoves.end(), g);
        player = std::make_unique<play::agent::RandomPlayer<GameState, Move>>();
    }

    void evaluateMoves() {
        MCTSNode* node = selectNode();
        const auto& winner = node->performRollout();
        propagateWinner(node, winner);
    }

    std::vector<Move> getBestMoves() const {
        float winPct = -1;
        std::vector<Move> bestMoves;
        for (int i = 0; i < evaluatedMoves; ++i) {
            const auto& child = children[i];
            float win = child->computeWinningPct(getActivePlayer(state));
            if (win > winPct) {
                winPct = win;
                bestMoves.clear();
                bestMoves.push_back(availableMoves[i]);
            }
        }
        return bestMoves;
    }
private:
    MCTSNode* parent{ nullptr };
    GameState state;
    std::vector<Move> availableMoves;
    int evaluatedMoves{ 0 };
    std::vector<std::unique_ptr<MCTSNode>> children;
    int player1Wins{ 0 }, player2Wins{ 0 };
    int numRollouts{ 0 };
    float temperature{ 1.5f };

    std::unique_ptr<play::agent::RandomPlayer<GameState, Move>> player;
    random_selector<> selector{};

    MCTSNode* addChild() {
        GameState newState = applyMove(availableMoves[evaluatedMoves], state);
        children.push_back(std::make_unique<MCTSNode>(newState, this));
        ++evaluatedMoves;
        return children.back().get();
    }

    void recordWin(const play::game::Player& player) {
        if (player == play::game::Player::Player1)
            ++player1Wins;
        else if (player == play::game::Player::Player2)
            ++player2Wins;
        ++numRollouts;
    }

    bool canAddChild() {
        return evaluatedMoves < availableMoves.size();
    }

    bool isTerminal() {
        return isGameOver(state);
    }

    MCTSNode* selectNode() {
        MCTSNode* node = this;
        while (!node->canAddChild() && !node->isTerminal())
            node = node->selectChild();
        if (node->canAddChild())
            node = node->addChild();
        return node;
    }

    static float computeUCTScore(int totalRollouts, int rollouts, float winPct, float temperature) {
        return winPct + static_cast<float>(temperature * std::sqrt(std::log(totalRollouts) / rollouts));
    }

    float computeWinningPct(const play::game::Player& player) const {
        if (numRollouts == 0)
            return 0.0f;

        if (player == play::game::Player::Player1)
            return static_cast<float>(player1Wins) / static_cast<float>(numRollouts);
        else
            return static_cast<float>(player2Wins) / static_cast<float>(numRollouts);
    }

    MCTSNode* selectChild() {
        float bestScore = -1;
        MCTSNode* bestChild = nullptr;
        int childrenTotalRollouts = std::accumulate(children.begin(), children.end(), 0, [](int a, std::unique_ptr<MCTSNode>& child) {return a + child->numRollouts; });
        for (auto& child : children) {
            float winPct = child->computeWinningPct(getActivePlayer(state).other());
            float score = computeUCTScore(childrenTotalRollouts, child->numRollouts, winPct, temperature);
            if (score > bestScore) {
                bestScore = score;
                bestChild = child.get();
            }
        }
        return bestChild;
    }

    const play::game::Player& performRollout() {
        GameState game = state;
        while (!isGameOver(game)) {
            auto moves = player->selectMoves(game);
            const auto& m = selector(moves);
            game = applyMove(m, game);
        }
        return getWinner(game);
    }

    void propagateWinner(MCTSNode* node, const play::game::Player& winner) {
        while (node) {
            node->recordWin(winner);
            node = node->parent;
        }
    }

};

}

template<class GameState, class Move, int rollouts=10000>
class MCTSPlayer : public Agent<GameState, Move> {
public:
    std::vector<Move> selectMoves(const GameState& state) override {
        MCTSNode<GameState, Move> root{ state, nullptr };
        for (int i = 0; i < rollouts; ++i) {
            root.evaluateMoves();
        }
        return root.getBestMoves();
    }
};

}

#endif
