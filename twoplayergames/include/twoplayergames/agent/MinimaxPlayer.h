/* *********************************************************** *
 * MinimaxPlayer.h
 * *********************************************************** */

#ifndef AGENT_MINIMAX_PLAYER_H
#define AGENT_MINIMAX_PLAYER_H

#include "Agent.h"
#include "../gameplay/Player.h"
#include "../gameplay/GameStateEvaluator.h"
#include <vector>

namespace play::agent {

template<class GameState, class Move, class EvaluatorType = play::game::BasicIntEvaluator<GameState>>
class MinimaxPlayer : public Agent<GameState, Move> {
public:
    MinimaxPlayer(int maxDepth = -1) : maxDepth{ maxDepth } {
        evaluator = std::make_unique<EvaluatorType>();
    }

    std::vector<Move> selectMoves(const GameState& game) override {
        std::vector<Move> bestMoves;
        int bestValue = evaluator->lowerBound();
        int alpha = evaluator->lowerBound();
        
        const auto legalMoves = listLegalMoves(game);
        for (const auto& move : legalMoves) {
            GameState state = applyMove(move, game);
            int value = -evaluateGame(state, maxDepth, alpha, evaluator->upperBound());
            if (value > bestValue) {
                bestValue = value;
                bestMoves.clear();
                bestMoves.push_back(move);
            } else if (value == bestValue) {
                bestMoves.push_back(move);
            }
        }

        return bestMoves;
    }

private:
    int maxDepth{ -1 };
    std::unique_ptr<EvaluatorType> evaluator;

    template<class EvalType>
    EvalType evaluateGame(const GameState& game, int depth, EvalType alpha, EvalType beta) {
        if (depth == 0 || isGameOver(game)) {
            return evaluator->evaluateGameState(game);
        } else {
            if (depth > 0)
                --depth;
            const auto legal = listLegalMoves(game);
            EvalType bestValue = evaluator->lowerBound();
            for (const auto& move : legal) {
                GameState state = applyMove(move, game);
                EvalType value = -evaluateGame(state, depth, -beta, -alpha);
                bestValue = std::max(bestValue, value);
                alpha = std::max(alpha, bestValue);
                if (alpha >= beta)
                    return bestValue;
            }
            return bestValue;
        }
    }
};

}

#endif
