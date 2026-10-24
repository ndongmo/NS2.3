/*!
* \file Entity.h
* \brief Entity data structure.
* \author Ndongmo Silatsa Fabrice
* \date 22-05-2017
* \version 2.0
*/

#pragma once

#include "MathUtils.h"
#include "IDraw.h"
#include "Body.h"
#include "Types.h"
#include "MoveChange.h"
#include "IFileManager.h"

#include <glm/glm.hpp>
#include <string>


class ObjectLoader;
class ObjectSaver;
class EntityEditor;
class Level;
class Sensor;
class Agent;
struct ActionDialog;

class Entity : public IDraw, public IFileManager
{
	friend ObjectLoader;
	friend ObjectSaver;
	friend EntityEditor;

public:
	virtual ~Entity() = default;

	/*!
	* \brief Overload of << operator.
	* \param os : output stream
	* \param e : entity
	* \return the output stream with the entity description
	*/
	friend std::ostream& operator<<(std::ostream& os, const Entity& e);
	/*!
	* \brief Check the entity type.
	* \param type : type to check
	* \return true if entity type equal to the param type
	*/
	bool isType(EntityType type);
	/*!
	* \brief Reset an entity.
	*/
	virtual void reset();
	/*!
	* \brief Init an entity.
	*/
	virtual void init() {}
	/*!
	* \brief Reinitialize the dynamic activities of the entity.
	*/
	virtual void reinit(Level& level) {}
	/*!
	* \brief Updates the entiy.
	* \param level : current level
	* \param deltaTime : elapsed time since the last update
	*/
	virtual void update(Level& level, float deltaTime) {}
	/*!
	* \brief Updates AABB surounding shape for the collision structure.
	*/
	virtual void updateAABB(AABB& aabb);
	/*!
	* \brief Checks if the body shape has reached the boundaries of the AABB shape.
	*/
	virtual bool needToUpdate(AABB& aabb);
	/*!
	* \brief Applies the accumulated forces.
	*/
	virtual void applyForces(float deltaTime);
	/*!
	* \brief Update the entity's body according to the given move.
	*/
	void addChange(const MoveChange& mc);
	/*!
	* \brief Check the collision between the current entity and the given one.
	*/
	virtual bool checkCollision(Level& level, Entity* e);
	/*!
	* \brief Prevent the given entity for a tree node collision.
	*/
	virtual void nodeCollision(Level& level, Entity* e) {}
	/*!
	* \brief Handle the collision between the current entity and the given one.
	*/
	virtual void handleCollision(Level& level, Entity* e, const glm::vec2& collisionDepth);
	/*!
	* \brief Force the body to move with the given impulse.
	*/
	virtual void addImpulse(float x, float y);
	/*!
	* \brief Force the body to rotate with the given angular impulse.
	* \param angle :
	*/
	virtual void addAngularImpulse(float angle);
	/*!
	* \brief Handle the sensor event when an entity get in.
	*/
	virtual void handleEnter(Level& level, Sensor* sensor, Entity* e) {}
	/*!
	 * \brief Handle the sensor event when an entity get out.
	 */
	virtual void handleExit(Level& level, Sensor* sensor, Entity* e) {}
	/*!
	 * \brief Make the action regarding the player.
	 */
	virtual bool makeAction(Agent* player, Level& level, ActionDialog& current) { return false; }
	/*!
	 * Make the action regarding the player.
	 */
	virtual bool applyDamage(Level& level, const DamageData& d, const EffectData& e) { return false; }
	/*!
	 * Handle the dead process of the entity.
	 */
	virtual void dead(Level& level);

	virtual void draw(NS2::SpriteBatch& spriteBatch) const override;
	virtual void drawDebug(NS2::DebugRenderer& debugRender, const NS2::ColorRGBA8& color) const override;
	virtual void destroy(Level& level);
	virtual bool isPlayer() { return false; }
	virtual bool canInteract() const { return m_interact; }
	virtual bool checkActive(Level& level) { return m_active; }

	bool isStatic() const { return m_state == EntityState::STATIC; }

	/* SETTERS */
	void setLevelIndex(int index) { m_levelIndex = index; }
	void setNodeIndex(int index) { m_nodeIndex = index; }
	void setEngineIndex(int index) { m_engineIndex = index; }
	void setQuestIndex(int index) { m_questIndex = index; }
	void setName(std::string name) { m_name = name; }
	void setAlive(bool alive) { m_alive = alive; }
	void setType(EntityID type) { m_type = type; }
	void setState(EntityState state) { m_state = state; }
	void setFilter(long filter) { m_filter = filter; }
	void setActive(bool active) { m_active = active; }

	/* GETTERS */
	int getLevelIndex() const { return m_levelIndex; }
	int getNodeIndex() const { return m_nodeIndex; }
	int getEngineIndex() const { return m_engineIndex; }
	int getQuestIndex() const { return m_questIndex; }
	const std::string& getName() const { return m_name; }
	bool isActive() const { return m_active; }
	bool isAlive() const { return m_alive; }
	bool canCollide() const { return m_collide; }
	bool canResurrect() const { return m_resurrect; }
	EntityID getType() const { return m_type; }
	long getFilter() const { return m_filter; }
	EntityState getState() const { return m_state; }

	Body& getBody() { return m_body; }
	const Body& getConstBody() const { return m_body; }
	const Shape2D& getConstShape() const { return m_body.getConstShape(); }

protected:
	bool m_active = true;		/* active or not */
	bool m_alive = true;		/* Entity is alive */
	bool m_collide = true;		/* can collide? */
	bool m_resurrect = false;	/* can resurrect? */
	bool m_interact = false;	/* can interact? */
	int m_levelIndex = -1;		/* Index of the current entity in the level */
	int m_nodeIndex = -1;		/* Index of the current body node in the tree structure */
	int m_engineIndex = -1;		/* Index of the current body node in the physics engine */
	int m_questIndex = -1;		/* Index of the bound quest */
	std::string m_name;			/* Entity name */

	Body m_body;									/* Entity body */
	EntityState m_state = EntityState::STATIC;		/* State of the entity */
	EntityID m_type = EntityID::ID_ENTITY;			/* Entity type */
	long m_filter = EntityID::ID_NONE;				/* Collision mask bits */
};