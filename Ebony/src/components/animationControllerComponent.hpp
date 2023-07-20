#pragma once

#include "component.hpp"
#include <chrono>
#include <cstdint>
#include <memory>
#include <vector>
#include "../animation.hpp"


namespace components
{
	struct Link
	{
		size_t childNode;
		std::function<bool()> shouldTraverse;
	};

	struct Node
	{
		std::vector<Link> links;
	};

	class AnimationController : public PolymorphicComparable<Component, AnimationController>
	{
	public:
		AnimationController() : animations(std::vector<std::vector<Ebony::Animation>>()), animationTree(std::vector<Node>()) { };
		AnimationController(std::vector<Node> animationTree, std::vector<std::vector<Ebony::Animation>> animations) : animationTree(animationTree), animations(animations) {};

		std::vector<Ebony::Animation> GetSprite()
		{
			return animations.at(currentNode);
		}

		// Add animation will both add a node and an animation
		// Create a file format for loading/saving animation trees
		// Potentially just dump the animation controller to binary
		//void AddAnimation(std::vector<Ebony::Animation> animation) 
		//{ 
		//	animations.emplace_back(animation); 
		//}

		//void AddNode(Node node)
		//{
		//	animationTree.emplace_back(node);
		//}

		//void AddAnimationAndNode(Ebony::Animation animation, Node node)
		//{
		//	animations.emplace_back(animation);
		//	animationTree.emplace_back(node);
		//}


		bool operator==(AnimationController& rhs)
		{
			if (animations.size() == rhs.animations.size())
			{
				for (unsigned int i = 0; i < animations.size(); i++)
				{
					if (animations[i].size() != rhs.animations[i].size())
					{
						return false;
					}
				}

				return true;
			}
			return false;
		}


		// Vector of nodes containing animations
		std::vector<std::vector<Ebony::Animation>> animations;
		std::vector<Node> animationTree;
		size_t currentNode{ 0 };

		// Needs a graph structure here
		// Animations are the nodes
		// The graph will describe the connections between the animation
		// i.e. when walking ends, should move to idle
		// This lets you keep track what your current node is, which allows you to check only those connections directly from that node
		// If walking, then it can either run or become idle, those are only options
		// This will handle checking if it has fulfilled the conditions
		// This check will be handled on the system
		

		// std::vector<std::vector<std::pair<int, std::function<bool>>>>
		// First vector tells outgoing links for the current node (The node itself) Node = std::vector<std::pair<int, std::function<bool>>> (A node's outgoing links)
		// Inner vector is the all of the places it can go (if node 0 connects to 1, 2, and 3)
		// For each outgoing link, call the function. If it is true, the new current node is the integer of the std::pair (This sets the current node to the node it should be at)

		// Node 0 = Animation 0

	};
}

