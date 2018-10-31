/*
 * Implement your functionality here without changing given test cases
 */
#include "Map.hpp"

namespace cs540{
	/*
	Implementation of Map
	*/
	template<typename Key_T, typename Mapped_T> // copy constructor
	Map<Key_T, Mapped_T>::Map(const Map& m){
		this->root = deep_copy(m.root);
		this->end_node = new AVLNode(m.end_node->valObj);
		this->end_node->height = -1;
		this->size_map = m.size_map;
	}

	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::AVLNode* Map<Key_T, Mapped_T>::deep_copy(AVLNode* node){
		if (node == NULL) return NULL;
		AVLNode* copy = new AVLNode(node->valObj);
		AVLNode* l = deep_copy(node->left);
		AVLNode* r = deep_copy(node->right);
		copy->left = l;
		copy->right = r;
		if (l != NULL) l->parent = copy;
		if (r != NULL) r->parent = copy;
		copy->height = node->height;
		return copy;
	}

	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>& Map<Key_T, Mapped_T>::operator=(const Map<Key_T, Mapped_T>& m){
		if (this->root == m.root) {
			return *this;
		}
		// delete original tree
		// std::queue<AVLNode*> q;
		// if (this->root != NULL) q.push(this->root);
		// AVLNode* node = NULL;
		// while (!q.empty()) {
		// 	node = q.front();
		// 	q.pop();
		// 	if (node->left != NULL) q.push(node->left);
		// 	if (node->right != NULL) q.push(node->right);
		// 	if (node->reference_num > 0) {
		// 		-- node->reference_num;
		// 	} else if (node->reference_num == 0) {
		// 		delete(node);
		// 	}
		// }
		clear();
		if (this->end_node != NULL) {
			if(this->end_node->reference_num > 0) {
				-- this->end_node->reference_num;
			} else if (this->end_node->reference_num == 0) {
				delete(this->end_node);
			}
		}


		// increase reference_num

		this->root = m.root;
		this->end_node = m.end_node;
		this->size_map = m.size_map;
		copy_assign_helper(this->root);
		++ this->end_node->reference_num;
		return *this;
	}



	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::copy_assign_helper(AVLNode* node){
		if (node == NULL) return;
		++ node->reference_num;
		copy_assign_helper(node->left);
		copy_assign_helper(node->right);
	}


	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> l){
		this->size_map = 0;
		auto it = l.begin();
		end_node = new AVLNode(*it);
		for (; it != l.end(); it++) {
			insert(*it);
			++ this->size_map;
		}
		end_node->height = -1;
	}

	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::~Map(){
		clear();
	}

	template<typename Key_T, typename Mapped_T>
	size_t Map<Key_T, Mapped_T>::size() const {
		return this->size_map;
	}

	template<typename Key_T, typename Mapped_T>
	bool Map<Key_T, Mapped_T>::empty() const {
		return this->size_map == 0;
	}

	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::begin() {
		if (root == NULL) {
			return Iterator(end_node, this);
		}
		Map<Key_T, Mapped_T>::AVLNode* node = root;
		while (node->left != NULL) {
			node = node->left;
		}
		return Iterator(node, this);
	}

	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::end() {
		return Iterator(end_node, this);
	}

	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::begin() const {
		if (root == NULL) {
			return ConstIterator(end_node, this);
		}
		Map<Key_T, Mapped_T>::AVLNode* node = root;
		while (node->left != NULL) {
			node = node->left;
		}
		return ConstIterator(node, this);
	}
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::end() const {
		return ConstIterator(end_node, this);
	}

	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::rbegin() {
		if (root == NULL) {
			return ReverseIterator(end_node, this);
		}
		AVLNode* tmp = root;
		while (tmp->right != NULL) tmp = tmp->right;
		return ReverseIterator(tmp, this);
	}
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::rend() {
		ReverseIterator rit = ReverseIterator(end_node, this);
		return rit;
	}

	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::find(const Key_T& k) {
		AVLNode* res = find_helper(k, root);
		if ( res == NULL) {
			return Iterator(end_node, this);
		} else {
			return Iterator(res, this);
		}
	}

	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::find(const Key_T& k) const {
		AVLNode* res = find_helper(k, root);
		if ( res == NULL) {
			return ConstIterator(end_node, this);
		} else {
			return ConstIterator(res, this);
		}
	}

	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::AVLNode* Map<Key_T, Mapped_T>::find_helper(const Key_T& k, AVLNode* node) const{
		if (node == NULL) return NULL;
		if (node->valObj.first == k) {
			return node;
		}
		if (k < node->valObj.first) {
			return find_helper(k, node->left);
		} else {
			return find_helper(k, node->right);
		}
	}

	template<typename Key_T, typename Mapped_T>
	Mapped_T& Map<Key_T, Mapped_T>::at(const Key_T& k) {
		return at_helper(k, root);
	}

	template<typename Key_T, typename Mapped_T>
	Mapped_T& Map<Key_T, Mapped_T>::at_helper(const Key_T& k, Map<Key_T, Mapped_T>::AVLNode* node) {
		if (node == NULL) {
			throw std::out_of_range("Not found the key.");
		} else {
			if (node->valObj.first == k) {
				return *(&node->valObj.second);
			} else if (k < node->valObj.first) {
				return at_helper(k, node->left);
			} else {
				return at_helper(k, node->right);
			}
		}
	}


	template<typename Key_T, typename Mapped_T>
	const Mapped_T& Map<Key_T, Mapped_T>::at(const Key_T& k) const {
		return at_helper_const(k, root);
	}

	template<typename Key_T, typename Mapped_T>
	const Mapped_T& Map<Key_T, Mapped_T>::at_helper_const(const Key_T& k, Map<Key_T, Mapped_T>::AVLNode* node) const {
		if (node == NULL) {
			throw std::out_of_range("Not found the key.");
		} else {
			if (node->valObj.first == k) {
				return *(&node->valObj.second);
			} else if (k < node->valObj.first) {
				return at_helper_const(k, node->left);
			} else {
				return at_helper_const(k, node->right);
			}
		}
	}


	template<typename Key_T, typename Mapped_T>
	Mapped_T & Map<Key_T, Mapped_T>::operator[](const Key_T & k) {
		if (root == NULL) {
			root = new AVLNode(ValueType(k, Mapped_T()));
			end_node = new AVLNode(ValueType(k, Mapped_T()));
			end_node->height = -1;
			return *(&root->valObj.second);
		} else {
			return get_helper(k, root);
		}
	}

	template<typename Key_T, typename Mapped_T>
	Mapped_T & Map<Key_T, Mapped_T>::get_helper(const Key_T & k, AVLNode* node) {
		if (k == node->valObj.first) {
			return *(&node->valObj.second);
		} else if (k < node->valObj.first) {
			if (node->left == NULL) {
				AVLNode* n = new AVLNode(ValueType(k,Mapped_T()));
				node->left = n;
				n->parent = node;
				n->reference_num = node->reference_num;
				node->height = std::max(height(node->left), height(node->right))+1;
				return *(&n->valObj.second);
			} else {
				Mapped_T& v = get_helper(k, node->left);
				// adjust balance
				if (height(node->left)-height(node->right) == 2) {
					if (k < node->left->valObj.first) ll(node);
					else lr(node);
				}
				node->height = std::max(height(node->left), height(node->right))+1;
				return v;
			}
		} else {
			if (node->right == NULL) {
				AVLNode* n = new AVLNode(ValueType(k,Mapped_T()));
				node->right = n;
				n->parent = node;
				n->reference_num = node->reference_num;
				node->height = std::max(height(node->left), height(node->right))+1;
				return *(&n->valObj.second);
			} else {
				Mapped_T& v = get_helper(k, node->right);
				// adjust balance
				if (height(node->right)-height(node->left) == 2) {
					if (k < node->right->valObj.first) rl(node);
					else rr(node);
				}
				node->height = std::max(height(node->left), height(node->right))+1;
				return v;
			}
		}
	}

	template<typename Key_T, typename Mapped_T>
	int Map<Key_T, Mapped_T>::height( Map<Key_T, Mapped_T>::AVLNode* node){
		if (node == NULL) return -1;
		else return node->height;
	}

	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::AVLNode*
	Map<Key_T, Mapped_T>::insert_helper(AVLNode* cur, bool& insert_ok, const ValueType& v) {
		typename Map<Key_T, Mapped_T>::AVLNode* n;
		if (root == NULL) {
			n = new AVLNode(v);
			end_node = new AVLNode(v);
			end_node->height = -1;
			if (n == NULL) {
				insert_ok = false;
				return NULL;
			}
			root = n;
			insert_ok = true;
			return root;
		}
		if (v.first < cur->valObj.first) {
			// left
			if (cur->left == NULL) {
				n = new AVLNode(v);
				cur->left = n;
				n->parent = cur;
				insert_ok = true;
				n->reference_num = cur->reference_num;
			} else {
				n = insert_helper(cur->left, insert_ok, v);
				if (height(cur->left)- height(cur->right) == 2) {
					if (v.first < cur->left->valObj.first) ll(cur);
					else lr(cur);
				}
			}
			cur->height = std::max(height(cur->left), height(cur->right))+1;
			return n;
		} else if (cur->valObj.first == v.first) {
			// repeat
			insert_ok = false;
			return cur;
		} else  {
			// right
			if (cur->right == NULL) {
				n = new typename Map<Key_T, Mapped_T>::AVLNode(v);
				cur->right = n;
				n->parent = cur;
				insert_ok = true;
				n->reference_num = cur->reference_num;
			} else {
				n = insert_helper(cur->right, insert_ok, v);
				if (height(cur->right)-height(cur->left) == 2) {
					if (v.first < cur->right->valObj.first) rl(cur);
					else rr(cur);
				}
			}
			cur->height = std::max(height(cur->left), height(cur->right))+1;
			return n;
		}
	}

	template<typename Key_T, typename Mapped_T>
	std::pair<typename Map<Key_T, Mapped_T>::Iterator, bool>
	Map<Key_T, Mapped_T>::insert(const ValueType& v) {
		bool insert_ok = false;
		AVLNode* node = insert_helper(root, insert_ok, v);
		Iterator it(node, this);
		return std::make_pair(it, insert_ok);
	}

	template<typename Key_T, typename Mapped_T>
	template <typename IT_T>
	void Map<Key_T, Mapped_T>::insert(IT_T range_beg, IT_T range_end) {
		IT_T it = range_beg;
		for(; it != range_end; ++it) insert(*it);
	}

	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::erase(Iterator pos) {
		erase_helper(pos.an->valObj.first, root);
	}

	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::erase(const Key_T& k) {
		erase_helper(k, root);
	}
	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::erase_helper(const Key_T& k, AVLNode* n) {
		if (n == NULL) return;
		if (n->valObj.first == k) {
			AVLNode* p = n->parent;
			trash(n);
			n->height = std::max(height(n->left), height(n->right))+1;
			return;
		}
		if (k < n->valObj.first) return erase_helper(k, n->left);
		else return erase_helper(k, n->right);
	}

	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::trash(AVLNode* res) {
		if (res != NULL)  {
			if (res->left == NULL && res->right == NULL) {
				if (res->parent == NULL) {
					this->root = NULL;
				} else {
					if (res == res->parent->left) {
						res->parent->left = NULL;
					} else {
						res->parent->right = NULL;
					}
				}
				delete(res);
			} else if (res->left != NULL && res->right != NULL){
				AVLNode* replace = res->left;
				if (replace->right == NULL) {
					replace->right = res->right;
					res->right->parent = replace;
					if (res->parent == NULL) {
						replace->parent = NULL;
						this->root = replace;
						delete(res);
					} else {
						replace->parent = res->parent;
						if (res == res->parent->left) {
							res->parent->left = replace;
						} else {
							res->parent->right = replace;
						}
						delete(res);
					}
				} else {
					while (replace->right != NULL) replace = replace->right;
					if (replace->left != NULL) {
						replace->parent->right = replace->left;
						replace->left->parent = replace->parent;
					} else {
						replace->parent->right = NULL;
					}
					if (res->parent == NULL) {
						replace->parent = NULL;
						this->root = replace;
					} else {
						replace->parent = res->parent;
						if (res->parent->right == res) {
							res->parent->right = replace;
						} else {
							res->parent->left = replace;
						}
					}
					replace->left = res->left;
					res->left->parent = replace;
					replace->right = res->right;
					res->right->parent = replace;
					delete(res);
				}

			} else {
				if (res->left == NULL) {
					if (res->parent == NULL) {
						this->root = res->right;
						this->root->parent = NULL;
						delete(res);
					} else {
						res->right->parent = res->parent;
						if (res == res->parent->left) {
							res->parent->left = res->right;
						} else {
							res->parent->right = res->right;
						}
						delete(res);
					}
				} else if (res->right == NULL) {
					if (res->parent == NULL) {
						this->root = res->left;
						this->root->parent = NULL;
						delete(res);
					} else {
						res->left->parent = res->parent;
						if (res == res->parent->left) {
							res->parent->left = res->left;
						} else {
							res->parent->right = res->left;
						}
						delete(res);
					}
				}
			}
		}
	}

	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::clear() {
		if (root != NULL) clear_helper(root);
		if (this->end_node != NULL) {
			if(this->end_node->reference_num > 0) {
				-- this->end_node->reference_num;
			} else if (this->end_node->reference_num == 0) {
				delete(this->end_node);
			}
		}
	}
	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::clear_helper(AVLNode* node) {
		if (node == NULL) return;
		clear_helper(node->left);
		clear_helper(node->right);
		if (node->reference_num == 0 ) {
			delete(node);
		} else {
			-- node->reference_num;
		}
	}


	template<typename Key_T, typename Mapped_T>
	bool Map<Key_T, Mapped_T>::operator==(const Map<Key_T, Mapped_T>& m) {
		return equal(this->root, m.root);
	}

	template<typename Key_T, typename Mapped_T>
	bool Map<Key_T, Mapped_T>::operator!=(const Map<Key_T, Mapped_T>& m) {
		return !equal(this->root, m.root);
	}

	template<typename Key_T, typename Mapped_T>
	bool Map<Key_T, Mapped_T>::operator<(const Map<Key_T, Mapped_T>& m) {
		auto t1 = this->begin();
		auto t2 = m.begin();
		auto e1 = this->end();
		auto e2 = m.end();
		for (; t1 != e1 && t2 != e2; ) {
			if (t1.an->valObj.first == t2.an->valObj.first) {++t1; ++t2;}
			else if (t1.an->valObj.first < t2.an->valObj.first) {return true;}
			else return false;
		}
		if (t1 == e1) return true;
		else return false;
	}

	template<typename Key_T, typename Mapped_T>
	bool Map<Key_T, Mapped_T>::equal(AVLNode* t1, AVLNode* t2) {
		if (t1 == t2) return true;
		if (t1->valObj.first == t2->valObj.first && t1->valObj.second == t2->valObj.second) {
			if (equal(t1->left, t2->left) && equal(t1->right, t2->right)) return true;
			else return false;
		}
		return false;
	}

	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::printTree_mymap() {
		printHelper(root, 0);
	}

	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::printHelper(typename Map<Key_T, Mapped_T>::AVLNode* cur, int level) {
		if (cur == NULL) return;
		printHelper(cur->left, level+1);
		std::cout << std::setw(10*level) << cur->valObj.first << "," << cur->valObj.second << "#" << cur->height << std::endl;
		printHelper(cur->right, level+1);
	}

	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::ll(typename Map<Key_T, Mapped_T>::AVLNode* n) {
		AVLNode* t = n->left;
		AVLNode* p = n->parent;
		AVLNode* x = t->right;
		if (x == NULL) {n->left = NULL;}
		else {n->left = x; x->parent = n;}
		t->right = n;
		n->parent = t;
		if (p == NULL) {t->parent = NULL; root = t;}
		else {
			t->parent = p;
			if (n == p->right) p->right = t;
			else p->left = t;
		}
		n->height = std::max(height(n->left), height(n->right))+1;
		t->height = std::max(height(t->left), height(t->right))+1;
	}

	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::lr(typename Map<Key_T, Mapped_T>::AVLNode* n) {
		rr(n->left); ll(n);
	}

	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::rl(typename Map<Key_T, Mapped_T>::AVLNode* n) {
		ll(n->right); rr(n);
	}

	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::rr(typename Map<Key_T, Mapped_T>::AVLNode* n) {
		AVLNode* t = n->right;
		AVLNode* p = n->parent;
		AVLNode* x = t->left;
		if (x != NULL) {n->right = x; x->parent = n;}
		else {n->right = NULL;}
		n->parent = t;
		t->left = n;
		if (p != NULL) {
			t->parent = p;
			if (n == p->left) p->left = t;
			else p->right = t;
		}
		else {root = t; t->parent = NULL;}
		n->height = std::max(height(n->left), height(n->right))+1;
		t->height = std::max(height(t->left), height(t->right))+1;
	}

	/*
	Implementation of Iterator
	*/
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::Iterator::Iterator(AVLNode* an, Map<Key_T, Mapped_T>* map) {
		this->an = an;
		this->end_node = map->end_node;
		this->map = map;
	}
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::Iterator::Iterator(const Iterator& it) {
		this->an = it.an;
		this->end_node = it.end_node;
		this->map = it.map;
	}
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator&
	Map<Key_T, Mapped_T>::Iterator::operator++() {
		if (this->an == end_node) {
			std::cout << "cs540::Segmentation Fault. Cannot go to the next of last element.\n";
			return *this;
		} else if (this->an == NULL) {
			std::cout <<"cs540::Segmentation Fault. Cannot go to the next of NULL.\n";
			return *this;
		} else {
			if (this->an->right == NULL) {
				AVLNode* tmp = this->an;
				while (tmp->parent != NULL && tmp->parent->right == tmp) {
					tmp = tmp->parent;
				}
				if (tmp->parent == NULL) {
					this->an = this->end_node;
					return *this;
				} else {
					this->an = tmp->parent;
					return *this;
				}
			} else {
				this->an = this->an->right;
				while (this->an->left != NULL) this->an = this->an->left;
				return *this;
			}
		}
		return *this;
	}
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator&
	Map<Key_T, Mapped_T>::Iterator::operator--(){
		if (this->an == NULL) {
			std::cout << "cs540::Segmentation Fault. Cannot go to the previous of NULL.\n";
			return *this;
		} else if (this->an == end_node) {
			AVLNode* tmp = this->map->root;
			if (tmp == NULL) {
				std::cout << "cs540::Undefined Behavior. Cannot go to the previous of NULL root.\n";
				return *this;
			}
			while (tmp->right != NULL) tmp = tmp->right;
			this->an = tmp;
			return *this;
		} else {
			if (this->an->left == NULL) {
				AVLNode* tmp = this->an;
				if (tmp == tmp->parent->right) {
					tmp = tmp->parent;
					this->an = tmp;
					return *this;
				} else {
					while (tmp->parent != NULL && tmp == tmp->parent->left) {
						tmp = tmp->parent;
					}
					if (tmp->parent == NULL) {
						std::cout << "cs540::Undefined Behavior. Cannot go to the previous of begin.\n";
						return *this;
					} else {
						tmp = tmp->parent;
						this->an = tmp;
						return *this;
					}
				}
			} else {
				this->an = this->an->left;
				return *this;
			}
		}
	}

	/*
	Implementation of ConstIterator
	*/
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::ConstIterator::ConstIterator(AVLNode* an, const Map<Key_T, Mapped_T>* map) {
		this->an = an;
		this->map = const_cast<Map<Key_T, Mapped_T>*>(map);
		this->end_node = map->end_node;
	}
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::ConstIterator::ConstIterator(const Map<Key_T, Mapped_T>::Iterator& it) {
		this->an = it.an;
		this->map = it.map;
		this->end_node = it.end_node;
	}
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator&
	Map<Key_T, Mapped_T>::ConstIterator::operator++() {
		if (this->an == end_node) {
			std::cout << "cs540::Segmentation Fault. Cannot go to the next of last element.\n";
			return *this;
		} else if (this->an == NULL) {
			std::cout <<"cs540::Segmentation Fault. Cannot go to the next of NULL.\n";
			return *this;
		} else {
			if (this->an->right == NULL) {
				AVLNode* tmp = this->an;
				while (tmp->parent != NULL && tmp->parent->right == tmp) {
					tmp = tmp->parent;
				}
				if (tmp->parent == NULL) {
					// the last one
					this->an = this->end_node;
					return *this;
				} else {
					this->an = tmp->parent;
					return *this;
				}
			} else {
				this->an = this->an->right;
				while (this->an->left != NULL) this->an = this->an->left;
				return *this;
			}
		}
		return *this;
	}
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator&
	Map<Key_T, Mapped_T>::ConstIterator::operator--() {
		if (this->an == NULL) {
			std::cout << "cs540::Segmentation Fault. Cannot go to the previous of NULL.\n";
			return *this;
		} else if (this->an == end_node) {
			AVLNode* tmp = this->map->root;
			if (tmp == NULL) {
				std::cout << "cs540::Undefined Behavior. Cannot go to the previous of NULL root.\n";
				return *this;
			}
			while (tmp->right != NULL) tmp = tmp->right;
			this->an = tmp;
			return *this;
		} else {
			if (this->an->left == NULL) {
				AVLNode* tmp = this->an;
				if (tmp == tmp->parent->right) {
					tmp = tmp->parent;
					this->an = tmp;
					return *this;
				} else {
					while (tmp->parent != NULL && tmp == tmp->parent->left) {
						tmp = tmp->parent;
					}
					if (tmp->parent == NULL) {
						std::cout << "cs540::Undefined Behavior. Cannot go to the previous of begin.\n";
						return *this;
					} else {
						tmp = tmp->parent;
						this->an = tmp;
						return *this;
					}
				}
			} else {
				this->an = this->an->left;
				return *this;
			}
		}
	}
	/*
	Implementation of ReverseIterator
	*/
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::ReverseIterator::ReverseIterator(AVLNode* an, Map<Key_T, Mapped_T>* map) {
		this->an = an;
		this->map = map;
		this->end_node = map->end_node;
	}

	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::ReverseIterator::ReverseIterator
	(const Map<Key_T, Mapped_T>::Iterator& it) {
		this->an = it.an;
		this->map = it.map;
		this->end_node = it.end_node;
	}
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ReverseIterator&
	Map<Key_T, Mapped_T>::ReverseIterator::operator++(){
		if (this->an == NULL) {
			std::cout << "cs540::Segmentation Fault. Cannot go to the previous of NULL.\n";
			return *this;
		} else if (this->an == end_node) {
			std::cout << "cs540::Segmentation Fault. Cannot go to the next of end.\n";
			return *this;
		} else {
			if (this->an->left == NULL) {
				AVLNode* tmp = this->an;
				while (tmp->parent != NULL && tmp == tmp->parent->left) tmp = tmp->parent;
				if (tmp->parent == NULL) {
						this->an = this->end_node;
						return *this;
				} else {
					this->an = tmp->parent;
					return *this;
				}
			} else {
				AVLNode* tmp = this->an;
				tmp = tmp->left;
				while (tmp->right != NULL) tmp = tmp->right;
				this->an = tmp;
				return *this;
			}
		}
	}
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ReverseIterator&
	Map<Key_T, Mapped_T>::ReverseIterator::operator--(){
		if (an == NULL) {
			std::cout << "ReverseIterator Cannot go to next of NULL.\n";
			return *this;
		}
		if (an == end_node) {
			AVLNode* tmp = map->root;
			if (tmp == NULL) {std::cout << "ReverseIterator Cannot go to next of NULL.\n"; return *this;}
			while (tmp->left != NULL) tmp = tmp->left;
			an = tmp;
			return *this;
		}
		AVLNode* tmp = an;
		if (tmp->right == NULL) {
			while (tmp->parent != NULL && tmp->parent->right == tmp) tmp = tmp->parent;
			if (tmp->parent == NULL) an = end_node;
			else an = tmp->parent;
		} else {
			tmp = tmp->right;
			while (tmp->left != NULL) tmp = tmp->left;
			an = tmp;
		}
		return *this;
	}
}
