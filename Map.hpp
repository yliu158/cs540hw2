#ifndef CS540_MAP_HPP
#define CS540_MAP_HPP

#include <iostream>
#include <iomanip>
#include <queue>


namespace cs540 {

	template<typename Key_T, typename Mapped_T>
	class Map{

	public:

		typedef std::pair<Key_T, Mapped_T> ValueType;

		class AVLNode{
		public:
			ValueType valObj;
			AVLNode* left = NULL;
			AVLNode* right = NULL;
			AVLNode* parent = NULL;
			int reference_num = 0;
			int height = 0;
			AVLNode(const ValueType& vo): valObj(vo){}
			AVLNode()= delete;
		};

		class Iterator;
		class ConstIterator;
		class ReverseIterator;

		AVLNode* root = NULL;
		AVLNode* end_node = NULL;
		size_t size_map = 0;

		Map(){}// default constructor
		Map(const Map&); // copy constructor
		Map & operator=(const Map &); // copy assignment operator
		Map(std::initializer_list<std::pair<const Key_T, Mapped_T>>); // initializer_list constructor
		~Map();

		size_t size() const;
		bool empty() const;

		Iterator begin();
		Iterator end();
		ConstIterator begin() const;
		ConstIterator end() const;
		ReverseIterator rbegin();
		ReverseIterator rend();
		Iterator find(const Key_T &);
		ConstIterator find(const Key_T &) const;
		Mapped_T & at(const Key_T &);
		const Mapped_T & at(const Key_T &) const;
		Mapped_T & operator[](const Key_T &);
		std::pair<Iterator, bool> insert(const ValueType&);
		template <typename IT_T>
		void insert(IT_T range_beg, IT_T range_end);
		void erase(Iterator pos);
		void erase(const Key_T &);
		void clear();

		bool operator==(const Map&);
		bool operator!=(const Map&);
		bool operator<(const Map&);

		void printTree_mymap();
		void printHelper(AVLNode*, int);

	private:
		int height(AVLNode*);
		AVLNode* deep_copy(AVLNode*);
		void copy_assign_helper(AVLNode*);
		AVLNode* insert_helper(AVLNode*, bool&, const ValueType&);
		Mapped_T & at_helper(const Key_T &, AVLNode*);
		const Mapped_T & at_helper_const(const Key_T &, AVLNode*) const;
		Mapped_T & get_helper(const Key_T &, AVLNode*);
		AVLNode* find_helper(const Key_T&, AVLNode*) const;
		void trash(AVLNode*);
		void erase_helper(const Key_T&, AVLNode*);
		bool equal(AVLNode* t1, AVLNode* t2);
		void clear_helper(AVLNode*);
		void ll(AVLNode*);
		void lr(AVLNode*);
		void rl(AVLNode*);
		void rr(AVLNode*);
	};

	template<typename Key_T, typename Mapped_T>
	class Map<Key_T, Mapped_T>::Iterator {
	public:
		AVLNode* an = NULL;
		AVLNode* end_node = NULL;
		Map<Key_T, Mapped_T>* map = NULL;
		Iterator() = delete;
		~Iterator(){}
		Iterator(AVLNode*, Map<Key_T, Mapped_T>*);
		Iterator(const Iterator&);
		Iterator& operator++();
		Iterator operator++(int){Iterator retv = Iterator(this->an, this->map);operator++();return retv;}
		Iterator& operator--();
		Iterator operator--(int) {Iterator retv = Iterator(this->an, this->map);operator--();return retv;}
		ValueType & operator*() const{return *(&this->an->valObj);}
		ValueType * operator->() const{return &this->an->valObj;}
		bool operator==(const Iterator & it) {return an == it.an;}
		bool operator!=(const Iterator & it) {return an != it.an;}
		bool operator==(const ConstIterator & it) {return an == it.an;}
		bool operator!=(const ConstIterator & it) {return an != it.an;}
	};

	template<typename Key_T, typename Mapped_T>
	class Map<Key_T, Mapped_T>::ConstIterator {
	public:
		AVLNode* an = NULL;
		AVLNode* end_node = NULL;
		Map<Key_T, Mapped_T>* map = NULL;
		ConstIterator() = delete;
		ConstIterator(AVLNode*, const Map<Key_T, Mapped_T>*);
		ConstIterator(const Iterator&);
		~ConstIterator(){}
		ConstIterator& operator++();
		ConstIterator operator++(int){ConstIterator retv = ConstIterator(this->an, this->map);operator++();return retv;}
		ConstIterator& operator--();
		ConstIterator operator--(int){ConstIterator retv = ConstIterator(this->an, this->map);operator--();return retv;}
		const ValueType & operator*() const{return *(&this->an->valObj);}
		const ValueType * operator->() const{return &this->an->valObj;}
		bool operator==(const ConstIterator & it) {return an == it.an;}
		bool operator!=(const ConstIterator & it) {return an != it.an;}
		bool operator==(const Iterator & it) {return an == it.an;}
		bool operator!=(const Iterator & it) {return an != it.an;}
	};

	template<typename Key_T, typename Mapped_T>
	class Map<Key_T, Mapped_T>::ReverseIterator {
	public:
		AVLNode* an = NULL;
		AVLNode* end_node = NULL;
		Map<Key_T, Mapped_T>* map = NULL;
		ReverseIterator() = delete;
		ReverseIterator(AVLNode*, Map<Key_T, Mapped_T>*);
		ReverseIterator(const Iterator&);
		~ReverseIterator(){}
		ReverseIterator& operator++();
		ReverseIterator operator++(int){ReverseIterator retv = ReverseIterator(this->an, this->map);operator++();return retv;}
		ReverseIterator& operator--();
		ReverseIterator operator--(int){ReverseIterator retv = ReverseIterator(this->an, this->map);operator--();return retv;}
		ValueType & operator*() const {return *(&this->an->valObj);}
		ValueType * operator->() const {return &this->an->valObj;}
		bool operator== (const ReverseIterator& it) {return an == it.an;}
		bool operator!= (const ReverseIterator& it) {return an != it.an;}
	};

}/*cs540*/

#include "Map.cpp"

#endif
