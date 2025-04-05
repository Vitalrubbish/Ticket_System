#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

template<typename T, class Compare = std::less<T>>
struct Node {
	T* val = nullptr;
	Node* child = nullptr;
	Node* sibling = nullptr;
	Node* original_node = nullptr;

	explicit Node(const T &val) {
		this -> val = new T{val};
	}

	Node(const Node &other) {
		this -> val = new T{*other.val};
		this -> child = other.child;
		this -> sibling = other.sibling;
	}

	~Node() {
		delete val;
	}
};

/**
 * a container like std::priority_queue which is a heap internal.
 */

template<typename T, class Compare = std::less<T>>
class priority_queue {

	Node<T>* head = nullptr;
	std::size_t sze = 0;
	Compare cmp;

public:
	/**
	 * TODO constructors
	 */
	priority_queue() = default;

	priority_queue(const priority_queue &other) {
		build(nullptr, other.head, true);
		sze = other.sze;
	}
	/**
	 * TODO deconstructor
	 */
	~priority_queue() {
		destruct(head);
	}
	/**
	 * TODO Assignment operator
	 */
	priority_queue &operator=(const priority_queue &other) {
		if (this == &other) {
			return *this;
		}

		destruct(head);
		build(nullptr, other.head, true);
		sze = other.sze;
		return *this;
	}
	/**
	 * get the top of the queue.
	 * @return a reference of the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	const T & top() const {
		if (empty()) {
			throw container_is_empty();
		}
		return *head -> val;
	}
	/**
	 * TODO
	 * push new element to the priority queue.
	 */
	void push(const T &e) {
		Node<T>* tmp = new Node<T>{e};
		try {
			if (empty()) {
				head = tmp;
			}
			else if (cmp(*head -> val, e)) {
				tmp -> child = head;
				head = tmp;
			}
			else {
				tmp -> sibling = head -> child;
				head -> child = tmp;
			}
			++sze;
		} catch (sjtu::runtime_error) {
			delete tmp;
			throw;
		}
	}

	void pop() {
		if (empty()) {
			throw container_is_empty();
		}

		Node<T>* p_head = new Node<T> {*head -> val};
		p_head -> child = photo(nullptr, head -> child);

		Node<T>* tmp = head -> child;
		delete head;
		try {
			head = popProcess(tmp);
			photo_destruct(p_head -> child, false);
			delete p_head;
			--sze;
		} catch (sjtu::runtime_error) {
			photo_destruct(p_head -> child, true);
			head = p_head;
			throw;
		}
	}
	/**
	 * return the number of the elements.
	 */
	size_t size() const {
          return sze;
	}
	/**
	 * check if the container has at least an element.
	 * @return true if it is empty, false if it has at least an element.
	 */
	bool empty() const {
          return sze == 0;
	}
	/**
	 * merge two priority_queues with at most O(logn) complexity.
	 * clear the other priority_queue.
	 */
	void merge(priority_queue &other) {
		if (other.sze == 0) {
			return;
		}
		head = combine(head, other.head);
		sze += other.sze;
		other.sze = 0;
		other.head = nullptr;
	}

	Node<T>* photo(Node<T>* pre, Node<T>* cur) {
		if (cur == nullptr) {
			return cur;
		}
		Node<T>* tmp = nullptr;
		tmp = new Node<T>{*cur -> val};
		if (pre != nullptr) {
			pre -> sibling = tmp;
		}
		tmp -> child = cur -> child;
		tmp -> original_node = cur;
		if (cur -> sibling != nullptr) {
			photo(tmp, cur -> sibling);
		}
		return tmp;
	}

	void photo_destruct(Node<T>* cur, bool tag) {
		if (cur == nullptr) {
			return;
		}
		photo_destruct(cur -> sibling, tag);
		if (tag) {
			delete cur -> original_node;
		}
		else {
			delete cur;
		}
	}

    void build(Node<T>* pre, Node<T>* cur, bool is_child) {
		Node<T>* tmp = nullptr;
		tmp = new Node<T>{*cur -> val};
		if (pre != nullptr) {
			if (is_child) {
				pre -> child = tmp;
			}
			else {
				pre -> sibling = tmp;
			}
		}
		else {
			head = tmp;
		}

		if (cur -> child != nullptr) {
			build(tmp, cur -> child, true);
		}
		if (cur -> sibling != nullptr) {
			build(tmp, cur -> sibling, false);
		}
    }

	void destruct(Node<T>* cur) {
		if (cur == nullptr) {
			return;
		}
		destruct(cur -> child);
		destruct(cur -> sibling);
		delete cur;
		--sze;
	}

	Node<T>* combine(Node<T>* x, Node<T>* y) {
		if (x == nullptr) {
			return y;
		}
		if (y == nullptr) {
			return x;
		}
		try {
			if (cmp(*y -> val, *x -> val)) {
				y -> sibling = x -> child;
				x -> child = y;
				return x;
			}
			x -> sibling = y -> child;
			y -> child = x;
			return y;
		} catch (sjtu::runtime_error) {
			throw;
		}
	}

	Node<T>* popProcess(Node<T>* cur) {
		if (cur == nullptr || cur -> sibling == nullptr) {
			return cur;
		}
		Node<T>* former = cur;
		Node<T>* latter = cur -> sibling;
		Node<T>* nex = latter -> sibling;
		former -> sibling = nullptr;
		latter -> sibling = nullptr;
		try {
			return combine(popProcess(nex), combine(former, latter));
		} catch (sjtu::runtime_error) {
			throw;
		}
	}


};

}

#endif