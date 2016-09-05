#pragma once
#include <iostream>
#include <stack>


template<class ElemType>
class BinTree
{
	enum Color { BLACK, RED };

	struct Node
	{
		Node(const ElemType & data, Color _clr, Node *_parent);
		Node(const ElemType & data, Color _clr, Node *_parent, Node *_left, Node *_right);

		ElemType value;
		
		Color clr;
	
		Node *parent;
		Node *left;
		Node *right;

		friend std::ostream& operator<<(std::ostream& os, const Node &dt);

	};

	class Iterator
	{
		private:
			Node *ptr;
			std::stack<Node*> nodes;
			std::stack<Node*> r_nodes;

		public:
			friend BinTree;

			Iterator(Node* p);

			operator bool() const;
			
			Iterator &operator++ ();
			Iterator &operator-- ();

			ElemType &operator* ();
			ElemType &operator-> ();

			bool operator== (Iterator &right);
			bool operator!= (Iterator &right);

	};

	private:
		Node *root;
		unsigned _size;

		void PostOrderDelete(Node* _node);

		void rotate_left(Node *n);
		void rotate_right(Node *n);

		void balancing(Node *new_node);
		void after_delete_balancing(Node *deleted_node);

		Node *searching(const ElemType &key, Node *_node) const;

	public:
		BinTree();
		~BinTree();
		
		Iterator begin();
		Iterator null_iter();
		Iterator end();

		Iterator find(const ElemType &key) const;

		void insert(const ElemType &val);

		unsigned size();
		bool empty();

		void swap(BinTree tree);


		void remove(const ElemType &key);
		void remove(Iterator key);
		void clear();
};


#pragma region NODE

template<typename ElemType>
BinTree<ElemType>::Node::Node(const ElemType & data, Color _clr, Node *_parent)
	: value(data), clr(_clr), parent(_parent), right(nullptr), left(nullptr)
{}

template<typename ElemType>
BinTree<ElemType>::Node::Node(const ElemType & data, Color _clr, Node *_parent, Node *_left, Node *_right)
	: value(data), clr(_clr), parent(_parent), right(_right), left(_left)
{}

//----------------------------------------------------------

template<typename ElemType>
std::ostream& operator<<(std::ostream& os, const typename BinTree<ElemType>::Node &dt)
{
	os << dt.value;
	return os;
}

#pragma endregion



#pragma region ITERATOR

template<typename ElemType>
BinTree<ElemType>::Iterator::Iterator(Node *p) : ptr(p)
{}

//----------------------------------------------------------

template<typename ElemType>
BinTree<ElemType>::Iterator::operator bool() const
{
	return ptr ? true : false;
}

//----------------------------------------------------------

template<typename ElemType>
typename BinTree<ElemType>::Iterator &BinTree<ElemType>::Iterator::operator++ ()
{
	if (ptr)
	{
		if (ptr->right)
		{
			ptr = ptr->right;
			while (ptr->left)
			{
				nodes.push(ptr);
				ptr = ptr->left;
			}
		}
		else
		{
			r_nodes.push(ptr);
			if (nodes.empty())
			{
				ptr = nullptr;
			}
			else
			{
				ptr = nodes.top();
				nodes.pop();
			}
		}
	}
	return *this;
}


template<typename ElemType>
typename BinTree<ElemType>::Iterator &BinTree<ElemType>::Iterator::operator-- ()
{
	if (!ptr)
		return end();

	if (ptr->left)
	{
		ptr = ptr->left;
		while (ptr->right)
		{
			r_nodes.push(ptr);
			ptr = ptr->right;
		}
	}
	else
	{
		nodes.push(ptr);
		if (nodes.empty())
		{
			ptr = nullptr;
		}
		else
		{
			ptr = r_nodes.top();
			nodes.pop();
		}
	}

	return *this;
}

//----------------------------------------------------------

template<typename ElemType>
ElemType& BinTree<ElemType>::Iterator::operator* ()
{
	return ptr->value;
}

template<typename ElemType>
ElemType& BinTree<ElemType>::Iterator::operator-> ()
{
	return ptr->value;
}

//----------------------------------------------------------

template<typename ElemType>
bool BinTree<ElemType>::Iterator::operator== (Iterator &right_expr)
{
	return ptr == right_expr.ptr;
}

template<typename ElemType>
bool BinTree<ElemType>::Iterator::operator!= (Iterator &right_expr)
{
	return ptr != right_expr.ptr;
}

#pragma endregion


#pragma region BINTREE

//-------------------------PRIVATE--------------------------

template<typename ElemType>
void BinTree<ElemType>::PostOrderDelete(Node *_node)
{
	if (_node) {
		PostOrderDelete(_node->left);
		PostOrderDelete(_node->right);
		delete _node;
	}
}

//----------------------------------------------------------

template<typename ElemType>
void BinTree<ElemType>::rotate_left(Node *n)
{
	Node *pivot = n->right;

	pivot->parent = n->parent; 
	if (n->parent) {
		if (n->parent->left == n)
			n->parent->left = pivot;
		else
			n->parent->right = pivot;
	}

	n->right = pivot->left;
	if (pivot->left)
		pivot->left->parent = n;

	n->parent = pivot;
	pivot->left = n;
}


template<typename ElemType>
void BinTree<ElemType>::rotate_right(Node *n)
{
	Node *pivot = n->left;

	pivot->parent = n->parent; 
	if (n->parent) 
	{
		if (n->parent->left == n)
			n->parent->left = pivot;
		else
			n->parent->right = pivot;
	}

	n->left = pivot->right;
	if (pivot->right)
		pivot->right->parent = n;

	n->parent = pivot;
	pivot->right = n;
}

//----------------------------------------------------------

template<typename ElemType>
void BinTree<ElemType>::balancing(Node *x)
{
	while (x != root && x->parent->clr == RED)
	{
		if (x->parent == x->parent->parent->right)
		{
			Node *y = x->parent->parent->right;
			if (y->clr == RED)
			{	
				x->parent->clr = BLACK;
				y->clr = BLACK;
				x->parent->parent->clr = RED;
				x = x->parent->parent;
			}
			else {

				if (x == x->parent->right) 
				{
					x = x->parent;
					rotate_left(x);
				}

				x->parent->clr = BLACK;
				x->parent->parent->clr = RED;
				rotate_right(x->parent->parent);
			}
		}
		else 
		{
			Node *y = x->parent->parent->left;
			if (y->clr == RED) {

				x->parent->clr = BLACK;
				y->clr = BLACK;
				x->parent->parent->clr = RED;
				x = x->parent->parent;
			}
			else {

				if (x == x->parent->left) {
					x = x->parent;
					rotate_right(x);
				}
				x->parent->clr = BLACK;
				x->parent->parent->clr = RED;
				rotate_left(x->parent->parent);
			}
		}
	}
	root->clr = BLACK;
}


template<typename ElemType>
void BinTree<ElemType>::after_delete_balancing(Node *x)
{
	while (x != root && x->clr == BLACK)
	{
		if (x == x->parent->left)
		{
			Node *w = x->parent->right;
			if (w->clr == RED)
			{
				w->clr = BLACK;
				x->parent->clr = RED;
				rotate_left(x->parent);
				w = x->parent->right;
			}
			if (w->left->clr == BLACK && w->right->clr == BLACK)
			{
				w->clr = RED;
				x = x->parent;
			}
			else 
			{
				if (w->right->clr == BLACK)
				{
					w->left->clr = BLACK;
					w->clr = RED;
					rotate_right(w);
					w = x->parent->right;
				}
				w->clr = x->parent->clr;
				x->parent->clr = BLACK;
				w->right->clr = BLACK;
				rotate_left(x->parent);
				x = root;
			}
		}
		else 
		{
			Node *w = x->parent->left;
			if (w->clr == RED)
			{
				w->clr = BLACK;
				x->parent->clr = RED;
				rotate_right(x->parent);
				w = x->parent->left;
			}
			if (w->right->clr == BLACK && w->left->clr == BLACK)
			{
				w->clr = RED;
				x = x->parent;
			}
			else 
			{
				if (w->left->clr == BLACK)
				{
					w->right->clr = BLACK;
					w->clr = RED;
					rotate_left(w);
					w = x->parent->left;
				}
				w->clr = x->parent->clr;
				x->parent->clr = BLACK;
				w->left->clr = BLACK;
				rotate_right(x->parent);
				x = root;
			}
		}
	}

	x->clr = BLACK;
}

//----------------------------------------------------------

template<typename ElemType>
typename BinTree<ElemType>::Node *BinTree<ElemType>::searching(const ElemType &key, Node *_node) const
{
	if (key == _node->value || _node == nullptr)
		return _node;
	
	key < _node->value ? searching(key, _node->left) : searching(key, _node->right);
}

//-------------------------PUBLIC---------------------------

template<typename ElemType>
BinTree<ElemType>::BinTree() : root(nullptr), _size(0)
{}

template<typename ElemType>
BinTree<ElemType>::~BinTree()
{
	clear();
}

//----------------------------------------------------------

template<typename ElemType>
typename BinTree<ElemType>::Iterator BinTree<ElemType>::begin()
{
	Iterator runner(root);

	while (runner.ptr->left)
	{
		runner.nodes.push(runner.ptr);
		runner.ptr = runner.ptr->left;
	}

	return runner;
}


template<typename ElemType>
typename BinTree<ElemType>::Iterator BinTree<ElemType>::null_iter()
{
	return Iterator (nullptr);
}


template<typename ElemType>
typename BinTree<ElemType>::Iterator BinTree<ElemType>::end()
{
	Iterator runner(root);

	while (runner.ptr->right)
	{
		runner.r_nodes.push(runner.ptr);
		runner.ptr = runner.ptr->right;
	}
		
	return runner;
}

//----------------------------------------------------------

template<typename ElemType>
typename BinTree<ElemType>::Iterator BinTree<ElemType>::find(const ElemType &key) const
{
	return Iterator(searching(key, root));
}

//----------------------------------------------------------

template<typename ElemType>
void BinTree<ElemType>::insert(const ElemType &val)
{
	if (root)
	{
		Node *runner, *runner_next = root;

		do{
			runner = runner_next;
			if (val > runner->value) runner_next = runner->right;
				else if (val < runner->value) runner_next = runner->left;
					else return;

		} while (runner_next);

		Node *new_node = new Node(val, RED, runner);

		val < runner->value ? runner->left = new_node : runner->right = new_node;

		balancing(new_node);
	}
	else
	{
		root = new Node(val, BLACK, nullptr);
	}

	_size++;
}

//----------------------------------------------------------

template<typename ElemType>
unsigned BinTree<ElemType>::size()
{
	return size;
}

template<typename ElemType>
bool BinTree<ElemType>::empty()
{
	return size ? true : false;
}

//----------------------------------------------------------

template<typename ElemType>
void BinTree<ElemType>::swap(BinTree tree)
{
	Node *n_buf = root;
	unsigned u_buf = size;
	
	root = tree->root;
	size = tree->size;

	tree->root = n_buf;
	tree->size = u_buf;
}

//----------------------------------------------------------

template<typename ElemType>
void BinTree<ElemType>::remove(const ElemType &key)
{
	remove(find(key));
}

template<typename ElemType>
void BinTree<ElemType>::remove(Iterator key)
{
	{
		Node *x, *y, *z = key.ptr;

		if (!z || z == nullptr) return;
		
		if (z->left == nullptr || z->right == nullptr)
		{
			y = z;
		}
		else 
		{
			y = z->right;
			while (y->left) y = y->left;
		}

		
		if (y->left)	x = y->left;
			else x = y->right;

		x->parent = y->parent;

		if (y->parent)
			if (y == y->parent->left) y->parent->left = x;
				else y->parent->right = x;
					else root = x;

		if (y != z) z->value = y->value;

		if (y->clr == BLACK)
			after_delete_balancing(x);

		delete y;
	}
}

template<typename ElemType>
void BinTree<ElemType>::clear()
{
	PostOrderDelete(root);
}

#pragma endregion

