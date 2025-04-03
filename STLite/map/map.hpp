/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

enum class Color {red, black};

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type;

	struct Node {
		value_type* val = nullptr;
		Color color = Color::red;
		Node* father = nullptr;
		Node* left_son = nullptr;
		Node* right_son = nullptr;

		Node() = default;

		Node(Key key, T value) {
			val = new value_type{key, value};
		}

		Node(Key key, T value, Color color) {
			val = new value_type{key, value};
			this -> color = color;
		}

		~Node() {
			delete val;
		}
	};
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */

	class const_iterator;
	class iterator {
	public:
		Node* cur = nullptr;
		map* obj;

		iterator() = default;

		explicit iterator(Node* cur, const map* obj) {
			this -> cur = cur;
			this -> obj = const_cast<map*>(obj);
		}

		iterator(const iterator &other) {
			cur = other.cur;
			obj = other.obj;
		}

		iterator operator++(int) {
			if (cur == nullptr) {
				throw invalid_iterator();
			}
			if (cur == obj -> end_node) {
				throw invalid_iterator();
			}
			iterator ret = *this;
			if (cur -> right_son == nullptr) {
				do {
					if (cur -> father == nullptr) {
						cur = obj -> end_node;
						break;
					}
					if (cur -> father -> left_son == cur) {
						cur = cur -> father;
						break;
					}
					cur = cur -> father;
				} while (true);
			}
			else {
				cur = cur -> right_son;
				while (cur -> left_son != nullptr) {
					cur = cur -> left_son;
				}
			}
			return ret;
		}

		iterator & operator++() {
			if (cur == nullptr) {
				throw invalid_iterator();
			}
			if (cur == obj -> end_node) {
				throw invalid_iterator();
			}
			iterator ret = *this;
			if (cur -> right_son == nullptr) {
				do {
					if (cur -> father == nullptr) {
						cur = obj -> end_node;
						break;
					}
					if (cur -> father -> left_son == cur) {
						cur =  cur -> father;
						break;
					}
					cur = cur -> father;
				} while (true);
			}
			else {
				cur = cur -> right_son;
				while (cur -> left_son != nullptr) {
					cur = cur -> left_son;
				}
			}
			return *this;
		}

		iterator operator--(int) {
			if (cur == nullptr) {
				throw invalid_iterator();
			}
			iterator ret = *this;
			if (cur == obj -> end_node) {
				cur = obj -> rightmost;
				return ret;
			}
			if (cur -> left_son == nullptr) {
				do {
					if (cur -> father == nullptr) {
						*this = ret;
						throw invalid_iterator();
					}
					if (cur -> father -> right_son == cur) {
						cur = cur -> father;
						break;
					}
					cur = cur -> father;
				} while(true);
			}
			else {
				cur = cur -> left_son;
				while (cur -> right_son != nullptr) {
					cur = cur -> right_son;
				}
			}
			return ret;
		}

		iterator & operator--() {
			iterator ret = *this;
			if (cur == nullptr) {
				throw invalid_iterator();
			}
			if (cur == obj -> end_node) {
				cur = obj -> rightmost;
				return *this;
			}
			if (cur -> left_son == nullptr) {
				do {
					if (cur -> father == nullptr) {
						*this = ret;
						throw invalid_iterator();
					}
					if (cur -> father -> right_son == cur) {
						cur = cur -> father;
						break;
					}
					cur = cur -> father;
				} while(true);
			}
			else {
				cur = cur -> left_son;
				while (cur -> right_son != nullptr) {
					cur = cur -> right_son;
				}
			}
			return *this;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {
			return *cur -> val;
		}
		bool operator==(const iterator &rhs) const {
			return cur == rhs.cur;
		}
		bool operator==(const const_iterator &rhs) const {
			return cur == rhs.cur;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return cur != rhs.cur;
		}
		bool operator!=(const const_iterator &rhs) const {
			return cur != rhs.cur;
		}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {
			return cur -> val;
		}
	};
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		private:
		public:
		    const Node* cur = nullptr;
		    const map* obj = nullptr;

			const_iterator() = default;

			const_iterator(const const_iterator &other) {
				cur = other.cur;
				obj = other.obj;
			}
			const_iterator(const iterator &other) {
				cur = other.cur;
				obj = other.obj;
			}

			const_iterator& operator= (const iterator& other) {
				cur = other.cur;
				obj = other.obj;
				return *this;
			}

			const_iterator operator++(int) {
				if (cur == nullptr) {
					throw invalid_iterator();
				}
				if (cur == obj -> end_node) {
					throw invalid_iterator();
				}
				const_iterator ret = *this;
				if (cur -> right_son == nullptr) {
					do {
						if (cur -> father == nullptr) {
							cur = obj -> end_node;
							break;
						}
						if (cur -> father -> left_son == cur) {
							cur = cur -> father;
							break;
						}
						cur = cur -> father;
					} while (true);
				}
				else {
					cur = cur -> right_son;
					while (cur -> left_son != nullptr) {
						cur = cur -> left_son;
					}
				}
				return ret;
			}

			const_iterator & operator++() {
				if (cur == nullptr) {
					throw invalid_iterator();
				}
				if (cur == obj -> end_node) {
					throw invalid_iterator();
				}
				const_iterator ret = *this;
				if (cur -> right_son == nullptr) {
					do {
						if (cur -> father == nullptr) {
							cur = obj -> end_node;
							break;
						}
						if (cur -> father -> left_son == cur) {
							cur =  cur -> father;
							break;
						}
						cur = cur -> father;
					} while (true);
				}
				else {
					cur = cur -> right_son;
					while (cur -> left_son != nullptr) {
						cur = cur -> left_son;
					}
				}
				return *this;
			}

			const_iterator operator--(int) {
				if (cur == nullptr) {
					throw invalid_iterator();
				}
				const_iterator ret = *this;
				if (cur == obj -> end_node) {
					cur = obj -> rightmost;
					return ret;
				}
				if (cur -> left_son == nullptr) {
					do {
						if (cur -> father == nullptr) {
							*this = ret;
							throw invalid_iterator();
						}
						if (cur -> father -> right_son == cur) {
							cur = cur -> father;
							break;
						}
						cur = cur -> father;
					} while(true);
				}
				else {
					cur = cur -> left_son;
					while (cur -> right_son != nullptr) {
						cur = cur -> right_son;
					}
				}
				return ret;
			}

			const_iterator & operator--() {
				if (cur == nullptr) {
					throw invalid_iterator();
				}
				const_iterator ret = *this;
				if (cur == obj -> end_node) {
					cur = obj -> rightmost;
					return *this;
				}
				if (cur -> left_son == nullptr) {
					do {
						if (cur -> father == nullptr) {
							*this = ret;
							throw invalid_iterator();
						}
						if (cur -> father -> right_son == cur) {
							cur = cur -> father;
							break;
						}
						cur = cur -> father;
					} while(true);
				}
				else {
					cur = cur -> left_son;
					while (cur -> right_son != nullptr) {
						cur = cur -> right_son;
					}
				}
				return *this;
			}
			/**
			 * a operator to check whether two iterators are same (pointing to the same memory).
			 */
			value_type & operator*() const {
				return *cur -> val;
			}
			bool operator==(const iterator &rhs) const {
				return cur == rhs.cur;
			}
			bool operator==(const const_iterator &rhs) const {
				return cur == rhs.cur;
			}
			/**
			 * some other operator for iterator.
			 */
			bool operator!=(const iterator &rhs) const {
				return cur != rhs.cur;
			}
			bool operator!=(const const_iterator &rhs) const {
				return cur != rhs.cur;
			}

			/**
			 * for the support of it->first.
			 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
			 */
			const value_type* operator->() const noexcept {
				return cur -> val;
			}

	};

	Node* begin_node = nullptr;
	Node* rightmost = nullptr;
	Node* end_node = new Node;
	Node* root = nullptr;
	Compare cmp;
	size_t sze = 0;

	map() = default;

	map(const map &other) {
		build(nullptr, other.root, other.begin_node, other.rightmost);
		sze = other.sze;
	}

	map & operator=(const map &other) {
		if (this == &other) {
			return *this;
		}
		destruct(root);
		build(nullptr, other.root, other.begin_node, other.rightmost);
		sze = other.sze;
		return *this;
	}

	~map() {
		destruct(root);
		delete end_node;
	}
	/**
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {
		Node* p = root;
		while (p != nullptr) {
			if (!cmp(p -> val -> first, key) && !cmp(key, p -> val -> first)) {
				return p -> val -> second;
			}
			if (cmp(p -> val -> first, key)) {
				p = p -> right_son;
			}
			else {
				p = p -> left_son;
			}
		}
		throw index_out_of_bound();
	}
	const T & at(const Key &key) const {
		Node* p = root;
		while (p != nullptr) {
			if (!cmp(p -> val -> first, key) && !cmp(key, p -> val -> first)) {
				return p -> val -> second;
			}
			if (cmp(p -> val -> first, key)) {
				p = p -> right_son;
			}
			else {
				p = p -> left_son;
			}
		}
		throw index_out_of_bound();
	}
	/**
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {
		Node* p = root;
		Node* q = nullptr;
		while (p != nullptr) {
			if (!cmp(p -> val -> first, key) && !cmp(key, p -> val -> first)) {
				return p -> val -> second;
			}
			if (cmp(p -> val -> first, key)) {
				q = p;
				p = p -> right_son;
			}
			else {
				q = p;
				p = p -> left_son;
			}
		}

		sze++;
		T value_;
		Node* new_node = new Node{key, value_};
		if (sze == 1) {
			root = begin_node = rightmost = new_node;
			return new_node -> val -> second;
		}
		if (cmp(q -> val -> first, key)) {
			q -> right_son = new_node;
			new_node -> father = q;
			if (q == rightmost) {
				rightmost = new_node;
			}
		}
		else {
			q -> left_son = new_node;
			new_node -> father = q;
			if (q == begin_node) {
				begin_node = new_node;
			}
		}

		Node* son_node = new_node;
		while (q != nullptr && q -> color == Color::red) {
			if (q == root) {
				q -> color = Color::black;
				break;
			}
			Node* uncle_node;
			if (q -> father -> left_son == q) {
				uncle_node = q -> father -> right_son;
			}
			else {
				uncle_node = q -> father -> left_son;
			}
			if (uncle_node != nullptr && uncle_node -> color == Color::red) {
				q -> color = Color::black;
				uncle_node -> color = Color::black;
				q -> father -> color = Color::red;
				son_node = q -> father;
				q = q -> father -> father;
			}
			else {
				Node* grandfather = q -> father;
				if (grandfather -> left_son == q && q -> left_son == son_node) {
					turnRight(q);
					grandfather -> color = Color::red;
					q -> color = Color::black;

				}
				else if (grandfather -> right_son == q && q -> right_son == son_node) {
					turnLeft(q);
					grandfather -> color = Color::red;
					q -> color = Color::black;
				}
				else if (grandfather -> left_son == q && q -> right_son == son_node) {
					turnLeft(son_node);
					turnRight(son_node);
					son_node -> color = Color::black;
					grandfather -> color = Color::red;
				}
				else {
					turnRight(son_node);
					turnLeft(son_node);
					son_node -> color = Color::black;
					grandfather -> color = Color::red;
				}
			}
		}
		return new_node -> val -> second;
	}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
		Node* p = root;
		while (p != nullptr) {
			if (!cmp(p -> val -> first, key) && !cmp(key, p -> val -> first)) {
				return p -> val -> second;
			}
			if (cmp(p -> val -> first, key)) {
				p = p -> right_son;
			}
			else {
				p = p -> left_son;
			}
		}
		throw index_out_of_bound();
	}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {
		iterator ret {begin_node, this};
		return ret;
	}
	const_iterator cbegin() const {
		iterator tmp {begin_node, this};
		const_iterator ret {tmp};
		return ret;
	}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {
		iterator ret {end_node, this};
		return ret;
	}
	const_iterator cend() const {
		iterator tmp {end_node, this};
		const_iterator ret {tmp};
		return ret;
	}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {
		return sze == 0;
	}
	/**
	 * returns the number of elements.
	 */
	[[nodiscard]] size_t size() const {
		return sze;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		destruct(root);
		root = rightmost = nullptr;
		begin_node = end_node;
		sze = 0;
	}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value) {
		Node* p = root;
		Node* q = nullptr;
		Key key = value.first;
		while (p != nullptr) {
			if (!cmp(p -> val -> first, key) && !cmp(key, p -> val -> first)) {
				return pair<iterator, bool> {iterator{p, this}, false};
			}
			if (cmp(p -> val -> first, key)) {
				q = p;
				p = p -> right_son;
			}
			else {
				q = p;
				p = p -> left_son;
			}
		}

		sze++;
		Node* new_node = new Node{value.first, value.second};
		if (sze == 1) {
			root = begin_node = rightmost = new_node;
			return pair<iterator, bool> {iterator{new_node, this}, true};
		}

		if (cmp(q -> val -> first, key)) {
			q -> right_son = new_node;
			new_node -> father = q;
			if (q == rightmost) {
				rightmost = new_node;
			}
		}
		else {
			q -> left_son = new_node;
			new_node -> father = q;
			if (q == begin_node) {
				begin_node = new_node;
			}
		}

		Node* son_node = new_node;
		while (q != nullptr && q -> color == Color::red) {
			if (q == root) {
				q -> color = Color::black;
				break;
			}
			Node* uncle_node;
			if (q -> father -> left_son == q) {
				uncle_node = q -> father -> right_son;
			}
			else {
				uncle_node = q -> father -> left_son;
			}
			if (uncle_node != nullptr && uncle_node -> color == Color::red) {
				q -> color = Color::black;
				uncle_node -> color = Color::black;
				q -> father -> color = Color::red;
				son_node = q -> father;
				q = q -> father -> father;
			}
			else {
				Node* grandfather = q -> father;
				if (grandfather -> left_son == q && q -> left_son == son_node) {
					turnRight(q);
					grandfather -> color = Color::red;
					q -> color = Color::black;

				}
				else if (grandfather -> right_son == q && q -> right_son == son_node) {
					turnLeft(q);
					grandfather -> color = Color::red;
					q -> color = Color::black;
				}
				else if (grandfather -> left_son == q && q -> right_son == son_node) {
					turnLeft(son_node);
					turnRight(son_node);
					son_node -> color = Color::black;
					grandfather -> color = Color::red;
				}
				else {
					turnRight(son_node);
					turnLeft(son_node);
					son_node -> color = Color::black;
					grandfather -> color = Color::red;
				}
			}
		}
		return pair<iterator, bool>{iterator{new_node, this}, true};
	}
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) {
		Node* cur_node = pos.cur;
		if (cur_node == nullptr || pos == this -> end() || pos.obj != this) {
			throw index_out_of_bound();
		}

		--sze;
		if (cur_node == begin_node) {
			if (sze > 0) {
				iterator tmp = pos;
				++tmp;
				begin_node = tmp.cur;
			}
			else {
				begin_node = end_node;
			}
		}
		if (cur_node == rightmost) {
			if (sze > 0) {
				iterator tmp = pos;
				--tmp;
				rightmost = tmp.cur;
			}
			else {
				rightmost = nullptr;
			}
		}
		if (cur_node -> left_son != nullptr && cur_node -> right_son != nullptr) {
			Node* p = cur_node -> left_son;
			while (p -> right_son != nullptr) {
				p = p -> right_son;
			}
			if (cur_node == root) {
				root = p;
			}
			Node* tmp = p -> father;
			Node* reg_1 = p -> left_son;
			p -> father = cur_node -> father;
			if (p -> father != nullptr) {
				if (cur_node == p -> father -> left_son) {
					p -> father -> left_son = p;
				}
				else {
					p -> father -> right_son = p;
				}
			}
			p -> right_son = cur_node -> right_son;
			if (p -> right_son != nullptr) {
				p -> right_son -> father = p;
			}
			if (p == cur_node -> left_son) {
				p -> left_son = cur_node;
				cur_node -> father = p;
			}
			else {
				p -> left_son = cur_node -> left_son;
				if (cur_node -> left_son != nullptr) {
					cur_node -> left_son -> father = p;
				}
				cur_node -> father = tmp;
				tmp -> right_son = cur_node;
			}
			std::swap(p -> color, cur_node -> color);
			cur_node -> left_son = reg_1;
			cur_node -> right_son = nullptr;
		}

		if (cur_node -> left_son != nullptr) {
			if (cur_node -> father != nullptr) {
				if (cur_node -> father -> left_son == cur_node ) {
					cur_node -> father -> left_son = cur_node  -> left_son;
					cur_node -> left_son -> color = Color::black;
					cur_node -> left_son -> father = cur_node  -> father;
				}
				else {
					cur_node  -> father -> right_son = cur_node  -> left_son;
					cur_node  -> left_son -> color = Color::black;
					cur_node  -> left_son -> father = cur_node  -> father;
				}
			}
			else {
				root = cur_node -> left_son;
				cur_node -> left_son -> color = Color::black;
				cur_node -> left_son -> father = cur_node  -> father;
			}
			delete cur_node;
			return;
		}
		if (cur_node -> right_son != nullptr) {
			if (cur_node -> father != nullptr) {
				if (cur_node -> father -> left_son == cur_node) {
					cur_node -> father -> left_son = cur_node -> right_son;
					cur_node -> right_son -> color = Color::black;
					cur_node -> right_son -> father = cur_node -> father;
				}
				else {
					cur_node -> father -> right_son = cur_node -> right_son;
					cur_node -> right_son -> color = Color::black;
					cur_node -> right_son -> father = cur_node -> father;
				}
			}
			else {
				root = cur_node -> right_son;
				cur_node -> right_son -> color = Color::black;
				cur_node -> right_son -> father = cur_node -> father;
			}
			delete cur_node;
			return;
		}

		if (cur_node == root) {
			delete cur_node;
			root = nullptr;
			return;
		}
		if (cur_node -> color == Color::red) {
			if (cur_node == cur_node -> father -> left_son) {
				cur_node -> father -> left_son = nullptr;
			}
			else {
				cur_node -> father -> right_son = nullptr;
			}
			delete cur_node;
			return;
		}

		Node* sibling;
		if (cur_node == cur_node -> father -> left_son) {
			sibling = cur_node -> father -> right_son;
			delete cur_node;
			sibling -> father -> left_son = nullptr;
		}
		else {
			sibling = cur_node -> father -> left_son;
			delete cur_node;
			sibling -> father -> right_son = nullptr;
		}

		while (true) {
			if (sibling -> color == Color::red) {
				sibling -> color = Color::black;
				sibling -> father -> color = (sibling -> father -> color == Color::red ? Color::black : Color::red);
				if (sibling == sibling -> father -> right_son) {
					Node* new_sibling = sibling -> left_son;
					turnLeft(sibling);
					sibling = new_sibling;
				}
				else {
					Node* new_sibling = sibling -> right_son;
					turnRight(sibling);
					sibling = new_sibling;
				}
			}
			else {
				if ((sibling -> left_son == nullptr || sibling -> left_son -> color == Color::black) &&
					(sibling -> right_son == nullptr || sibling -> right_son -> color == Color::black)) {
					sibling -> color = Color::red;
					if (sibling -> father -> color == Color::red || sibling -> father == root) {
						sibling -> father -> color = Color::black;
						break;
					}
					if (sibling -> father -> father -> left_son == sibling -> father) {
						sibling = sibling -> father -> father -> right_son;
					}
					else {
						sibling = sibling -> father -> father -> left_son;
					}
					continue;
				}
				if (sibling -> father -> right_son == sibling) {
					if (sibling -> right_son != nullptr && sibling -> right_son -> color == Color::red) {
						sibling -> right_son -> color = sibling -> color;
						sibling -> color = sibling -> father -> color;
						sibling -> father -> color = Color::black;
						turnLeft(sibling);
					}
					else {
						sibling -> left_son -> color = sibling -> father -> color;
						sibling -> father -> color = Color::black;
						Node* center = sibling -> left_son;
						turnRight(center);
						turnLeft(center);
					}
				}
				else {
					if (sibling -> left_son != nullptr && sibling -> left_son -> color == Color::red) {
						sibling -> left_son -> color = sibling -> color;
						sibling -> color = sibling -> father -> color;
						sibling -> father -> color = Color::black;
						turnRight(sibling);
					}
					else {
						sibling -> right_son -> color = sibling -> father -> color;
						sibling -> father -> color = Color::black;
						Node* center = sibling -> right_son;
						turnLeft(center);
						turnRight(center);
					}
				}
				break;
			}
		}
	}
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const {
		Node* p = root;
		while (p != nullptr) {
			if (!cmp(p -> val -> first, key) && !cmp(key, p -> val -> first)) {
				return 1;
			}
			if (cmp(p -> val -> first, key)) {
				p = p -> right_son;
			}
			else {
				p = p -> left_son;
			}
		}
		return 0;
	}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {
		Node* p = root;
		while (p != nullptr) {
			if (!cmp(p -> val -> first, key) && !cmp(key, p -> val -> first)) {
				return iterator{p, this};
			}
			if (cmp(p -> val -> first, key)) {
				p = p -> right_son;
			}
			else {
				p = p -> left_son;
			}
		}
		return end();
	}
	const_iterator find(const Key &key) const {
		Node* p = root;
		while (p != nullptr) {
			if (!cmp(p -> val -> first, key) && !cmp(key, p -> val -> first)) {
				iterator tmp{p, this};
				return const_iterator{tmp};
			}
			if (cmp(p -> val -> first, key)) {
				p = p -> right_son;
			}
			else {
				p = p -> left_son;
			}
		}
		return cend();
	}

	void build(Node* pre, Node* cur, Node* begin_node, Node* rightmost, bool flag = false) {
		if (cur == nullptr) {
			return;
		}
		Node* tmp = new Node{cur -> val -> first, cur -> val -> second, cur -> color};
		if (cur == begin_node) {
			this -> begin_node = tmp;
		}
		if (cur == rightmost) {
			this -> rightmost = tmp;
		}
		tmp -> father = pre;
		if (pre != nullptr) {
			if (flag) {
				pre -> left_son = tmp;
			}
			else {
				pre -> right_son = tmp;
			}
		}
		else {
			root = tmp;
		}
		build(tmp, cur -> left_son, begin_node, rightmost, true);
		build(tmp, cur -> right_son, begin_node, rightmost, false);
	}

	void destruct(Node* cur) {
		if (cur == nullptr) {
			return;
		}
		destruct(cur -> left_son);
		destruct(cur -> right_son);
		delete cur;
	}

	void turnLeft(Node* cur) {
		Node* cur_father = cur -> father;
		cur -> father = cur_father -> father;
		if (cur -> father != nullptr) {
			if (cur_father == cur -> father -> left_son) {
				cur -> father -> left_son = cur;
			}
			else {
				cur -> father -> right_son = cur;
			}
		}
		else {
			root = cur;
		}
		cur_father -> right_son = cur -> left_son;
		if (cur_father -> right_son != nullptr) {
			cur_father -> right_son -> father = cur_father;
		}
		cur_father -> father = cur;
		cur -> left_son = cur_father;
	}

	void turnRight(Node* cur) {
		Node* cur_father = cur -> father;
		cur -> father = cur_father -> father;
		if (cur -> father != nullptr) {
			if (cur_father == cur -> father -> left_son) {
				cur -> father -> left_son = cur;
			}
			else {
				cur -> father -> right_son = cur;
			}
		}
		else {
			root = cur;
		}
		cur_father -> left_son = cur -> right_son;
		if (cur_father -> left_son != nullptr) {
			cur_father -> left_son -> father = cur_father;
		}
		cur_father -> father = cur;
		cur -> right_son = cur_father;
	}
};

}

#endif
