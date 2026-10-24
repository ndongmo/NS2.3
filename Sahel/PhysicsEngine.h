/*!
* \file PhysicsEngine.h
* \brief Manages all physics events of the game.
* \author Ndongmo Silatsa Fabrice
* \date 21-05-2017
* \version 2.0
*/

#pragma once

#include <vector>
#include <DebugRenderer.h>
#include <SpriteBatch.h>

#include "CollisionManager.h"
#include "DynamicTree.h"
#include "Entity.h"

class Level;

class PhysicsEngine
{
public:
	/*!
	* \brief Initializes the physics engine.
	* \param maxPhysicsSteps :
	* \param maxFixture : number max of fixture the engine can manage,
	*					so be carreful
	*/
	void init(int maxPhysicsSteps);
	/*!
	* \brief Disposes the physics engine.
	*/
	void dispose();
	/*!
	* \brief Physics step.
	*/
	void step(Level& level, float deltaTime);
	/*!
	 * \brief update the given entity in the tree.
	 */
	void step(Entity* e);
	/*!
	* \brief Adds the given entity in the tree and amonst the managed bodies.
	* \param e :
	*/
	void addCollisionBody(Entity* e);
	/*!
	* \brief removes the given entity in the tree and amonst the managed bodies.
	* \param e :
	*/
	void removeCollisionBody(Entity* e);
	/*!
	* \brief Checks for collision between pairs of nodes.
	*/
	void broadPhase(Level& level);
	/*!
	* \brief Checks for collision between pairs of nodes for the given entity.
	*/
	void broadPhase(Level& level, Entity* e);
	/*!
	* \brief Queries the tree with the given AABB. For each overlapping,
	*  it calls the given callback as handler.
	* \param callback : can decide to stop the execution after some calls
	* \param aabb :
	*/
	void query(ICollisionCallback* callback, const AABB& aabb) const;
	/*!
	* \brief Queries the tree with the given segment (ray). For each overlapping,
	*  it calls the given callback as handler.
	* \param callback : can decide to stop the execution after some calls
	* \param p1 :
	*/
	void rayCast(ICollisionCallback* callback, const glm::vec2& p1, const glm::vec2& p2) const;
	/*!
	* \brief Find the Entity which collide with the given point.
	* \param p :
	* \return null if nothing has been found
	*/
	Entity* find(const glm::vec2& p) const;
	/*!
	* \brief Find the Entity which collide with the given point.
	* The priority is given to the higher depth.
	* \param p :
	* \return null if nothing has been found
	*/
	Entity* findByDepth(const glm::vec2& p) const;
	/*!
	* \brief Find the Entity which collide with the given entity with the same type.
	* \param e : entity to find nearby
	* \return null if nothing has been found
	*/
	Entity* findNearBy(Entity* e) const;

	/*!
	* \brief Queries the tree with the given AABB and draws each overlapping entity.
	* \param spriteBatch : batch
	* \param view : view area
	*/
	void draw(NS2::SpriteBatch& spriteBatch, const glm::vec4& view) const;
	/*!
	* \brief Queries the tree with the given AABB and draws each overlapping entity.
	* \param debugRenderer : batch
	* \param entityColor : entity debug color
	* \param treeColor : tree debug color
	* \param view : view area
	* \param debugEntities : draw entity box
	* \param debugNodes : draw the tree nodes
	*/
	void drawDebug(NS2::DebugRenderer& debugRenderer, const NS2::ColorRGBA8& entityColor, const NS2::ColorRGBA8& treeColor,
		const glm::vec4& view, bool debugEntities = true, bool debugNodes = false, EntityID type = EntityID::ID_ENTITY) const;
	/*!
	 * \brief Update the entities pointers when the capacity has changed.
	 */
	void updateEntities(std::vector<Entity*> entities);

private:
	/*!
	* \brief Finds collision recursively between the given two nodes.
	* \param node1 : always a dynamic body
	* \param node2 : dynamic/static body
	*/
	bool queryCollision(Level& level, int node1, int node2);
	/*!
	* \brief Check and update collision between the two given entities.
	* \param e1 : 
	* \param e2 : 
	*/
	bool updateCollision(Level& level, Entity* e1, Entity* e2);

	int m_maxPhysicsSteps;						/* Constant number of physics steps */
	std::vector<Entity*> m_dynamicBodies;		/* Set of dynamic entities */
	std::vector<Entity*> m_staticBodies;		/* Set of static entities */
	std::vector<ContactPair> m_contacts;		/* List of current contact to solve */
	DynamicTree<Entity> m_tree;					/* Tree structure for collision detection */
};