#ifndef TOWER_UI_IDLE_STATE_H
#define TOWER_UI_IDLE_STATE_H

#include "../../../Utility/utility.hpp"
#include "../../../hud.hpp"
#include "../../../entity_manager.hpp"
#include "../../../Utility/audio_loader.hpp"
#include "../../map_component.hpp"
#include "../../wallet_component.hpp"
#include "../../../EntityFactories/tower_entity_factory.hpp"
#include "tower_ui_state_component.hpp"
#include "tower_ui_shift_left_state.hpp"
#include "tower_ui_shift_right_state.hpp"
#include "DataLoaders/tower_data_loader.hpp"
#include "DataLoaders/tower_structs.hpp"

class TowerUiIdleState : public TowerUiStateComponent {
public:
  virtual void input(EntityManager& entities, GLboolean keys[], GLboolean keysProcessed[]);
  virtual void update(EntityManager& entities, float dt);

  static const int typeID = ComponentType::tower_ui_state; // use same type id as parent
  inline virtual int getTypeID() const { return typeID; };
private:
  void processOperate(glm::vec2 playerCenterPosition, TOWER_UI_OPT_TYPE operationType, EntityManager& entityManager);
};

#endif // !TOWER_UI_IDLE_STATE
