/* *********************************************************** *
 * GameStateEvaluator.h
 * *********************************************************** */

#ifndef GAME_GAME_STATE_EVALUATOR_H
#define GAME_GAME_STATE_EVALUATOR_H

namespace play::game {

template<class EvalType, class GameState>
class GameStateEvaluator {
public:
    virtual EvalType evaluateGameState(const GameState& gameState) = 0;

    virtual EvalType lowerBound() const = 0;
    virtual EvalType upperBound() const = 0;

    virtual ~GameStateEvaluator() = default;
};

template<class GameState>
class BasicIntEvaluator : public GameStateEvaluator<int, GameState> {
public:
    int evaluateGameState(const GameState& gameState) override {
        const auto& winner = getWinner(gameState);
        if (winner == getActivePlayer(gameState))
            return 1;
        else if (winner == getActivePlayer(gameState).other())
            return -1;
        else
            return 0;
    }

    int lowerBound() const override {
        return -1;
    }

    int upperBound() const override {
        return 1;
    }
};

}

#endif
