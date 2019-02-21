#include "world.hpp"

World::World()
{
}

World::~World()
{
}

void World::init(glm::vec2 screen)
{
  // Pre-load fonts to be used
  text.setProjectionSize(static_cast<GLfloat>(screen.x), static_cast<GLfloat>(screen.y));
  text.preprocessGlyphs();

	vector<shared_ptr<Entity>> entities = entityManager.getEntities();
	projection = glm::ortho(0.0f, static_cast<GLfloat>(screen.x), static_cast<GLfloat>(screen.y), 0.0f, -1.0f, 1.0f);
	entityManager = EntityManager();

	enemy.loadEnemy(screen, entityManager);

	ResourceFactory resourceFactory;
	Entity r = resourceFactory.build(vec2(screen.x / 2, 50), vec2(16, 16));
	entityManager.addEntity(r);

	Entity mapDataEntity = MapEntityFactory::createMapEntityFromFile(map_path("map0.txt"));
	entityManager.addEntity(mapDataEntity);
	tileMapSystem.loadTileMap(entityManager); 

	PlayerFactory playerFactory;
	Entity p = playerFactory.build();
	entityManager.addEntity(p);
	physicsSystem.setScreenInfo(screen);
}

void World::update(float dt)
{
  enemy.move();
  vector<shared_ptr<Entity>> entities = entityManager.getEntities();
	playerSystem.interpInput(entityManager, dt, keys, keysProcessed);
  physicsSystem.moveEntities(entityManager, dt);

  // Towers
  towerAttackSystem.checkRangeAndShootAimProjectiles(entityManager);
  towerAttackSystem.reduceElapsedTimeToNextFire(entityManager, dt);

  // OffScreen garbage check
  projectileGarbageSystem.destroyOffScreenEntities(entityManager);
	resourceSystem.handleResourceSpawnAndDespawn(entityManager, dt);
}

void World::processInput(float dt)
{

}

void World::draw()
{
  spriteSystem.drawSprites(entityManager, projection);
  billboardSystem.drawBillboards(entityManager, projection);
  towerRangeDisplaySystem.drawRanges(entityManager, projection);

  text.render("Hg", glm::vec2(20.0f, 60.0f), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}

void World::destroy()
{

}
