#ifndef BASE_H
#define BASE_H

#include <vector>
#include "utility.hpp"
#include "entity.hpp"
#include "entity_manager.hpp"
#include "sprite_system.hpp"

class Base : public Entity
{
  public:

    Base(glm::vec2 posn);
    ~Base();

    // posn in window coordinate system (origin is at top left)
    glm::vec2 getPosition() const;

    bool isAlive() const;

  private:

    glm::vec2 position;
    int health;
};
#endif 