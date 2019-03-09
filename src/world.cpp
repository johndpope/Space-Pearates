#include "world.hpp"

// Entities that we do not want to erase on resetting
const vector<int> non_recyclable_components = {
  ComponentType::tile, ComponentType::map, ComponentType::home, ComponentType::background_sprite, ComponentType::player };

void World::init(vec2 screen)
{
  projection = glm::ortho(0.0f, static_cast<GLfloat>(screen.x), static_cast<GLfloat>(screen.y), 0.0f, -1.0f, 1.0f);
  physicsSystem.setScreenInfo(screen);
  collisionSystem.setScreenInfo(screen);
  HUD::getInstance();
  entityManager = EntityManager();
  particleSystem.initParticleSystem(entityManager);

  Entity mapData = MapEntityFactory::createMapEntityFromFile(map_path("map0.txt"));
  entityManager.addEntity(mapData);
  TileMapSystem::loadTileMap(entityManager, player_spawn);
  entityManager.addEntity(PlayerFactory::build(player_spawn));

  ResourceFactory::spawnMany(entityManager); // TODO: maybe remove

  entityManager.addEntity(EnemySpawnFactory::build(2.0));
  enemySystem.setMap(entityManager);

  // create background entity
  Entity backgroundEntity = BackgroundEntityFactory::createBackgroundEntity();
  entityManager.addEntity(backgroundEntity);

  // Generate the build tower ui entity
  Entity towerUiEntity = TowerUiEntityFactory::create();
  entityManager.addEntity(towerUiEntity);

  Entity w = WavesetManagerFactory::build(waveset_path("waveset0.txt"));
  entityManager.addEntity(w);

  renderToTextureSystem.initWaterEffect();
}

void World::update(float dt)
{
  HUD::getInstance().update(dt);

  // Note: Be careful, order may matter in some cases for systems
  wavesetSystem.handleBuildAndDefensePhase(entityManager, dt);

  enemySystem.move(dt, entityManager, wavesetSystem);
  physicsSystem.moveEntities(entityManager, dt);
  physicsSystem.rotateEntities(entityManager, dt);
  interpolationSystem.update(entityManager, dt);
  collisionSystem.checkCollisions(entityManager, wavesetSystem);
  spriteSystem.updateElapsedTime(dt);

  // Background Update
  backgroundSystem.update(entityManager);

  // Build Tower UI
  towerUiSystem.update(entityManager, dt);

  // Towers
  towerAttackSystem.checkRangeAndShootProjectiles(entityManager);
  towerAttackSystem.reduceElapsedTimeToNextFire(entityManager, dt);

  // OffScreen garbage check
  offscreenGarbageSystem.destroyEntitiesContainingAll(entityManager, vector<int>{ComponentType::projectile, ComponentType::movement});
  resourceSystem.handleResourceSpawnAndDespawn(entityManager, dt);
  deathSystem.handleDeaths(entityManager);

  particleSystem.updateParticles(entityManager, dt);
}

void World::processInput(float dt, GLboolean keys[], GLboolean keysProcessed[])
{
  // Reset
  if (keys[GLFW_KEY_R] && !keysProcessed[GLFW_KEY_R] && false)
  {
    // Remove all recyclable entities
    entityManager.filterRemoveByComponentType(non_recyclable_components);

    // Reset HUD and music
    HUD::getInstance().reset();
    AudioLoader::getInstance().reset();
    // Reset player position
    shared_ptr<Entity> player = entityManager.getEntitiesHasOneOf(entityManager.getComponentChecker(ComponentType::player))[0];
    player->getComponent<TransformComponent>()->position = player_spawn;
    // Spawn starting resources
    ResourceFactory::spawnMany(entityManager); // TODO: maybe remove

    entityManager.addEntity(EnemySpawnFactory::build(2.0));
    enemySystem.setMap(entityManager);

    Entity towerUiEntity = TowerUiEntityFactory::create();
    entityManager.addEntity(towerUiEntity);

    entityManager.addEntity(WavesetManagerFactory::build(waveset_path("waveset0.txt")));

    keysProcessed[GLFW_KEY_R] = true;
  }
  // TODO: music should alternate
  if (keys[GLFW_KEY_H] && !keysProcessed[GLFW_KEY_H])
  {
    AudioLoader::getInstance().changeBgm();
    keysProcessed[GLFW_KEY_H] = true;
  }

  playerSystem.interpInput(entityManager, dt, keys, keysProcessed);
  towerUiSystem.interpInput(entityManager, keys, keysProcessed);
}

void World::draw()
{
  // get ready to render to texture
  renderToTextureSystem.beforeRenderWaterEffect();
  // Background will render first
  backgroundSystem.render(entityManager, projection);
  spriteSystem.drawSprites(entityManager, projection);
  // so far everything rendered into buffer
  // add effects to the buffer and render this buffer to screen
  renderToTextureSystem.drawWaterEffect(entityManager, projection);

  billboardSystem.drawBillboards(entityManager, projection);
  towerRangeDisplaySystem.drawRanges(entityManager, projection);
  towerUiSystem.render(entityManager, projection);
  HUD::getInstance().draw();
}

void World::destroy()
{
  entityManager.destroyAll();
}
