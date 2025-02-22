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
		Link(size_t childNode, std::function<bool()> shouldTraverse) : childNode(childNode), shouldTraverse(shouldTraverse) {}

		size_t childNode; // The node that the link points to
		std::function<bool()> shouldTraverse; // The function that has to evaluate as true in order to move to that node
	};

	struct Node
	{
		Node(std::vector<Link> links, std::vector<Ebony::Animation> animations) : links(links), animations(animations) {}

		std::vector<Link> links{};
		std::vector<Ebony::Animation> animations{};
	};

	class AnimationController : public PolymorphicComparable<Component, AnimationController>
	{
	public:
		AnimationController(Ebony::RenderLayer renderLayer) : animationTree(std::vector<Node>()), isUI(false), renderLayer(renderLayer) { };
		AnimationController(std::vector<Node> animationTree, Ebony::RenderLayer renderLayer, bool isUI = false) : animationTree(animationTree), isUI(isUI), renderLayer(renderLayer) {};

		std::vector<Ebony::Animation> GetSprite()
		{
			return animationTree.at(currentNode).animations;
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
			if (animationTree.size() == rhs.animationTree.size())
			{
				for (unsigned int i = 0; i < animationTree.size(); i++)
				{
					if (animationTree[i].animations.size() != rhs.animationTree[i].animations.size())
					{
						return false;
					}
				}

				return true;
			}
			return false;
		}


		// Vector of nodes containing animations
		//std::vector<std::vector<Ebony::Animation>> animations;
		std::vector<Node> animationTree;
		size_t currentNode{ 0 };
		bool isUI;
		Ebony::RenderLayer renderLayer;

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

