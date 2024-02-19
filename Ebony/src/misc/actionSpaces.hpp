#pragma once

#include <glm/glm.hpp>
#include <pybind11/stl.h>
#include <string>

namespace Ebony
{
	class Box
	{
	public:
		Box() : box{0.0f, 0.0f, 0.0f, 0.0f} {}

		Box(float x, float y, float z, float w) : box({x, y, z, w})
		{
		}

		void setBox(float x, float y, float z, float w)
		{
			box = std::vector<float>{ x, y, z, w };
		}

		std::vector<float> getBox() const
		{
			return box;
		}

		std::vector<float> box{};
	};

	class Action
	{
	public:
		Action() : action{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } {}

		Action(float x, float y, float z, float w, float a) : action({ x, y, z, w, a })
		{
		}

		void setAction(float x, float y, float z, float w, float a)
		{
			action = std::vector<float>{ x, y, z, w, a };
		}

		std::vector<float> getAction() const
		{
			return action;
		}

		std::vector<float> action{};
	};

	class Discrete
	{
	public:
		Discrete() {}
		Discrete(int n) : n(n)
		{}

		int getValue() const
		{
			return n;
		}

		void setValue(int newN)
		{
			n = newN;
		}

		int n{};
	};

	class MultiBinary
	{
	public:
		MultiBinary() {};
		MultiBinary(std::vector<bool> values) : n(values)
		{}

		std::vector<bool> n{};
	};

	class MultiDiscrete
	{
	public:
		MultiDiscrete() {}
		MultiDiscrete(std::vector<int> values) : n(values) {}

		std::vector<int> n{};
	};

	class ActionVector
	{
	public:
		ActionVector(std::vector<std::string> actions) : actions(actions)
		{}

		std::vector<std::string> actions;
	};
}