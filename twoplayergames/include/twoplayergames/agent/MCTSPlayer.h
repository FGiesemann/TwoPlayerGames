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
    MCTSNode(const GameState &state) : MCTSNode(state, nullptr) {}

    void evaluateMoves() {
        auto node = selectMCTSNode();
        node = node->expand();
        auto result = node->simulate();
        node->backpropagate(result);
    }

    std::vector<Move> getBestMoves() const {
        int visits{0};
        std::vector<Move> moves;
        for (int i = 0; i < children.size(); ++i) {
            const auto* c = children[i].get();
            if (c->numVisits > visits) {
                visits = c->numVisits;
                moves.clear();
                moves.push_back(availableMoves[i]);
            } else if (c->numVisits == visits) {
                moves.push_back(availableMoves[i]);
            }
        }
        return moves;
    }

    MCTSNode(const GameState &state, MCTSNode *parent) : 
        parent{parent}, state{state}, availableMoves{ listLegalMoves(state) } {
        std::shuffle(availableMoves.begin(), availableMoves.end(), g);
    }

private:
    MCTSNode* parent{nullptr};
    GameState state;
    std::vector<Move> availableMoves;
    std::vector<std::unique_ptr<MCTSNode>> children;
    int evaluatedMoves{0};
    int numVisits{0};
    int wins{0}, losses{0};
    float temperature{1.4f};

    static std::random_device rd;
    static std::mt19937 g;

    bool isFullyExpanded() const {
        return evaluatedMoves == availableMoves.size();
    }

    bool isTerminal() const {
        return isGameOver(state);
    }

    MCTSNode* selectMCTSNode() {
        MCTSNode *n = this;
        while (n->isFullyExpanded() && !n->isTerminal())
            n = n->selectChildNode();
        return n;
    }

    float computeUCTScore() {
        auto v = static_cast<float>(numVisits);
        auto pv = static_cast<float>(parent->numVisits);
        auto winPct = static_cast<float>(wins - losses) / v;
        return winPct + temperature * std::sqrt(2 * std::log(pv) / v);
    }

    MCTSNode* selectChildNode() const {
        auto child = std::max_element(children.begin(), children.end(), [&](const auto& c1, const auto& c2){
            const auto uct_c1 = c1->computeUCTScore();
            const auto uct_c2 = c2->computeUCTScore();
            return uct_c1 < uct_c2;
        });
        return child->get();
    }

    MCTSNode* expand() {
        if (isTerminal()) {
            return this;
        }

        const auto move = availableMoves[evaluatedMoves];
        ++evaluatedMoves;
        GameState newState = applyMove(move, state);
        children.push_back(std::make_unique<MCTSNode>(newState, this));
        return children.back().get();        
    }

    play::game::Player simulate() const {
        GameState game = state;
        play::agent::RandomPlayer<GameState, Move> player;
        random_selector<> selector{};
        while (!isGameOver(game)) {
            auto moves = player.selectMoves(game);
            const auto& move = selector(moves);
            game = applyMove(move, game);
        }
        return getWinner(game);
    }

    void backpropagate(const play::game::Player &player) {
        if (player == getActivePlayer(state).other()) {
            ++wins;
        } else if (player == getActivePlayer(state)) {
            ++losses;
        }

        ++numVisits;
        if (parent)
            parent->backpropagate(player);
    }
};

template<class GameState, class Move>
std::random_device MCTSNode<GameState, Move>::rd;

template<class GameState, class Move>
std::mt19937 MCTSNode<GameState, Move>::g{MCTSNode<GameState, Move>::rd()};

}

template<class GameState, class Move, int rollouts=2000>
class MCTSPlayer : public Agent<GameState, Move> {
public:
    std::vector<Move> selectMoves(const GameState& state) override {
        MCTSNode<GameState, Move> root{ state };
        for (int i = 0; i < rollouts; ++i) {
            root.evaluateMoves();
        }
        return root.getBestMoves();
    }
};

}

#endif
