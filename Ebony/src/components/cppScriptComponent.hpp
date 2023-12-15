#pragma once

#include "component.hpp"
#include "../entity.hpp"

namespace components
{
	class CppScriptBase : public PolymorphicComparable<Component, CppScriptBase>
	{
	public:
		CppScriptBase()
		{
		}

		
		void Start() {};
		void Reset() {};
		void Update(std::chrono::microseconds elapsedTime) {};

		void OnDestroy() {};
		void OnCollision() {};
		void OnCollisionEnd() {};
		void OnCollisionStart() {};
		void OnMouseDown() {};
		void OnMouseUp() {};
		void OnMouseEnter() {};
		void OnMouseOver() {};
	};
}