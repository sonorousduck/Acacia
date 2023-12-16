#pragma once

#include "component.hpp"
#include "../entity.hpp"
#include <iostream>

namespace components
{
	class CppScript : public PolymorphicComparable<Component, CppScript>
	{
	public:
		CppScript()
		{
		}

		
		virtual void Start() {};
		virtual void Reset() {};
		virtual void Update(std::chrono::microseconds elapsedTime) { };

		virtual void OnDestroy() {};
		virtual void OnCollision(entities::EntityPtr, std::chrono::microseconds elapsedTime) {};
		virtual void OnCollisionEnd(entities::EntityPtr, std::chrono::microseconds elapsedTime) {};
		virtual void OnCollisionStart(entities::EntityPtr, std::chrono::microseconds elapsedTime) {};
		virtual void OnMouseDown() {};
		virtual void OnMouseUp() {};
		virtual void OnMouseEnter() {};
		virtual void OnMouseOver() {};
	};
}