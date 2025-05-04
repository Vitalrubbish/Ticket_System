#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include <cstddef>
#include <memory>
#include <utility>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a
 * list.
 */
template <typename T> class list {
protected:
  class node {
  public:
   alignas(T) unsigned char data[sizeof(T)]{};
   node* nxt = nullptr;
   node* prv = nullptr;
    /**
     * add data members and constructors & destructor
     */

   template <typename... Args>
   explicit node(Args&&... args) : prv(nullptr), nxt(nullptr) {
    new (data) T(std::forward<Args>(args)...);
   }

   ~node() {
    reinterpret_cast<T*>(data)->~T();
   }

   T& get() {
    return *reinterpret_cast<T*>(data);
   }

   const T& get() const {
    return *reinterpret_cast<const T*>(data);
   }
  };

protected:
  /**
   * add data members for linked list as protected members
   */
 node* head = nullptr;
 node* tail = nullptr;
 int sze = 0;
 std::allocator<node> alloc;
  /**
   * insert node cur before node pos
   * return the inserted node cur
   */
  node *insert(node *pos, node *cur) {
   if (pos == head) {
    head = cur;
   }
   else {
    pos -> prv -> nxt = cur;
    cur -> prv = pos -> prv;
   }
   cur -> nxt = pos;
   pos -> prv = cur;
   ++sze;
   return cur;
  }
  /**
   * remove node pos from list (no need to delete the node)
   * return the removed node pos
   */
  node *erase(node *pos) {
   if (pos == head) {
    head = head -> nxt;
    head -> prv = nullptr;
   }
   else {
    pos -> prv -> nxt = pos -> nxt;
   }
   pos -> nxt -> prv = pos -> prv;
   return pos;
  }

public:
  class const_iterator;
  class iterator {
  public:
   node* cursor = nullptr;
    /**
     * TODO add data members
     *   just add whatever you want.
     */

   explicit iterator(node* cursor = nullptr) {
    this -> cursor = cursor;
   }

    iterator operator++(int) {
    if (cursor -> nxt == nullptr) {
     return *this;
    }
    iterator tmp = *this;
    cursor = cursor -> nxt;
    return tmp;

    }
    iterator &operator++() {
    if (cursor -> nxt == nullptr) {
     return *this;
    }
    cursor = cursor -> nxt;
    return *this;
    }
    iterator operator--(int) {
    if (cursor -> prv == nullptr) {
     return *this;
    }
    iterator tmp = *this;
     cursor = cursor -> prv;
     return tmp;
    }

    iterator &operator--() {
    if (cursor -> prv == nullptr) {
     return *this;
    }
     cursor = cursor -> prv;
     return *this;
    }

   node* getCursor() {
    return cursor;
   }
    /**
     * TODO *it
     * throw std::exception if iterator is invalid
     */
    T &operator*() const {
     return cursor -> get();
    }
    /**
     * TODO it->field
     * throw std::exception if iterator is invalid
     */
    T *operator->() const noexcept {
     return &cursor -> get();
    }

    /**
     * a operator to check whether two iterators are same (pointing to the same
     * memory).
     */
    bool operator==(const iterator &rhs) const {
     return cursor == rhs.cursor;
    }
    bool operator==(const const_iterator &rhs) const {
     return cursor == rhs.cursor;
    }
    
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const {
     return cursor != rhs.cursor;
    }
    bool operator!=(const const_iterator &rhs) const {
     return cursor != rhs.cursor;
    }
  };

  /**
   * TODO
   * has same function as iterator, just for a const object.
   * should be able to construct from an iterator.
   */
  class const_iterator {
  private:
   const node* cursor = nullptr;
   /**
    * TODO add data members
    *   just add whatever you want.
    */

  public:
   explicit const_iterator(const node* cursor = nullptr) {
    this -> cursor = cursor;
   }

   explicit const_iterator(const iterator &iter) {
    cursor = iter.cursor;
   }

   node* getCursor() {
    return cursor;
   }

   const_iterator operator++(int) {
    if (cursor -> nxt == nullptr) {
     return *this;
    }
    const_iterator tmp = *this;
    cursor = cursor -> nxt;
    return tmp;
   }
   const_iterator &operator++() {
    if (cursor -> nxt == nullptr) {
     return *this;
    }
    cursor = cursor -> nxt;
    return *this;
   }
   const_iterator operator--(int) {
    if (cursor -> prv == nullptr) {
     return *this;
    }
    const_iterator tmp = *this;
    cursor = cursor -> prv;
    return tmp;
   }
   const_iterator &operator--() {
    if (cursor -> prv == nullptr) {
     return *this;
    }
    cursor = cursor -> prv;
    return *this;
   }

   /**
    * TODO *it
    * throw std::exception if iterator is invalid
    */
   const T &operator*() const {
    return cursor -> get();
   }
   /**
    * TODO it->field
    * throw std::exception if iterator is invalid
    */
   const T *operator->() const noexcept {
    return &cursor -> get();
   }

   /**
    * a operator to check whether two iterators are same (pointing to the same
    * memory).
    */
   bool operator==(const iterator &rhs) const {
    return cursor == rhs.cursor;
   }
   bool operator==(const const_iterator &rhs) const {
    return cursor == rhs.cursor;
   }

   /**
    * some other operator for iterator.
    */
   bool operator!=(const iterator &rhs) const {
    return cursor != rhs.cursor;
   }
   bool operator!=(const const_iterator &rhs) const {
    return cursor != rhs.cursor;
   }
  };

  /**
   * TODO Constructs
   * At least two: default constructor, copy constructor
   */
  list() {
   head = tail = alloc.allocate(1);
   head -> prv = tail -> prv = nullptr;
   head -> nxt = tail -> nxt = nullptr;
   sze = 0;
  }

  list(const list &other) {
   head = tail = alloc.allocate(1);
   head -> prv = tail -> prv = nullptr;
   head -> nxt = tail -> nxt = nullptr;
   node* p = other.head;
   while (p != other.tail) {
    push_back(p -> get());
    p = p -> nxt;
   }
   sze = other.sze;
  }
  /**
   * TODO Destructor
   */
  virtual ~list() {
   node* p = head;
   while (p != tail) {
    node* tmp = p -> nxt;
    delete p;
    p = tmp;
   }
   sze = 0;
   alloc.deallocate(tail, 1);
   head = tail = nullptr;
  }
  /**
   * TODO Assignment operator
   */
  list &operator=(const list &other) {
   if (this == &other) {
    return *this;
   }
   clear();
   node* p = other.head;
   while (p != other.tail) {
    push_back(p -> get());
    p = p -> nxt;
   }
   sze = other.sze;
   return *this;
  }
  /**
   * access the first / last element
   * throw container_is_empty when the container is empty.
   */
  const T &front() const {
   node* p = head;
   return p -> get();
  }
  const T &back() const {
   node* p = tail -> prv;
   return p -> get();
  }
  /**
   * returns an iterator to the beginning.
   */
  iterator begin() {
   iterator ret(head);
   return ret;
  }
  const_iterator cbegin() const {
   const_iterator ret(head);
   return ret;
  }

  /**
   * returns an iterator to the end.
   */
  iterator end() {
   iterator ret(tail);
   return ret;
  }
  const_iterator cend() const {
   const_iterator ret(tail);
   return ret;
  }
  /**
   * checks whether the container is empty.
   */
  virtual bool empty() const {
   return sze == 0;
  }
  /**
   * returns the number of elements
   */
  virtual size_t size() const {
   return sze;
  }

  /**
   * clears the contents
   */
  virtual void clear() {
   node* p = head;
   while (p != tail) {
    node* tmp = p -> nxt;
    delete p;
    p = tmp;
   }
   head = tail;
   sze = 0;
  }
  /**
   * insert value before pos (pos may be the end() iterator)
   * return an iterator pointing to the inserted value
   * throw if the iterator is invalid
   */
  virtual iterator insert(iterator pos, const T &value) {
   node* new_node = new node{value};
   node* cur = pos.getCursor();
   if (cur == head) {
    head -> prv = new_node;
    new_node -> nxt = head;
    head = new_node;
   }
   else {
    new_node -> nxt = cur;
    new_node -> prv = cur -> prv;
    cur -> prv -> nxt = new_node;
    cur -> prv = new_node;
   }
   iterator ret(new_node);
   ++sze;
   return ret;
  }
  /**
   * remove the element at pos (the end() iterator is invalid)
   * returns an iterator pointing to the following element, if pos pointing to
   * the last element, end() will be returned. throw if the container is empty,
   * the iterator is invalid
   */
  virtual iterator erase(iterator pos) {
   node* cur = pos.getCursor();
   if (cur == tail) {
    return pos;
   }
   iterator ret(cur -> nxt);
   if (cur != head) {
    cur -> prv -> nxt = cur -> nxt;
   }
   else {
    head = cur -> nxt;
   }
   cur -> nxt -> prv = cur -> prv;
   --sze;
   delete cur;
   return ret;
  }
  /**
   * adds an element to the end
   */
   iterator push_back(const T &value) {
   node* new_node = new node{value};
   new_node -> nxt = tail;
   if (sze == 0) {
    head = new_node;
   }
   else {
    new_node -> prv = tail -> prv;
    new_node -> prv -> nxt = new_node;
   }
   tail -> prv = new_node;
   ++sze;
   return iterator{new_node};
  }
  /**
   * removes the last element
   * throw when the container is empty.
   */
  void pop_back() {
   if (sze == 0) {return;}
   node* p = tail -> prv;
   node* tmp = p -> prv;
   delete p;
   tail -> prv = tmp;
   if (tmp != nullptr) {
    tmp -> nxt = tail;
   }
   else {
    head = tail;
   }
   --sze;
  }
  /**
   * inserts an element to the beginning.
   */
  void push_front(const T &value) {
   node* new_node = new node{value};
   new_node -> nxt = head;
   head -> prv = new_node;
   head = new_node;
   ++sze;
  }
  /**
   * removes the first element.
   * throw when the container is empty.
   */
  void pop_front() {
   if (sze == 0){return;}
   node* tmp = head -> nxt;
   delete head;
   head = tmp;
   head -> prv = nullptr;
   --sze;
  }
};

} // namespace sjtu

#endif // SJTU_LIST_HPP
