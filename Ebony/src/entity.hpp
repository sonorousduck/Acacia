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

Thanks for Dean Mathias for base implementation of Entity base class
*/

#pragma once

#include "components/component.hpp"

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
#include <atomic>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>


namespace entities
{
   // --------------------------------------------------------------
   //
   // An Entity is a collection of components.
   //
   // The "name" for an entity is its "ctti::unnamed_type_id_t".  This
   // allows for compile-time determination of the name, using an integral-like
   // type, which also allows it to have fast lookup/use in various
   // associative containers.
   //
   // --------------------------------------------------------------

	class Entity : public std::enable_shared_from_this<Entity>
	{
	private:
		static std::atomic_uint32_t nextId; // Each entity needs a unique id
	public:
		using IdType = decltype(nextId.load());

		Entity() : m_Id(Entity::nextId.load())
		{
			Entity::nextId++;
		}

		virtual ~Entity() {};

		auto getId() { return m_Id; }

		template <typename T>
		void addComponent(std::unique_ptr<T> component);

		template <typename T>
		void removeComponent();

		template <typename T>
		bool hasComponent();

		template <typename T>
		T* getComponent();

		auto& getComponents() { return m_Components; }

		std::shared_ptr<Entity> clone();
		bool operator==(const Entity& rhs);
		bool operator!=(const Entity& rhs);


	private:
		IdType m_Id;
		std::unordered_map<ctti::unnamed_type_id_t, std::unique_ptr<components::Component>> m_Components;



	};

	// Convenience type aliases for use throughout the framework
	using EntityPtr = std::shared_ptr<Entity>;
	using EntityMap = std::unordered_map<Entity::IdType, EntityPtr>;
	using EntitySet = std::unordered_set<Entity::IdType>;
	using EntityVector = std::vector<EntityPtr>;

}

namespace std
{

	// --------------------------------------------------------------
	//
	// Hash function for entities::Entity, to allow it to be used in
	// std:: containers where a hash function is needed.
	//
	// --------------------------------------------------------------
	template <>
	struct hash<shared_ptr<entities::Entity>>
	{
		size_t operator()(const entities::EntityPtr& v) const noexcept
		{
			return v->getId();
		}
	};
} // namespace std