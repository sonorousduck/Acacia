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

		virtual ~Entity() 
		{
			children.clear();
		};

		auto getId() const { return m_Id; }

		template <typename T>
		void addComponent(std::unique_ptr<T> component);

		template <typename T>
		void removeComponent();

		template <typename T>
		bool hasComponent();

		template <typename T>
		T* getComponent();

		template <typename T>
		bool tryGetComponent(T*& component);

		auto& getComponents() { return m_Components; }

		//std::shared_ptr<Entity> clone();
		bool operator==(const Entity& rhs);
		bool operator!=(const Entity& rhs);

		bool isEnabled() const
		{
			return enabled;
		}

		void Disable()
		{
			enabled = false;

			for (auto& child : children)
			{
				child->Disable();
			}
		}

		void Enable()
		{
			enabled = true;

			for (auto& child : children)
			{
				child->Enable();
			}
		}

		void SetEnabledStatus(bool isEnabled)
		{
			enabled = isEnabled;

			for (auto& child : children)
			{
				if (isEnabled)
				{
					child->Enable();
				}
				else
				{
					child->Disable();
				}
			}
		}


		void AddChild(std::shared_ptr<Entity> child)
		{
			children.push_back(child);
		}

		void RemoveChild(std::shared_ptr<Entity> child)
		{
			children.erase(std::remove(children.begin(), children.end(), child), children.end());
		}

		std::vector<std::shared_ptr<Entity>> GetChildren() const
		{
			return children;
		}

		std::shared_ptr<Entity> GetChild(int index)
		{
			return children[index];
		}


	private:
		IdType m_Id;
		std::unordered_map<ctti::unnamed_type_id_t, std::unique_ptr<components::Component>> m_Components;
		bool enabled = true;
		std::vector<std::shared_ptr<Entity>> children{};
		//std::weak_ptr<Entity> parent; // TODO: Add scene graph
	};

	// Convenience type aliases for use throughout the framework
	using EntityPtr = std::shared_ptr<Entity>;
	using EntityMap = std::unordered_map<Entity::IdType, EntityPtr>;
	using EntitySet = std::unordered_set<Entity::IdType>;
	using EntityVector = std::vector<EntityPtr>;

	// --------------------------------------------------------------
	//
	// Components are stored by their compile-time unnamed_type_id, because
	// only one of each type can ever exist on an entity. 
	//
	// --------------------------------------------------------------
	template <typename T>
	void Entity::addComponent(std::unique_ptr<T> component)
	{
		component->entity = this;
		m_Components[ctti::unnamed_type_id<T>()] = std::move(component);
	}

	// This finds it by whatever was passed in the diagonal braces
	template <typename T>
	void Entity::removeComponent()
	{
		m_Components.erase(ctti::unnamed_type_id<T>());
	}

	// This finds it by whatever was passed in the diagonal braces
	template <typename T>
	bool Entity::hasComponent()
	{
		return m_Components.contains(ctti::unnamed_type_id<T>());
	}

	// --------------------------------------------------------------
	//
	// This method is returning a raw pointer, because ownership is
	// not an issue.  The calling object can only use/mutate the state
	// of the component, not destroy it.
	//
	// --------------------------------------------------------------
	template <typename T>
	T* Entity::getComponent()
	{
		assert(hasComponent<T>());

		return static_cast<T*>(m_Components[ctti::unnamed_type_id<T>()].get());
	}

	// --------------------------------------------------------------
	//
	// This method is returning a raw pointer, because ownership is
	// not an issue.  The calling object can only use/mutate the state
	// of the component, not destroy it.
	//
	// --------------------------------------------------------------
	template <typename T>
	bool Entity::tryGetComponent(T*& component)
	{
		if (hasComponent<T>())
		{
			component = static_cast<T*>(m_Components[ctti::unnamed_type_id<T>()].get());
			return true;
		}
		else
		{
			return false;
		}
	}

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