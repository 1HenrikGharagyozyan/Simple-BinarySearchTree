#pragma once
#include <iostream>
#include <cstddef>
#include <exception>
#include <initializer_list>
#include <utility>

template<typename T>
class BinarySearchTree
{
public:
	BinarySearchTree();

	~BinarySearchTree();

	BinarySearchTree(std::initializer_list<T> init);

	BinarySearchTree(const BinarySearchTree<T>& other);

	BinarySearchTree(BinarySearchTree<T>&& other);

	BinarySearchTree<T>& operator=(const BinarySearchTree<T>& other);

	BinarySearchTree<T>& operator=(BinarySearchTree<T>&& other);

	std::size_t size() const;

	T min() const;

	T max() const;

	std::size_t height() const;

	bool find(const T& value);

	void insert(const T& value);

	void reserve();

	void remove(const T& value);

	bool empty() const;

	bool operator==(const BinarySearchTree<T>& other) const;

	bool operator!=(const BinarySearchTree<T>& other) const;

private:
	struct Node
	{
		T value;
		Node* left;
		Node* right;
	};

	Node* copy(Node* node);

	void add(Node** node, const T& value);

	void deleteTree(Node* node);

	bool lookup(Node* node, const T& value) const;

	Node* minNode(Node* node) const;

	Node* maxNode(Node* node) const;

	std::size_t heightHelper(Node* node);

	void removeNode(Node*& node, T value) const;

	bool isIdentical(const Node* first, const Node* second) const;

private:
	std::size_t tree_size;
	Node* root;
};

/********************
*					*
*	Implementation	*
*					*
*********************/

template<typename T>
BinarySearchTree<T>::BinarySearchTree()
	: root(nullptr)
	, tree_size(0)
{
}

template<typename T>
BinarySearchTree<T>::~BinarySearchTree()
{
	deleteTree(this->root);
}

template<typename T>
BinarySearchTree<T>::BinarySearchTree(std::initializer_list<T> init)
{
	for (auto x : init)
		insert(x);
}

template<typename T>
BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree<T>& other)
	: root(nullptr)
	, tree_size(other.tree_size)
{
	if (this != &other && other.root != nullptr)
		root = copy(other.root);
}

template<typename T>
BinarySearchTree<T>::BinarySearchTree(BinarySearchTree<T>&& other)
	: root(std::exchange(other.root, nullptr))
	, tree_size(std::exchange(other.tree_size, 0))
{
}

template<typename T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(const BinarySearchTree<T>& other)
{
	if (this == &other)
		return *this;
	this->~BinarySearchTree();
	other.root = nullptr;
	tree_size = other.tree_size;
	root = copy(other.root);
	return *this;
}

template<typename T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(BinarySearchTree<T>&& other)
{
	if (this != &other)
	{
		this->~BinarySearchTree();
		root = other.root;
		tree_size = other.tree_size;
		other.root = nullptr;
		other.tree_size = 0;
	}
	return *this;
}

template<typename T>
inline std::size_t BinarySearchTree<T>::size() const
{
	return tree_size;
}

template<typename T>
inline T BinarySearchTree<T>::min() const
{
	return minNode(root)->value;
}

template<typename T>
inline T BinarySearchTree<T>::max() const
{
	return maxNode(root)->value;
}

template<typename T>
inline std::size_t BinarySearchTree<T>::height() const
{
	return heightHelper(root);
}

template<typename T>
bool BinarySearchTree<T>::find(const T& value)
{
	return lookup(this->root, value);
}

template<typename T>
void BinarySearchTree<T>::insert(const T& value)
{
	add(&root, value);
}

template<typename T>
inline void BinarySearchTree<T>::reserve()
{
	Node* temp = root;
	root = nullptr;
	deleteTree(temp);
}

template<typename T>
void BinarySearchTree<T>::remove(const T& value)
{
	removeNode(root, value);
}

template<typename T>
inline bool BinarySearchTree<T>::empty() const
{
	return root == nullptr ? true : false;
}

template<typename T>
inline bool BinarySearchTree<T>::operator==(const BinarySearchTree<T>& other) const
{
	return isIdentical(root, other.root);
}

template<typename T>
inline bool BinarySearchTree<T>::operator!=(const BinarySearchTree<T>& other) const
{
	return *this == other ? false : true;
}

template<typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::copy(Node* node)
{
	if (node == nullptr)
		return nullptr;

	Node* newNode = new Node{ node->value };
	newNode->left = copy(node->left);
	newNode->right = copy(node->right);
	return newNode;
}

template<typename T>
void BinarySearchTree<T>::add(Node** node, const T& value)
{
	if (*node == nullptr)
	{
		Node* newNode = new Node{ value, nullptr, nullptr };
		*node = newNode;
		++tree_size;
	}
	else
	{
		if (value > (*node)->value)
			add(&(*node)->right, value);
		else
			add(&(*node)->left, value);
	}

}

template<typename T>
void BinarySearchTree<T>::deleteTree(Node* node)
{
	if (node != nullptr)
	{
		deleteTree(node->left);
		deleteTree(node->right);
		delete node;
		node = nullptr;
	}
	tree_size = 0;
}

template<typename T>
bool BinarySearchTree<T>::lookup(Node* node, const T& value) const
{
	if (node == nullptr)
		return false;

	if (value == node->value)
		return true;
	else if (value > node->value)
		return lookup(node->right, value);
	else
		return lookup(node->left, value);
}

template<typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::minNode(Node* node) const
{
	if (node == nullptr)
		return nullptr;
	if (node->left == nullptr)
		return node;
	minNode(node->left);
}

template<typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::maxNode(Node* node) const
{
	if (node == nullptr)
		return nullptr;
	if (node->right == nullptr)
		return node;
	maxNode(node->right);
}

template<typename T>
std::size_t BinarySearchTree<T>::heightHelper(Node* node)
{
	if (node == nullptr)
		return 0;

	std::size_t leftHeight = heightHelper(node->left);
	std::size_t rightHeight = heightHelper(node->right);
	return std::max(leftHeight, rightHeight) + 1;
}

template<typename T>
void BinarySearchTree<T>::removeNode(Node*& node, T value) const
{
	if (node == nullptr)
		return;

	if (value > node->value)
		removeNode(node->right, value);
	else if (value < node->value)
		removeNode(node->left, value);
	else
	{
		if (node->left == nullptr && node->right == nullptr)
		{
			delete node;
			node = nullptr;
		}
		else if (node->left == nullptr || node->right == nullptr)
		{
			Node* temp = node;
			if (node->left != nullptr)
				node = node->left;
			else
				node = node->right;
			delete temp;
		}
		else
		{
			node->value = minNode(root)->value;
			removeNode(node->right, minNode(root)->value);
		}
	}
}

template<typename T>
bool BinarySearchTree<T>::isIdentical(const Node* first, const Node* second) const
{
	if (first == nullptr && second == nullptr)
		return true;

	if (first != nullptr && second != nullptr)
	{
		if (first->value == second->value)
			return isIdentical(first->left, second->left) && isIdentical(first->right, second->right);
	}

	return false;
}


