// Copyright(c) 2013-2019, mCODE A/S
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and /or other materials provided with the distribution.
//  3. Neither the name of the copyright holders nor the
//     names of its contributors may be used to endorse or promote products
//     derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
#pragma once

#include "Exports.h"
#include "MemoryManager.h"
#include <functional>
#include <utility>
#include <type_traits>


namespace m3d
{

template<typename T, typename S, bool multimap = false, typename CMP = std::less<T>>
class Map
{
private:
	struct _Node;

public:
	struct Node;
	struct cNode;

	typedef std::pair<const T, S> KeyValuePair;

	Map()
	{
		_init();
	}

	Map(const Map &s)
	{
		_init();
		insert(s.front(), s.back());
	}

	Map(Map &&s) : _root(s._root), _nil(s._nil), _size(s._size)
	{
		s._init();
	}

	Map(const std::initializer_list<KeyValuePair> &v)
	{
		_init();
		for (auto itr = v.begin(); itr != v.end(); itr++)
			insert(*itr);
	}

	~Map()
	{
		clear();
		mmdelete(_nil);
		mmdelete(_root);
	}

	Map &operator=(const Map &s)
	{
		if (&s != this) {
			clear();
			insert(s.front(), s.back());
		}
		return *this;
	}

	Map &operator=(Map &&s)
	{
		if (&s != this) {
			clear();
			mmdelete(_nil);
			mmdelete(_root);
			_root = s._root;
			_nil = s._nil;
			_size = s._size;
			s._init();
		}
		return *this;
	}

	S &operator[](const T &t)
	{
		Node n = find(t);
		if (n.valid())
			return n->second;
		return insert(std::make_pair(t, S())).first->second;
	}

	inline size_t size() const { return _size; }
	inline bool empty() const { return _size == 0; }

	size_t insert(cNode first, cNode last)
	{
		size_t i = 0;
		for (; first.valid(); first++) {
			if (insert(first.get()).second)
				i++;
			if (first == last)
				break;
		}
		return i;
	}

	std::pair<Node, bool> insert(const KeyValuePair &newEntry)
	{
		std::pair<_Node*, bool> r = _insert(mmnew _Node(newEntry));
		return std::make_pair(Node(this, r.first), r.second);
	}

	std::pair<Node, bool> insert(KeyValuePair &&newEntry)
	{
		std::pair<_Node*, bool> r = _insert(mmnew _Node(std::move(newEntry)));
		return std::make_pair(Node(this, r.first), r.second);
	}

	inline void clear() { _clear(_root->left); _root->left = _nil; }

	inline Node erase(Node node) { return Node(this, _erase(node.n)); }

	size_t erase(const T &t) 
	{ 
		if (multimap) {
			size_t c = 0;
			for (Node n = find(t), m = findFirstAfter(t); n != m; n = erase(n), c++);
			return c;
		}
		Node n = find(t);
		if (n.valid()) {
			erase(n);
			return 1;
		}
		return 0;
	}

	inline Node front() { return Node(this, _front()); }
	inline cNode front() const { return cNode(this, _front()); }
	inline Node back() { return Node(this, _back()); }
	inline cNode back() const { return cNode(this, _back()); }
	inline Node find(const T &key) { return Node(this, _find(key)); }
	inline cNode find(const T &key) const { return cNode(this, _find(key)); }
	inline Node findFirstAfter(const T &key) { return Node(this, _findFirstAfter(key)); }
	inline cNode findFirstAfter(const T &key) const { return cNode(this, _findFirstAfter(key)); }
	inline Node findFirstBefore(const T &key) { return Node(this, _findFirstBefore(key)); }
	inline cNode findFirstBefore(const T &key) const { return cNode(this, _findFirstBefore(key)); }

	bool isSubset(const Map &subsetOfThis) const 
	{
		for (cNode n = front(); n.valid(); n++)
			if (subsetOfThis.find(n.get().first).invalid())
				return false;
		return true;
	}

	struct Node 
	{
		Map *s;
		_Node *n;

		Node(Map *s = nullptr, _Node *n = nullptr) : s(s), n(n) {}
		explicit Node(Map *s) : s(s), n(s->_nil) {}

		bool operator==(const Node &rhs) const { return n == rhs.n; }
		bool operator==(const cNode &rhs) const { return n == rhs.n; }
		bool operator!=(const Node &rhs) const { return n != rhs.n; }
		bool operator!=(const cNode &rhs) const { return n != rhs.n; }

		KeyValuePair *operator->() { return &n->get(); }
		const KeyValuePair *operator->() const { return &n->get(); }

		Node operator+(int n) const { return Node(*this) += n; }
		Node operator-(int n) const { return Node(*this) -= n; }

		Node &operator+=(int n) 
		{ 
			if (n > 0)
				while (n--)
					(*this)++;
			else if (n < 0)
				while (n++)
					(*this)--;
			return *this; 
		}

		Node &operator-=(int n) 
		{ 
			if (n > 0)
				while (n--)
					(*this)--;
			else if (n < 0)
				while (n++)
					(*this)++;
			return *this; 
		}

		Node &operator--() { n = s->_prev(n); return *this; }
		Node &operator++() { n = s->_next(n); return *this; }
		Node operator--(int) { Node r = *this; n = s->_prev(n); return r; }
		Node operator++(int) { Node r = *this; n = s->_next(n); return r; }

		bool valid() const { return n && n != s->_nil; }
		bool invalid() const { return !n || n == s->_nil; }

		KeyValuePair& get() { return n->get(); }
		const KeyValuePair& get() const { return n->get(); }
	};

	struct cNode 
	{
		const Map *s;
		const _Node *n;

		cNode(const Map *s = nullptr, const _Node *n = nullptr) : s(s), n(n) {}
		cNode(const Node &n) : s(n.s), n(n.n) {}
		explicit cNode(const Map *s) : s(s), n(s->_nil) {}

		bool operator==(const Node &rhs) const { return n == rhs.n; }
		bool operator==(const cNode &rhs) const { return n == rhs.n; }
		bool operator!=(const Node &rhs) const { return n != rhs.n; }
		bool operator!=(const cNode &rhs) const { return n != rhs.n; }

		const KeyValuePair *operator->() const { return &n->get(); }

		cNode operator+(int n) const { return cNode(*this) += n; }
		cNode operator-(int n) const { return cNode(*this) -= n; }

		cNode &operator+=(int n) 
		{ 
			if (n > 0)
				while (n--)
					(*this)++;
			else if (n < 0)
				while (n++)
					(*this)--;
			return *this; 
		}

		cNode &operator-=(int n) 
		{ 
			if (n > 0)
				while (n--)
					(*this)--;
			else if (n < 0)
				while (n++)
					(*this)++;
			return *this; 
		}

		cNode &operator--() { n = s->_prev(n); return *this; }
		cNode &operator++() { n = s->_next(n); return *this; }
		cNode operator--(int) { cNode r = *this; n = s->_prev(n); return r; }
		cNode operator++(int) { cNode r = *this; n = s->_next(n); return r; }

		bool valid() const { return n && n != s->_nil; }
		bool invalid() const { return !n || n == s->_nil; }

		const KeyValuePair& get() const { return n->get(); }
	};

// Private stuff for now only!
private:
	struct _Node
	{
		_Node(const KeyValuePair &key) : key(key), red(0) {}
		_Node(KeyValuePair &&key) : key(std::move(key)), red(0) {}
		~_Node() {}

		std::pair<const T, S> &get() { return key; }
		const std::pair<const T, S> &get() const { return key; }

		struct cmp
		{
			bool operator()(const KeyValuePair &l, const KeyValuePair &r) { return CMP()(l.first, r.first); }
			bool operator()(const T &l, const KeyValuePair &r) { return CMP()(l, r.first); }
			bool operator()(const KeyValuePair &l, const T &r) { return CMP()(l.first, r); }
		};

		KeyValuePair key;
		int red; // if red=0 then the node is black
		_Node *left;
		_Node *right;
		_Node *parent;
	};


	_Node *_root;
	_Node *_nil;
	size_t _size;

	void LeftRotate(_Node *x)
	{
		_Node* y;

		y=x->right;
		x->right=y->left;

		if (y->left != _nil) 
			y->left->parent=x; 

		y->parent=x->parent;   

		if( x == x->parent->left) {
			x->parent->left=y;
		} else {
			x->parent->right=y;
		}
		y->left=x;
		x->parent=y;
	}

	void RightRotate(_Node *y)
	{
		_Node* x;

		x=y->left;
		y->left=x->right;

		if (_nil != x->right)  
			x->right->parent=y; 

		x->parent=y->parent;
		if( y == y->parent->left) {
			y->parent->left=x;
		} else {
			y->parent->right=x;
		}
		x->right=y;
		y->parent=x;
	}

	_Node *_treeInsertHelp(_Node *z)
	{
		//  This function should only be called by RedBlackTree::Insert
		_Node* x;
		_Node* y;

		z->left=z->right=_nil;
		y=_root;
		x=_root->left;
		while( x != _nil) {
			y=x;
			if ( _Node::cmp()(z->key, x->key) ) {
				x=x->left;
			}
			else if (multimap || _Node::cmp()(x->key, z->key)) {
				x=x->right;
			}
			else
				return x;
		}
		z->parent=y;
		if ( (y == _root) || _Node::cmp()(z->key, y->key) ) { 
			y->left=z;
		} else {
			y->right=z;
		}
		return z;
	}

	void _deleteFixUp(_Node *x)
	{
		_Node * w;
		_Node * rootLeft = _root->left;

		while( (!x->red) && (rootLeft != x)) {
			if (x == x->parent->left) {
				w=x->parent->right;
				if (w->red) {
					w->red=0;
					x->parent->red=1;
					LeftRotate(x->parent);
					w=x->parent->right;
				}
				if ( (!w->right->red) && (!w->left->red) ) { 
					w->red=1;
					x=x->parent;
				} else {
					if (!w->right->red) {
						w->left->red=0;
						w->red=1;
						RightRotate(w);
						w=x->parent->right;
					}
					w->red=x->parent->red;
					x->parent->red=0;
					w->right->red=0;
					LeftRotate(x->parent);
					x=rootLeft; // this is to exit while loop
				}
			} else { // the code below is has left and right switched from above
				w=x->parent->left;
				if (w->red) {
					w->red=0;
					x->parent->red=1;
					RightRotate(x->parent);
					w=x->parent->left;
				}
				if ( (!w->right->red) && (!w->left->red) ) { 
					w->red=1;
					x=x->parent;
				} else {
					if (!w->left->red) {
						w->right->red=0;
						w->red=1;
						LeftRotate(w);
						w=x->parent->left;
					}
					w->red=x->parent->red;
					x->parent->red=0;
					w->left->red=0;
					RightRotate(x->parent);
					x=rootLeft; // this is to exit while loop
				}
			}
		}
		x->red=0;
	}

	void _clear(_Node *n)
	{
		if (n != _nil) {
			_clear(n->left);
			_clear(n->right);
			mmdelete(n);
			_size--;
		}
	}

	_Node *_prev(const _Node *x) const
	{
		_Node* y;

		if (x == _nil)
			x = _back(); // next on nil returns last!
		if (x == _nil)
			return _nil;

		if (_nil != (y = x->left)) { /* assignment to y is intentional */
			while(y->right != _nil) { /* returns the maximum of the left subtree of x */
				y=y->right;
			}
			return(y);
		} else {
			y=x->parent;
			while(x == y->left) { 
				if (y == _root) return(_nil); 
				x=y;
				y=y->parent;
			}
			return(y);
		}
	}
  
	_Node *_next(const _Node *x) const
	{
		_Node* y;

		if (x == _nil)
			x = _front(); // next on nil returns 1st!
		if (x == _nil)
			return _nil;

		if (_nil != (y = x->right)) { /* assignment to y is intentional */
			while(y->left != _nil) { /* returns the minium of the right subtree of x */
				y=y->left;
			}
			return(y);
		} else {
			y=x->parent;
			while(x == y->right) { /* sentinel used instead of checking for _nil */
				x=y;
				y=y->parent;
			}
			if (y == _root) 
				return(_nil);
			return(y);
		}
	}

	_Node *_front() const
	{
		_Node *n = _root->left;
		while (n->left != _nil)
			n = n->left;
		return n;
	}

	_Node *_back() const
	{
		_Node *n = _root->left;//->right;
		while (n->right != _nil)
			n = n->right;
		return n;
	}

	_Node *_find(const T &key) const
	{
		_Node *n = _root->left;
		while (n != _nil) {
			if (_Node::cmp()(key, n->key))
				n = n->left;
			else if (_Node::cmp()(n->key, key))
				n = n->right;
			else {
				if (multimap) {
					for (_Node *m = _prev(n); m != _nil; n = m, m = _prev(m))
						if (_Node::cmp()(m->key, key))
							break;
				}
				return n;
			}
		}
		return _nil;
	}

	_Node *_findFirstAfter(const T &key) const
	{
		_Node *r = _nil;
		_Node *n = _root->left;
		while (n != _nil) {
			if (_Node::cmp()(key, n->key)) {
 				r = n;
				n = n->left;
			}
			else
				n = n->right;
		}
		return r;
	}

	_Node *_findFirstBefore(const T &key) const
	{
		_Node *r = _nil;
		_Node *n = _root->left;
		while (n != _nil) {
			if (_Node::cmp()(n->key, key)) {
				r = n;
				n = n->right;
			}
			else
				n = n->left;
		}
		return r;
	}

	void _init()
	{
		_nil = mmnew _Node(KeyValuePair());
		_nil->left = _nil->right = _nil->parent = _nil;
		_root = mmnew _Node(KeyValuePair());
		_root->left = _root->right = _root->parent = _nil;	
		_size = 0;
	}

	std::pair<_Node*, bool> _insert(_Node *newEntry)
	{
		_Node * y;
		_Node * x;
		_Node * newNode;

		x = newEntry;
		newNode = _treeInsertHelp(x);
		if (newNode != x) {
			mmdelete(x);
			return std::make_pair(newNode, false);
		}
		newNode = x;
		x->red=1;
		while(x->parent->red) { // use sentinel instead of checking for _root
			if (x->parent == x->parent->parent->left) {
				y=x->parent->parent->right;
				if (y->red) {
					x->parent->red=0;
					y->red=0;
					x->parent->parent->red=1;
					x=x->parent->parent;
				} else {
					if (x == x->parent->right) {
						x=x->parent;
						LeftRotate(x);
					}
					x->parent->red=0;
					x->parent->parent->red=1;
					RightRotate(x->parent->parent);
				} 
			} else { 
				// case for x->parent == x->parent->parent->right
				// this part is just like the section above with
				// left and right interchanged
				y=x->parent->parent->left;
				if (y->red) {
					x->parent->red=0;
					y->red=0;
					x->parent->parent->red=1;
					x=x->parent->parent;
				} else {
					if (x == x->parent->left) {
						x=x->parent;
						RightRotate(x);
					}
					x->parent->red=0;
					x->parent->parent->red=1;
					LeftRotate(x->parent->parent);
				} 
			}
		}
		_root->left->red=0;
		_size++;
		return std::make_pair(newNode, true);
	}

	// Returns first node after the one erased.
	_Node *_erase(_Node *z) 
	{
		_Node* y;
		_Node* x;

		if (z == _nil)
			return _nil;

		_Node* nextNode = _next(z);

		y = ((z->left == _nil) || (z->right == _nil)) ? z : _next(z);
		x = (y->left == _nil) ? y->right : y->left;
		if (_root == (x->parent = y->parent)) // assignment of y->p to x->p is intentional
			_root->left=x;
		else {
			if (y == y->parent->left)
				y->parent->left=x;
			else
				y->parent->right=x;
		}
		if (y != z) { // y should not be _nil in this case
			y->left=z->left;
			y->right=z->right;
			y->parent=z->parent;
			z->left->parent=z->right->parent=y;
			if (z == z->parent->left)
				z->parent->left=y; 
			else
				z->parent->right=y;
			if (!(y->red)) {
				y->red = z->red;
				_deleteFixUp(x);
			} 
			else
				y->red = z->red; 

			mmdelete(z);
		} 
		else {
			if (!(y->red)) 
				_deleteFixUp(x);
			mmdelete(y);
		}
		_size--;
		return nextNode;
	}

};

template<typename T, typename S, bool multimap, typename CMP>
bool operator==(const Map<T, S, multimap, CMP>& a, const Map<T, S, multimap, CMP>& b)
{
	if (a.size() != b.size())
		return false;

	for (Map<T, S, multimap, CMP>::cNode n = a.front(), m = b.front(); n.valid() && m.valid(); n++, m++)
		if (!(n.get() == m.get()))
			return false;
	return true;
}

template<typename T, typename S, bool multimap, typename CMP>
bool operator!=(const Map<T, S, multimap, CMP>& a, const Map<T, S, multimap, CMP>& b)
{
	if (a.size() != b.size())
		return true;

	for (Map<T, S, multimap, CMP>::cNode n = a.front(), m = b.front(); n.valid() && m.valid(); n++, m++)
		if (n.get() != m.get())
			return true;
	return false;
}

template<typename T, typename S, bool multimap, typename CMP>
bool operator<(const Map<T, S, multimap, CMP>& a, const Map<T, S, multimap, CMP>& b)
{
	for (Map<T, S, multimap, CMP>::cNode n = a.front(), m = b.front(); n.valid() && m.valid(); n++, m++) {
		if (n.get() < m.get())
			return true;
		if (m.get() < n.get())
			return false;
	}
	return a.size() < b.size(); // if we have fewer nodes, we are smallest!
}


}

