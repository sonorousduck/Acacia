#pragma once

#include <misc/actionSpaces.hpp>

#include <pybind11/stl.h>

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

#include <singletons/time.hpp>
#include <iostream>
#include <singletons/systemManager.hpp>

namespace BrickBreaker
{
	struct State
	{
		State()
		{}

		State(Ebony::Box paddlePosition) : paddlePosition(paddlePosition) {}
		
		Ebony::Box paddlePosition{};
		Ebony::Box ballPosition{};
		std::vector<Ebony::Box> brickPositions{}; // Will only have ~5 of these
		std::vector<Ebony::Box> powerupPositions{}; // Will only have 1-2 of these

		std::vector<Ebony::Box> getBrickPositions() const { return brickPositions; }
		std::vector<Ebony::Box> getPowerupPositions() const { return powerupPositions; }
		Ebony::Box getPaddlePosition() const { return paddlePosition; }
		Ebony::Box getBallPosition() const { return ballPosition; }
	};

	class PythonManager
	{
	public:
		static void Update(std::chrono::microseconds elapsedTime);
		static void ProcessInput();
		static void Reset();

		static void Init(const char* filename, bool isAiStartup = false);
		static void Destroy();


		static Ebony::Discrete action; // -1 is left, 0 is nothing, 1 is right
		static State state;
		static Ebony::Discrete reward;


	private:
		static pybind11::module pyModule;
		static bool initialized;


	};
}