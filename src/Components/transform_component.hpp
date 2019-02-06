#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "utility.hpp"
#include "base_component.hpp"

class TransformComponent : public BaseComponent
{
public:
	TransformComponent(glm::vec2 _position, glm::vec2 _size, GLfloat _rotate);
	~TransformComponent();
	glm::vec2 position;
	glm::vec2 size;
	GLfloat   rotate;
	static const int typeID = ComponentType::transform;
	inline virtual int getTypeID() const { return typeID; };
};

#endif