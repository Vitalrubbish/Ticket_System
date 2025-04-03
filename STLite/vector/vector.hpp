#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector
{
public:
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	int len;
	int sze;
	T* storage;
	class const_iterator;
	class iterator
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the
	// iterator points to.
	// STL algorithms and containers may use these type_traits (e.g. the following
	// typedef) to work properly. In particular, without the following code,
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

		T* head = nullptr;
		int cur = 0;
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator operator+(const int &n) const
		{
			iterator ret = *this;
			ret.cur = cur + n;
			return ret;
		}
		iterator operator-(const int &n) const
		{
			iterator ret = *this;
			ret.cur = cur - n;
			return ret;
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			if (head != rhs.head) {
				throw invalid_iterator();
			}
			return cur - rhs.cur;
		}
		iterator& operator+=(const int &n)
		{
			cur += n;
			return *this;
		}
		iterator& operator-=(const int &n)
		{
			cur -= n;
			return *this;
		}

		iterator operator++(int) {
			iterator ret = *this;
			cur++;
			return ret;
		}

		iterator& operator++() {
			cur++;
			return *this;
		}

		iterator operator--(int) {
			iterator ret = *this;
			cur--;
			return ret;
		}

		iterator& operator--() {
			cur--;
			return *this;
		}

		T& operator*() const {
			return *(head + cur);
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			if (rhs.head == head && rhs.cur == cur) {
				return true;
			}
			return false;
		}
		bool operator==(const const_iterator &rhs) const {
			if (rhs.head == head && rhs.cur == cur) {
				return true;
			}
			return false;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !(*this == rhs);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(*this == rhs);
		}
	};
	class const_iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

		const T* head = nullptr;
		int cur = 0;
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		const_iterator operator+(const int &n) const
		{
			const_iterator ret = *this;
			ret.cur = cur + n;
			return ret;
		}
		const_iterator operator-(const int &n) const
		{
			const_iterator ret = *this;
			ret.cur = cur - n;
			return ret;
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const const_iterator &rhs) const
		{
			if (head != rhs.head) {
				throw invalid_iterator();
			}
			return cur - rhs.cur;
		}
		const_iterator& operator+=(const int &n)
		{
			cur += n;
			return *this;
		}
		const_iterator& operator-=(const int &n)
		{
			cur -= n;
			return *this;
		}

		const_iterator operator++(int) {
			const_iterator ret = *this;
			cur++;
			return ret;
		}

		const_iterator& operator++() {
			cur++;
			return *this;
		}

		const_iterator operator--(int) {
			const_iterator ret = *this;
			cur--;
			return ret;
		}

		const_iterator& operator--() {
			cur--;
			return *this;
		}

		T& operator*() const {
			return *(const_cast<T*>(head) + cur);
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			if (rhs.head == head && rhs.cur == cur) {
				return true;
			}
			return false;
		}
		bool operator==(const const_iterator &rhs) const {
			if (rhs.head == head && rhs.cur == cur) {
				return true;
			}
			return false;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !(*this == rhs);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(*this == rhs);
		}

	};

	vector() {
		len = 0;
		sze = 1;
		storage = static_cast<T*> (operator new (sze * sizeof(T)));
	}

	vector(const vector &other) {
		len = other.len;
		sze = other.sze;
		storage = static_cast<T*> (operator new (sze * sizeof(T)));
		for (int i = 0; i < len; i++) {
			new (&storage[i]) T(other.storage[i]);
		}
	}

	~vector() {
		for (int i = 0; i < len; i++) {
			storage[i].~T();
		}
		operator delete (storage);
	}

	vector &operator=(const vector &other) {
		if (this == &other) {
			return *this;
		}
		for (int i = 0; i < len; i++) {
			storage[i].~T();
		}
		operator delete (storage);
		len = other.len;
		sze = other.sze;
		storage = static_cast<T*> (operator new (sze * sizeof(T)));
		for (int i = 0; i < len; i++) {
			new (&storage[i]) T(other.storage[i]);
		}
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
		if (pos < 0 || pos >= len) {
			throw index_out_of_bound();
		}
		return storage[pos];
	}
	const T & at(const size_t &pos) const {
		if (pos < 0 || pos >= len) {
			throw index_out_of_bound();
		}
		return storage[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		if (pos < 0 || pos >= len) {
			throw index_out_of_bound();
		}
		return storage[pos];
	}
	const T & operator[](const size_t &pos) const {
		if (pos < 0 || pos >= len) {
			throw index_out_of_bound();
		}
		return storage[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
		if (len == 0) {
			throw container_is_empty();
		}
		return storage[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
		if (len == 0) {
			throw container_is_empty();
		}
		return storage[len - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		iterator ret;
		ret.head = storage;
		ret.cur = 0;
		return ret;
	}
	const_iterator begin() const {
		const_iterator ret;
		ret.head = storage;
		ret.cur = 0;
		return ret;
	}
	const_iterator cbegin() const {
		const_iterator ret;
		ret.head = storage;
		ret.cur = 0;
		return ret;
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		iterator ret;
		ret.head = storage;
		ret.cur = len;
		return ret;
	}
	const_iterator end() const {
		const_iterator ret;
		ret.head = storage;
		ret.cur = len;
		return ret;
	}
	const_iterator cend() const {
		const_iterator ret;
		ret.head = storage;
		ret.cur = len;
		return ret;
	}
	/**
	 * checks whether the container is empty
	 */
	[[nodiscard]] bool empty() const {
		return len == 0;
	}
	/**
	 * returns the number of elements
	 */
	[[nodiscard]] size_t size() const {
		return len;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		for (int i = 0; i < len; i++) {
			storage[i].~T();
		}
		operator delete (storage);
		sze = 1;
		len = 0;
		storage = static_cast<T*> (operator new (sze * sizeof(T)));
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */

	void doubleSize() {
		sze *= 2;
		T* tmp = static_cast<T*> (operator new (sze * sizeof(T)));
		for (int i = 0; i < len; i++) {
			new (&tmp[i]) T(storage[i]);
			storage[i].~T();
		}
		operator delete (storage);
		storage = tmp;
	}

	void halfSize() {
		sze /= 2;
		T** tmp = static_cast<T*> (operator new (sze * sizeof(T)));
		for (int i = 0; i < len; i++) {
			new(&tmp[i]) T(storage[i]);
			storage[i].~T();
		}
		operator delete (storage);
		storage = tmp;
	}

	iterator insert(iterator pos, const T &value) {
		if (len == sze) {
			doubleSize();
		}
		new (&storage[len]) T(value);
		for (int i = len; i > pos.cur; i--) {
			storage[i] = storage[i - 1];
		}
		storage[pos.cur] = value;
		len++;
		return pos;
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		if (ind > len) {
			throw index_out_of_bound();
		}
		iterator pos;
		pos.head = storage;
		pos.cur = ind;
		if (len == sze) {
			doubleSize();
		}
		new (&storage[len]) T(value);
		for (int i = len; i > ind; i--) {
			storage[i] = storage[i - 1];
		}
		storage[ind] = value;
		len++;
		return pos;
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		len--;
		storage[pos.cur].~T();
		for (int i = pos.cur; i < len; i++) {
			storage[i] = storage[i + 1];
		}
		/*if (len < sze / 2 && sze != 1) {
			halfSize();
		}*/
		return pos;
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
		iterator ret;
		ret.head = storage;
		ret.cur = ind;
		if (ind >= len) {
			throw index_out_of_bound();
		}
		len--;
		storage[ind].~T();
		for (size_t i = ind; i < len; i++) {
			storage[i] = storage[i + 1];
		}

		/*if (len < sze / 2 && sze != 1) {
			halfSize();
		}*/
		return ret;
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		insert(len, value);
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		erase(len - 1);
	}
};


}

#endif
