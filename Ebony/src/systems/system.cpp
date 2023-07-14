/*
Copyright (c) 2022 James Dean Mathias

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.


Thanks to Dean Mathias for usage for his code for the base system
*/



#include "system.hpp"
#include <algorithm>

namespace systems
{

	bool System::AddEntity(entities::EntityPtr entity)
	{
		if (isInterested(entity))
		{
			m_Entities[entity->getId()] = entity;
			return true;
		}
		return false;
	}

	void System::RemoveEntity(entities::Entity::IdType entityId)
	{
		m_Entities.erase(entityId);
	}

	void System::UpdateEntity(entities::EntityPtr entity)
	{
		if (m_Entities.contains(entity->getId()))
		{
			if (!isInterested(entity))
			{
				m_Entities.erase(entity->getId());
			}
		}
		else if (isInterested(entity))
		{
			m_Entities[entity->getId()] = entity;
		}
	}

	bool System::isInterested(const entities::EntityPtr& entity)
	{
		bool doICare = std::all_of(
			m_Interests.begin(), m_Interests.end(),
			[&entity](auto interest)
			{
				return entity->getComponents().contains(interest);
			});

		return doICare;
	}
}