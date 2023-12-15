#include "cppScriptingSystem.hpp"
#include <iostream>
#include "../../../BrickBreaker/scripts/DeathScript.hpp"


namespace systems
{
	void CppScriptingSystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	void CppScriptingSystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		for (auto& [id, entity] : m_Entities)
		{
			auto test = entity->getComponentFromInheritedComponent<components::CppScriptBase>();
			auto test1 = test->GetType();
			auto test2 = static_cast<test1>(test);


			test->Update(elapsedTime);
		}

	}

	bool CppScriptingSystem::isInterested(const entities::EntityPtr& entity)
	{
		auto& components = entity->getComponents();
		for (auto& [typeInfo, component] : components)
		{
			if (component.get()->GetParentType() == ctti::unnamed_type_id<components::CppScriptBase>())
			{
				return true;
			}
		}


		//bool doICare = components.contains(ctti::unnamed_type_id<components::CppScriptBase>());

		return false;

	}
}

