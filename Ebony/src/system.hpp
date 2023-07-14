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




#pragma once

// Disable some compiler warnings that come from ctti
#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable : 4245)
	#endif
#if defined(__clang__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#endif
	#include <ctti/type_id.hpp>
#if defined(__clang__)
	#pragma GCC diagnostic pop
#endif
#if defined(_MSC_VER)
	#pragma warning(pop)
#endif

#include <chrono>
#include <initializer_list>
#include <unordered_set>

#include "entity.hpp"

namespace systems
{
	class System
	{
	public:
		System()
		{
		}

		System(const std::initializer_list<ctti::unnamed_type_id_t>& list) : m_Interests(list)
		{}
		virtual ~System() {};

		virtual void clear() { m_Entities.clear(); }
		virtual bool addEntity(entities::EntityPtr entity);
		virtual void removeEntity(entities::Entity::IdType entityId);
		virtual void updateEntity(entities::EntityPtr entity);
		virtual void update([[maybe_unused]] std::chrono::microseconds elapsedTime) {}
		virtual void shutdown() {}

	protected:
		entities::EntityMap m_Entities;

		virtual bool isInterested(const entities::EntityPtr& entity);

	private:
		std::unordered_set<ctti::unnamed_type_id_t> m_Interests;

	};



}