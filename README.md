# Two Player Games

Experimental implementation of different game AI algorithms for simple two-player (board) games.

This repository contains the implementation of two-player games and the `twoplayergames` library.
The library provides

- implementations of agents that use different AI algorithms to select moves,
- basic infrastructure to play games against the agents on the console.

## Implementation of games

Different games are implemented in their own subfolder in `games`. In order to be used by the game library, the game implementation should provide at least two classes:

- a class that manages the current state of the game (e.g., `GameState`)
- a class that represents a move that a player can take.

There is no need to derive the classes from any library-provided classes, as the library is templated and can use any two classes for this purpose. The interface with the library is provided via free functions that allow the library to query different aspects about the game and the current state. Which functions are needed may depend on the agent you want to use.

## The game library

The header-only library can be found in the `twoplayergames` subfolder. It provides basic tools for implementing games in the `gameplay` subfolder and the implementation of different AI algorithms in `agent`. An agent has to be derived from the `Agent` base class.

The library makes use of some functions that have to be provided by the game implementation as free functions. The exact set of functions depends on the agent you want to use and the game infrastructure, if any. For example, the `ConsoleGame` expects the following functions:

- a static function `newGame` in the `GameState` class to create the initial game state for a new game,
- a function `bool isGameOver(const GameState&)`,
- a function `GameState applyMove(const Move&, const GameState&)`,
- a function `const play::game::Player& getWinner(const GameState&);`,
- an operator `std::ostream& operator<<(std::ostream&, const Move&)`,
- an operator `std::ostream& operator<<(std::ostream&, const GameState&)`.

The `InteractivePlayer` agent makes use of these function:

- `Move askForMove(const GameState&)`,
- `bool isLegalMove(const Move&, const GameState&)`,

while the `RandomPlayer` need this function:

- `std::vector<Move> listLegalMoves(const GameState&)`.

More details can be found in the code or are provided by the compiler.
