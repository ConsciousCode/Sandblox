/**
 * Define the errors used by the game.
**/
#ifndef SANDBLOX_ERROR_H
#define SANDBLOX_ERROR_H

#include <stdexcept>
#include <string>

/**
 * Base error class of all game-related errors
**/
struct GameError:public std::runtime_error{
	GameError(const std::string& s):std::runtime_error(s){}
};

/**
 * Thrown when ending the game.
**/
struct EndGameError:public GameError{};

/**
 * Thrown during game initialization.
**/
struct InitError:public GameError{
	InitError(const std::string& s):GameError(s){}
};

/**
 * Thrown when something happens during function hooking.
 *
 * Throwing this means something is seriously wrong with the game, which will likely terminate.
**/
struct HookError:public InitError{
	HookError(const std::string& s):InitError(s){}
};

#endif
