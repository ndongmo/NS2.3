#include "PhysicsEngine.h"
#include "Level.h"

#include <SDL3/SDL.h>

const float MAX_DELTA_TIME = 1.0f;

void PhysicsEngine::init(int maxPhysicsSteps)
{
	m_maxPhysicsSteps = maxPhysicsSteps;
}

void PhysicsEngine::dispose()
{
	m_tree.clean();
	m_dynamicBodies.clear();
	m_staticBodies.clear();
	m_contacts.clear();
}

void PhysicsEngine::addCollisionBody(Entity* e)
{
	AABB aabb;
	e->updateAABB(aabb);
	int nodeIndex = m_tree.createNode(e, aabb);
	e->setNodeIndex(nodeIndex);
	
	if (e->getState() == EntityState::STATIC) {
		m_staticBodies.push_back(e);
		e->setEngineIndex(static_cast<int>(m_staticBodies.size()) - 1);
	}
	else {
		m_dynamicBodies.push_back(e);
		e->setEngineIndex(static_cast<int>(m_dynamicBodies.size()) - 1);
	}
}

void PhysicsEngine::removeCollisionBody(Entity* e)
{
	m_tree.freeLeaf(e->getNodeIndex());
	int i = e->getEngineIndex();

	if (e->getState() == EntityState::STATIC) {
		m_staticBodies[i] = m_staticBodies.back();
		m_staticBodies[i]->setEngineIndex(i);
		m_staticBodies.pop_back();
	}
	else {
		m_dynamicBodies[i] = m_dynamicBodies.back();
		m_dynamicBodies[i]->setEngineIndex(i);
		m_dynamicBodies.pop_back();
	}
}

void PhysicsEngine::broadPhase(Level& level)
{
	if (m_dynamicBodies.size() + m_staticBodies.size() < 2)
		return;

	for (Entity* b : m_dynamicBodies) {
		broadPhase(level, b);
	}
}

void PhysicsEngine::broadPhase(Level& level, Entity* e)
{
	int index = e->getNodeIndex();
	DTNode<Entity>& node = m_tree.m_nodes[index];
	int neighborIndex = m_tree.m_nodes[node.parent].right;

	if (neighborIndex == index)
	{
		neighborIndex = m_tree.m_nodes[node.parent].left;
	}

	queryCollision(level, index, neighborIndex);
	
	int grandParent = m_tree.m_nodes[node.parent].parent;

	while (grandParent != EmptyNode)
	{
		queryCollision(level, m_tree.m_nodes[grandParent].left, m_tree.m_nodes[grandParent].right);
		grandParent = m_tree.m_nodes[grandParent].parent;
	}
}

bool PhysicsEngine::queryCollision(Level& level, int node1, int node2)
{
	ContactPair cp(node1, node2);
	DTNode<Entity>& dt1 = m_tree.m_nodes[node1];
	DTNode<Entity>& dt2 = m_tree.m_nodes[node2];

	// if the nodes don't overlap or this pair is already handled
	if (!dt1.aabb.overlap(dt2.aabb) ||
		std::find(m_contacts.begin(), m_contacts.end(), cp) != m_contacts.end())
		return false;

	m_contacts.emplace_back(node1, node2);

	if (dt1.isLeaf() && dt2.isLeaf())
	{
		if (dt1.userData->getState() == EntityState::DYNAMIC) {
			return updateCollision(level, dt1.userData, dt2.userData);
		}
		else {
			return updateCollision(level, dt2.userData, dt1.userData);
		}
	}
	else
	{
		if (!dt2.isLeaf())
		{
			queryCollision(level, node1, dt2.left);
			queryCollision(level, node1, dt2.right);
		}
		if (!dt1.isLeaf())
		{
			queryCollision(level, node2, dt1.left);
			queryCollision(level, node2, dt1.right);
		}
	}

	return false;
}

bool PhysicsEngine::updateCollision(Level& level, Entity* e1, Entity* e2)
{
	if (e1->getState() == EntityState::STATIC || !e1->canCollide() || !e2->canCollide()) {
		e1->nodeCollision(level, e2);
		e2->nodeCollision(level, e1);
	}
	else if (!(e1->getFilter() & e2->getType())) {
		e1->checkCollision(level, e2);
	}
	else if (!(e2->getFilter() & e1->getType())) {
		e2->checkCollision(level, e1);
	}
	else {
		e1->nodeCollision(level, e2);
		e2->nodeCollision(level, e1);
	}
	return true;
}

void PhysicsEngine::step(Level& level, float deltaTime)
{
	for (Entity* b : m_dynamicBodies)
	{
		if (!b->isActive()) continue;

		if (b->needToUpdate(m_tree.m_nodes[b->getNodeIndex()].aabb))
		{
			b->updateAABB(m_tree.m_nodes[b->getNodeIndex()].aabb);
			m_tree.updateNode(b->getNodeIndex());
		}
	}

	for (Entity* b : m_staticBodies)
	{
		if (b->checkActive(level)) {
			b->update(level, deltaTime);
		}
	}

	int i = 0;
	while (deltaTime > 0.0f && i < m_maxPhysicsSteps)
	{
		float stepDeltaTime = (deltaTime > MAX_DELTA_TIME) ? MAX_DELTA_TIME : deltaTime;

		for (Entity* b : m_dynamicBodies)
		{
			if (!b->checkActive(level)) continue;

			b->applyForces(stepDeltaTime);
			b->update(level, stepDeltaTime);
		}

		broadPhase(level);
		m_contacts.clear();

		deltaTime -= stepDeltaTime;
		i++;
	}
}

void PhysicsEngine::step(Entity* e)
{
	if (e->getNodeIndex() >= 0 && e->getLevelIndex() >= 0)
	{
		e->updateAABB(m_tree.m_nodes[e->getNodeIndex()].aabb);
		m_tree.updateNode(e->getNodeIndex());
	}
}


void PhysicsEngine::query(ICollisionCallback* callback, const AABB& aabb) const
{
	// we use the stack and an iterative method here insted of a recursive one 
	// in order to manage the fact that the callback can decide to stop the search 
	std::stack<int> stack;
	stack.push(m_tree.m_root);

	while (stack.size() > 0)
	{
		int node = stack.top();
		stack.pop();
		if (node == EmptyNode)
			continue;

		if (MathUtils::isOverlapping(m_tree.m_nodes[node].aabb, aabb))
		{
			if (m_tree.m_nodes[node].isLeaf())
			{
				if (!callback->queryCallback(m_tree.m_nodes[node].userData))
					return;
			}
			else
			{
				stack.push(m_tree.m_nodes[node].left);
				stack.push(m_tree.m_nodes[node].right);
			}
		}
	}
}

void PhysicsEngine::rayCast(ICollisionCallback* callback, const glm::vec2& p1, const glm::vec2& p2) const
{
	// we use the stack and an iterative method here insted of a recursive one 
	// in order to manage the fact that the callback can decide to stop the search 
	std::stack<int> stack;
	stack.push(m_tree.m_root);

	while (stack.size() > 0)
	{
		int node = stack.top();
		stack.pop();
		if (node == EmptyNode)
			continue;

		if (MathUtils::isPointInsideAABB(p1, m_tree.m_nodes[node].aabb) || 
			MathUtils::isPointInsideAABB(p2, m_tree.m_nodes[node].aabb) || 
			MathUtils::isLineAABBCollide(p1, p2, m_tree.m_nodes[node].aabb))
		{
			if (m_tree.m_nodes[node].isLeaf())
			{
				if (!callback->queryCallback(m_tree.m_nodes[node].userData)) {
					return;
				}
			}
			else
			{
				stack.push(m_tree.m_nodes[node].left);
				stack.push(m_tree.m_nodes[node].right);
			}
		}
	}
}

Entity* PhysicsEngine::find(const glm::vec2& p) const
{
	std::stack<int> stack;
	stack.push(m_tree.m_root);

	while (stack.size() > 0)
	{
		int node = stack.top();
		stack.pop();
		if (node == EmptyNode)
			continue;

		if (MathUtils::isPointInsideAABB(p, m_tree.m_nodes[node].aabb))
		{
			if (m_tree.m_nodes[node].isLeaf())
			{
				if (MathUtils::isPointInsideBox(p, m_tree.m_nodes[node].userData->getBody().getShape()))
					return m_tree.m_nodes[node].userData;
			}
			else
			{
				stack.push(m_tree.m_nodes[node].left);
				stack.push(m_tree.m_nodes[node].right);
			}
		}
	}
	return nullptr;
}

Entity* PhysicsEngine::findByDepth(const glm::vec2& p) const
{
	Entity* result = nullptr;
	std::stack<int> stack;
	stack.push(m_tree.m_root);

	while (stack.size() > 0)
	{
		int node = stack.top();
		stack.pop();
		if (node == EmptyNode)
			continue;

		if (MathUtils::isPointInsideAABB(p, m_tree.m_nodes[node].aabb))
		{
			if (m_tree.m_nodes[node].isLeaf())
			{
				if (MathUtils::isPointInsideBox(p, m_tree.m_nodes[node].userData->getBody().getShape()) &&
					(result == nullptr || result->getBody().getDepth() < m_tree.m_nodes[node].userData->getBody().getDepth()))
					result = m_tree.m_nodes[node].userData;
			}
			else
			{
				stack.push(m_tree.m_nodes[node].left);
				stack.push(m_tree.m_nodes[node].right);
			}
		}
	}
	return result;
}

Entity* PhysicsEngine::findNearBy(Entity* e) const
{
	std::stack<int> stack;
	stack.push(m_tree.m_root);

	while (stack.size() > 0)
	{
		int node = stack.top();
		stack.pop();
		if (node == EmptyNode)
			continue;

		if (MathUtils::isOverlapping(m_tree.m_nodes[e->getNodeIndex()].aabb, m_tree.m_nodes[node].aabb))
		{
			if (e->getNodeIndex() != node && m_tree.m_nodes[node].isLeaf())
			{
				if (e->getType() == m_tree.m_nodes[node].userData->getType())
					return m_tree.m_nodes[node].userData;
			}
			else
			{
				stack.push(m_tree.m_nodes[node].left);
				stack.push(m_tree.m_nodes[node].right);
			}
		}
	}
	return nullptr;
}

void PhysicsEngine::draw(NS2::SpriteBatch& spriteBatch, const glm::vec4& view) const
{
	std::stack<int> stack;
	stack.push(m_tree.m_root);

	while (stack.size() > 0)
	{
		int node = stack.top();
		stack.pop();
		if (node == EmptyNode)
			continue;

		if (MathUtils::isBoxInView(m_tree.m_nodes[node].aabb, view))
		{
			if (m_tree.m_nodes[node].isLeaf())
			{
				m_tree.m_nodes[node].userData->draw(spriteBatch);
			}
			else
			{
				stack.push(m_tree.m_nodes[node].left);
				stack.push(m_tree.m_nodes[node].right);
			}
		}
	}
}

void PhysicsEngine::drawDebug(NS2::DebugRenderer& debugRenderer, const NS2::ColorRGBA8& entityColor, 
	const NS2::ColorRGBA8& treeColor, const glm::vec4& view, bool debugEntities, bool debugNodes, EntityID type) const
{
	std::stack<int> stack;
	stack.push(m_tree.m_root);

	while (stack.size() > 0)
	{
		int node = stack.top();
		stack.pop();
		if (node == EmptyNode)
			continue;

		if (MathUtils::isBoxInView(m_tree.m_nodes[node].aabb, view))
		{
			if (debugNodes) {
				static glm::vec4 destRect;
				m_tree.m_nodes[node].aabb.getRect(destRect);
				debugRenderer.drawBox(destRect, treeColor);
			}

			if (m_tree.m_nodes[node].isLeaf() && m_tree.m_nodes[node].userData->getType() & type)
			{
				if (debugEntities) {
					m_tree.m_nodes[node].userData->drawDebug(debugRenderer, entityColor);
				}
			}
			else
			{
				stack.push(m_tree.m_nodes[node].left);
				stack.push(m_tree.m_nodes[node].right);
			}
		}
	}
}

void PhysicsEngine::updateEntities(std::vector<Entity*> entities)
{
	for (Entity* e : entities) {
		if (e->getLevelIndex() >= 0) {
			if (e->getState() != EntityState::STATIC) {
				m_dynamicBodies[e->getEngineIndex()] = e;
				if (e->getNodeIndex() >= 0) {
					m_tree.m_nodes[e->getNodeIndex()].userData = e;
				}
			}
			else {
				m_staticBodies[e->getEngineIndex()] = e;
				if (e->getNodeIndex() >= 0) {
					m_tree.m_nodes[e->getNodeIndex()].userData = e;
				}
			}
		}
	}
}
