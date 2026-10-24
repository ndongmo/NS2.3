/*!
* \file DynamicTree.h
* \brief Dynamic tree structure for collision.
*	Inspired from the dynamic tree implementation of box2d and
*   Randy Gaul's Game programming blog.
*	For proper explanation see : Introduction to Game Physics with Box2D By Ian Parberry
* \author Ndongmo Silatsa Fabrice
* \date 13-04-2017
* \version 1.0
*/

#pragma once

#include <stack>

#include "MathUtils.h"

#define EmptyNode -1
 
struct ContactPair
{
	ContactPair(int node1, int node2) :node1(node1), node2(node2){}

	bool operator == (const ContactPair& cp) const
	{
		return (node1 == cp.node1 && node2 == cp.node2) || (node1 == cp.node2 && node2 == cp.node1);
	}

	int node1;
	int node2;
};

template<class T>
struct DTNode
{
	DTNode() : parent(EmptyNode), left(EmptyNode), right(EmptyNode), height(0), userData(nullptr){  }

	void init()
	{
		parent = EmptyNode;
		left = EmptyNode;
		right = EmptyNode;
		height = 0;
		userData = nullptr;
	}
	bool isLeaf() const { return right == EmptyNode; }

	int parent;
	int left;
	int right;
	int height;
	T* userData;
	AABB aabb;	// Enlarged AABB
};

template<class T>
struct DTNode;

/*!
* \brief Dymamic tree class. It is binary tree or more precisely an AVL tree structure,
*	that means it keeps its nodes balanced. Here we keep user data only in the leaves.
*/

template<class T>
class DynamicTree
{
	friend class PhysicsEngine;

public:
	DynamicTree() : m_root(EmptyNode), m_freeList(0) {}
	/*!
	* \brief Updates the given node by removing and inserting it in the tree.
	* \param node : node index
	*/
	void updateNode(int node);
	/*!
	* \brief Clears and reinitializes the tree;
	*/
	void clean();
	/*!
	* \brief Creates and inserts a new node in the tree with the given data.
	* \param data : collision body
	* \return the index of the created node.
	*/
	int createNode(T* data);
	/*!
	* \brief Creates and inserts a new node in the tree with the given data.
	* \param data : collision body
	* \param aabb : created node init aabb
	* \return the index of the created node.
	*/
	int createNode(T* data, const AABB& aabb);
	/*!
	* \brief Find the left most node to the given node.
	* \param node : origin node
	* \return the index of the left most node.
	*/
	int findLeftMost(int node);
	/*!
	* \brief Find the right most node to the given node.
	* \param node : origin node
	* \return the index of the right most node.
	*/
	int findRightMost(int node);

private:
	/*!
	* \brief Inserts a leaf node, this involves the creation of a bounding AABB for 
	*	the user data associated with.
	*/
	void insertLeaf(int node);
	/*!
	* \brief Removes a leaf node from the tree and frees it right after. 
	*	This involves to remove the leaf parent and replaces it with the sibling leaf.
	*/
	void freeLeaf(int node);
	/*!
	* \brief Removes a leaf node from the tree just before .
	*	This involves to remove the leaf parent and replaces it with the sibling leaf.
	*/
	void removeLeaf(int node);
	/*!
	* \brief Balances the tree in order to keep the height difference between the leaves 
	*	smaller than or equal to two. This operation improves finding and insertion.
	* \return the new root index
	*/
	int balanceTree(int index);
	/*!
	* \brief Add a new node in the node vector and return its index.
	*/
	int allocateNode();
	/*!
	* \brief Free the current index in the node vector.
	*/
	void freeNode(int node);

	int m_root;

	std::vector<DTNode<T>> m_nodes;
	int m_freeList;	/* Index of the first node of the free list */
};

template<class T>
int DynamicTree<T>::findLeftMost(int node)
{
	int parent = m_nodes[node].parent;
	while (parent != EmptyNode)
	{
		if (m_nodes[parent].right == node) {
			node = m_nodes[parent].left;
			break;
		}
		node = parent;
		parent = m_nodes[parent].parent;
	}
	if (parent == EmptyNode) {
		return EmptyNode;
	}
	while (!m_nodes[node].isLeaf())
	{
		node = m_nodes[node].right;
	}
	return node;
}

template<class T>
int DynamicTree<T>::findRightMost(int node)
{
	int parent = m_nodes[node].parent;
	while (parent != EmptyNode)
	{
		if (m_nodes[parent].left == node) {
			node = m_nodes[parent].right;
			break;
		}
		parent = m_nodes[parent].parent;
	}
	if (parent == EmptyNode) {
		return EmptyNode;
	}
	while (!m_nodes[node].isLeaf())
	{
		node = m_nodes[node].left;
	}
	return node;
}

template<class T>
void DynamicTree<T>::updateNode(int node)
{
	this->removeLeaf(node);
	this->insertLeaf(node);
}

template<class T>
void DynamicTree<T>::clean()
{
	m_root = EmptyNode;
	m_freeList = EmptyNode;
	m_nodes.clear();
}

template<class T>
int DynamicTree<T>::createNode(T* data)
{
	int index = allocateNode();
	m_nodes[index].userData = data;
	insertLeaf(index);
	return index;
}

template<class T>
int DynamicTree<T>::createNode(T* data, const AABB& aabb)
{
	int index = allocateNode();
	m_nodes[index].userData = data;
	m_nodes[index].aabb = aabb;
	insertLeaf(index);
	return index;
}

template<class T>
int DynamicTree<T>::allocateNode()
{
	if (m_freeList != EmptyNode)
	{
		int nodeId = m_freeList;
		m_freeList = m_nodes[nodeId].parent;
		m_nodes[nodeId].init();
		return nodeId;
	}
	else
	{
		m_nodes.emplace_back();
		return static_cast<int>(m_nodes.size()) - 1;
	}
}

template<class T>
void DynamicTree<T>::freeNode(int node)
{
	m_nodes[node].parent = m_freeList;
	m_nodes[node].height = -1;
	m_freeList = node;
}

template<class T>
void DynamicTree<T>::insertLeaf(int leaf)
{
	if (m_root == EmptyNode)
	{
		m_root = leaf;
		m_nodes[m_root].parent = EmptyNode;
		return;
	}

	// Find the best sibling for the given node
	AABB leafAABB = m_nodes[leaf].aabb;
	int index = m_root;
	// Prioritize iteration to recursiv approach
	while (!m_nodes[index].isLeaf())
	{
		int left = m_nodes[index].left;
		int right = m_nodes[index].right;

		float area = m_nodes[index].aabb.getPerimeter();
		AABB combinedAABB;
		combinedAABB.combine(m_nodes[index].aabb, leafAABB);
		float combinedArea = combinedAABB.getPerimeter();

		// cost of creating a new parent for this node and the new leaf
		float cost = 2.0f * combinedArea;
		// extra cost or minimum cost of pushing the leaf further down the tree
		float inheritanceCost = 2.0f * (combinedArea - area);

		// cost of descending into left child
		float costLeft;
		if (m_nodes[left].isLeaf())
		{
			AABB aabb;
			aabb.combine(leafAABB, m_nodes[left].aabb);
			costLeft = aabb.getPerimeter() + inheritanceCost;
		}
		else
		{
			AABB aabb;
			aabb.combine(leafAABB, m_nodes[left].aabb);
			float oldArea = m_nodes[left].aabb.getPerimeter();
			float newArea = aabb.getPerimeter();
			costLeft = (newArea - oldArea) + inheritanceCost;
		}

		// cost of descending into right child
		float costRight;
		if (m_nodes[right].isLeaf())
		{
			AABB aabb;
			aabb.combine(leafAABB, m_nodes[right].aabb);
			costRight = aabb.getPerimeter() + inheritanceCost;
		}
		else
		{
			AABB aabb;
			aabb.combine(leafAABB, m_nodes[right].aabb);
			float oldArea = m_nodes[right].aabb.getPerimeter();
			float newArea = aabb.getPerimeter();
			costRight = (newArea - oldArea) + inheritanceCost;
		}

		// Descend according to the minimum cost
		if (cost < costLeft && cost < costRight)
			break;
		if (costLeft < costRight)
			index = left;
		else
			index = right;
	}

	int sibling = index;

	// Create new parent
	int oldParent = m_nodes[sibling].parent;
	int newParent = allocateNode();
	m_nodes[newParent].parent = oldParent;
	m_nodes[newParent].aabb.combine(leafAABB, m_nodes[sibling].aabb);
	m_nodes[newParent].height = m_nodes[sibling].height + 1;

	if (oldParent != EmptyNode)
	{
		// the sibling wasn't the root
		if (m_nodes[oldParent].left == sibling)
			m_nodes[oldParent].left = newParent;
		else
			m_nodes[oldParent].right = newParent;

		m_nodes[newParent].left = sibling;
		m_nodes[newParent].right = leaf;
		m_nodes[sibling].parent = newParent;
		m_nodes[leaf].parent = newParent;
	}
	else
	{
		// the sibling was the root
		m_nodes[newParent].left = sibling;
		m_nodes[newParent].right = leaf;
		m_nodes[sibling].parent = newParent;
		m_nodes[leaf].parent = newParent;
		m_root = newParent;
	}

	// walk back up the tree fixing heights and AABBs
	index = m_nodes[leaf].parent;
	while (index != EmptyNode)
	{
		index = balanceTree(index);

		int left = m_nodes[index].left;
		int right = m_nodes[index].right;

		m_nodes[index].aabb.combine(m_nodes[left].aabb, m_nodes[right].aabb);
		m_nodes[index].height = 1 + MathUtils::myMax(m_nodes[left].height, m_nodes[right].height);
		index = m_nodes[index].parent;
	}
}

template<class T>
inline void DynamicTree<T>::freeLeaf(int node)
{
	removeLeaf(node);
	freeNode(node);
}

template<class T>
void DynamicTree<T>::removeLeaf(int leaf)
{
	if (leaf == m_root)
	{
		m_root = EmptyNode;
		return;
	}

	int parent = m_nodes[leaf].parent;
	int grandParent = m_nodes[parent].parent;
	int sibling;
	if (m_nodes[parent].left == leaf)
		sibling = m_nodes[parent].right;
	else
		sibling = m_nodes[parent].left;

	if (grandParent != EmptyNode)
	{
		if (m_nodes[grandParent].left == parent)
			m_nodes[grandParent].left = sibling;
		else
			m_nodes[grandParent].right = sibling;

		m_nodes[sibling].parent = grandParent;
		freeNode(parent);

		// adjust ancestor bounds
		int index = grandParent;
		while (index != EmptyNode)
		{
			index = balanceTree(index);

			int left = m_nodes[index].left;
			int right = m_nodes[index].right;

			m_nodes[index].aabb.combine(m_nodes[left].aabb, m_nodes[right].aabb);
			m_nodes[index].height = 1 + MathUtils::myMax(m_nodes[left].height, m_nodes[right].height);
			index = m_nodes[index].parent;
		}
	}
	else
	{
		m_root = sibling;
		m_nodes[sibling].parent = EmptyNode;
		freeNode(parent);
	}
}

template<class T>
int DynamicTree<T>::balanceTree(int iA)
{
	DTNode<T>& A = m_nodes[iA];

	if (A.isLeaf() || A.height < 2)
		return iA;

	int iB = A.left;
	int iC = A.right;
	DTNode<T>& B = m_nodes[iB];
	DTNode<T>& C = m_nodes[iC];

	int balance = C.height - B.height;

	if (balance > 1)
	{
		int iF = C.left;
		int iG = C.right;
		DTNode<T>& F = m_nodes[iF];
		DTNode<T>& G = m_nodes[iG];

		// swap A and C
		C.left = iA;
		C.parent = A.parent;
		A.parent = iC;

		// A's old parent should point to C
		if (C.parent != EmptyNode)
		{
			if (m_nodes[C.parent].left == iA)
				m_nodes[C.parent].left = iC;
			else
				m_nodes[C.parent].right = iC;
		}
		else
			m_root = iC;

		if (F.height > G.height)
		{
			C.right = iF;
			A.right = iG;
			G.parent = iA;
			A.aabb.combine(B.aabb, G.aabb);
			C.aabb.combine(A.aabb, F.aabb);

			A.height = 1 + MathUtils::myMax(B.height, G.height);
			C.height = 1 + MathUtils::myMax(A.height, F.height);
		}
		else
		{
			C.right = iG;
			A.right = iF;
			F.parent = iA;
			A.aabb.combine(B.aabb, F.aabb);
			C.aabb.combine(A.aabb, G.aabb);

			A.height = 1 + MathUtils::myMax(B.height, F.height);
			C.height = 1 + MathUtils::myMax(A.height, G.height);
		}
		return iC;
	}

	if (balance < -1)
	{
		int iD = B.left;
		int iE = B.right;
		DTNode<T>& D = m_nodes[iD];
		DTNode<T>& E = m_nodes[iE];

		// swap A and B
		B.left = iA;
		B.parent = A.parent;
		A.parent = iB;

		// A's old parent should point to B
		if (B.parent != EmptyNode)
		{
			if (m_nodes[B.parent].left == iA)
				m_nodes[B.parent].left = iB;
			else
				m_nodes[B.parent].right = iB;
		}
		else
			m_root = iB;
		// rotate
		if (D.height > E.height)
		{
			B.right = iD;
			A.left = iE;
			E.parent = iA;
			A.aabb.combine(C.aabb, E.aabb);
			B.aabb.combine(A.aabb, D.aabb);

			A.height = 1 + MathUtils::myMax(C.height, E.height);
			B.height = 1 + MathUtils::myMax(A.height, D.height);
		}
		else
		{
			B.right = iE;
			A.left = iD;
			D.parent = iA;
			A.aabb.combine(C.aabb, D.aabb);
			B.aabb.combine(A.aabb, E.aabb);

			A.height = 1 + MathUtils::myMax(C.height, D.height);
			B.height = 1 + MathUtils::myMax(A.height, E.height);
		}
		return iB;
	}
	return iA;
}