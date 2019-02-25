#ifndef ENTITY_H
#define ENTITY_H

#include "utility.hpp"
#include "Components/base_component.hpp"

class Entity {
public:
  Entity();
	int id;

	// Template functions MUST be defined in the header as the compiler must see it before generating code for any usages.
	template <typename T>
	T* getComponent()
	{
		T *component = static_cast<T*>(components[T::typeID]);
		return (component == nullptr) ? nullptr : component;
	}

	template <typename T>
	void setComponent(T* component)
	{
		components[T::typeID] = component;
		has_components.set(T::typeID, 1);
	}

	template <typename T>
	void removeComponent()
	{
		// TO-DO: Move to c++11 unique_ptrs as this can cause seg fault
		// if something is holding onto this pointer still...
		delete components[T::typeID];
		components[T::typeID] = nullptr;
		has_components.set(T::typeID, 0);	
	}

  bool hasComponents(std::bitset<ComponentType::max_count> component_checker);

  // check if an entity has any one of required components in component_checker
  bool hasOneOfComponents(std::bitset<ComponentType::max_count> component_checker);

	static int next() { return next_id++; };
	static int next_id;
  
  bool operator==(const Entity &other) const {
    return id == other.id;
  };

	void print();
private:
	std::vector<BaseComponent*> components;
	std::bitset<ComponentType::max_count> has_components;
};

#endif
