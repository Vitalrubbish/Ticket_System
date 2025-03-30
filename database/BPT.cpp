#include "BPT.h"

template<typename T>
void BPT<T>::readNode(const int &index_) {
    node_file.seekp(index_ * sizeof(Node));
    node_file.read(reinterpret_cast<char*>(&cur), sizeof(Node));
}

template<typename T>
void BPT<T>::writeNode(Node node, const int &index_) {
    node_file.seekp(index_ * sizeof(Node));
    node_file.write(reinterpret_cast<char*>(&node), sizeof(Node));
}

template<typename T>
void BPT<T>::splitNode() {
    int mid = node_size / 2;

    Node new_node{};
    new_node.index = new_id;
    new_node.is_leaf = cur.is_leaf;
    new_node.father = cur.father;
    ++new_id;

    if (cur.is_leaf) {
        if (cur.next != -1) {
            Node cur_node = cur;
            readNode(cur.next);
            cur.prev = new_node.index;
            writeNode(cur, cur.index);
            cur = cur_node;
        }
        new_node.next = cur.next;
        new_node.prev = cur.index;
        cur.next = new_node.index;
    }

    for (int i = mid; i < cur.size; ++i) {
        new_node.storage[i - mid] = cur.storage[i];
        new_node.son[i - mid] = cur.son[i];
    }

    new_node.size = cur.size - mid;
    cur.size = mid;

    Node original_node = cur;
    if (original_node.index == root) {
        Node new_root{};
        new_root.index = new_id;
        ++new_id;
        root = new_root.index;
        original_node.father = new_node.father = root;
        new_root.storage[0] = original_node.storage[original_node.size - 1];
        new_root.storage[1] = new_node.storage[new_node.size - 1];
        new_root.son[0] = original_node.index;
        new_root.son[1] = new_node.index;
        new_root.is_leaf = false;
        new_root.size = 2;
        cur = new_root;
    }
    else {
        readNode(original_node.father);
        for (int i = 0; i < cur.size; i++) {
            if (cur.son[i] == original_node.index) {
                cur.son[i] = new_node.index;
                break;
            }
        }
        insert(original_node.storage[original_node.size - 1], original_node.index);
    }

    writeNode(original_node, original_node.index);
    writeNode(new_node, new_node.index);
    writeNode(cur, cur.index);
}

template<typename T>
void BPT<T>::insert(const T &data, const int &son_index) {
    int l = 0, r = cur.size - 1;
    while (l <= r) {
        int mid = (l + r) / 2;
        if (data == cur.storage[mid]) {
            return;
        }
        if (data < cur.storage[mid]) {
            r = mid - 1;
        }
        else {
            l = mid + 1;
        }
    }
    for (int i = cur.size - 1; i >= l; --i) {
        cur.storage[i + 1] = cur.storage[i];
        cur.son[i + 1] = cur.son[i];
    }
    cur.storage[l] = data;
    cur.son[l] = son_index;
    ++cur.size;
}

template<typename T>
void BPT<T>::remove(const T &data) {
    int pos = -1, l = 0, r = cur.size - 1;
    while (l <= r) {
        int mid = (l + r) / 2;
        if (cur.storage[mid] == data) {
            pos = mid;
            break;
        }
        if (data < cur.storage[mid]) {
            r = mid - 1;
        }
        else {
            l = mid + 1;
        }
    }
    if (pos == -1) {
        return;
    }
    for (int i = pos; i < cur.size - 1; i++) {
        cur.storage[i] = cur.storage[i + 1];
        cur.son[i] = cur.son[i + 1];
    }
    --cur.size;
}

template<typename T>
void BPT<T>::flush(int st) {
    bool flag = false;
    if (st == root) {
        return;
    }
    Node st_node = cur;
    Node son_node = cur;
    readNode(cur.father);
    while (!flag) {
        for (int i = 0; i < cur.size; i++) {
            if (cur.son[i] == son_node.index) {
                if (cur.storage[i] != son_node.storage[son_node.size - 1]) {
                    cur.storage[i] = son_node.storage[son_node.size - 1];
                    writeNode(cur, cur.index);
                    son_node = cur;
                }
                else {
                    flag = true;
                }
                break;
            }
        }
        if (cur.index == root) {
            break;
        }
        readNode(cur.father);
    }
    cur = st_node;
}

template<typename T>
bool BPT<T>::borrowFromRight() {
    bool flag = false;
    int depth_count = 0;
    Node cur_node = cur;

    while (true) {
        if (cur.index == root) {
            cur = cur_node;
            return false;
        }
        readNode(cur.father);
        for (int i = 0; i < cur.size; i++) {
            if (cur.storage[i] >= cur_node.storage[cur_node.size - 1]) {
                if (i < cur.size - 1) {
                    int p = cur.index;
                    readNode(cur.son[i + 1]);
                    if (cur.father != p) {
                        cur.father = p;
                        writeNode(cur, cur.index);
                    }
                    for (int j = 0; j < depth_count; j++) {
                        p = cur.index;
                        readNode(cur.son[0]);
                        if (cur.father != p) {
                            cur.father = p;
                            writeNode(cur, cur.index);
                        }
                    }
                    flag = true;
                }
                break;
            }
        }
        if (flag) {
            break;
        }
        ++depth_count;
    }

    if (cur.size > node_size / 2) {
        T tmp = cur.storage[0];
        int right_son = cur.son[0];
        remove(tmp);
        writeNode(cur, cur.index);
        cur = cur_node;
        insert(tmp, right_son);
        writeNode(cur, cur.index);
        flush(cur.index);
        if (right_son != -1) {
            cur_node = cur;
            readNode(right_son);
            cur.father = cur_node.index;
            writeNode(cur, cur.index);
            cur = cur_node;
        }
        return true;
    }
    cur = cur_node;
    return false;
}

template<typename T>
bool BPT<T>::borrowFromLeft() {
    bool flag = false;
    int depth_count = 0;
    Node cur_node = cur;
    while (true) {
        if (cur.index == root) {
            cur = cur_node;
            return false;
        }
        readNode(cur.father);
        for (int i = 0; i < cur.size; i++) {
            if (cur.storage[i] >= cur_node.storage[cur_node.size - 1]) {
                if (i > 0) {
                    int p = cur.index;
                    readNode(cur.son[i - 1]);
                    if (cur.father != p) {
                        cur.father = p;
                        writeNode(cur, cur.index);
                    }
                    for (int j = 0; j < depth_count; j++) {
                        p = cur.index;
                        readNode(cur.son[cur.size - 1]);
                        if (cur.father != p) {
                            cur.father = p;
                            writeNode(cur, cur.index);
                        }
                    }
                    flag = true;
                }
                break;
            }
        }
        if (flag) {
            break;
        }
        ++depth_count;
    }

    if (cur.size > node_size / 2) {
        T tmp = cur.storage[cur.size - 1];
        int left_son = cur.son[cur.size - 1];
        remove(tmp);
        writeNode(cur, cur.index);
        flush(cur.index);
        cur = cur_node;
        insert(tmp, left_son);
        writeNode(cur, cur.index);
        if (left_son != -1) {
            cur_node = cur;
            readNode(left_son);
            cur.father = cur_node.index;
            writeNode(cur, cur.index);
            cur = cur_node;
        }
        return true;
    }
    cur = cur_node;
    return false;
}


template<typename T>
void BPT<T>::combine() {
    Node cur_node = cur;

    readNode(cur.father);
    for (int i = 0; i < cur.size; i++) {
        if (cur.storage[i] >= cur_node.storage[cur_node.size - 1]) {
            if (i == cur.size - 1) {
                cur.storage[i - 1] = cur.storage[i];
                --cur.size;
                writeNode(cur, cur.index);
                readNode(cur.son[i - 1]);
                if (cur.is_leaf) {
                    cur.next = cur_node.next;
                    Node tmp = cur;
                    if (cur_node.next != -1) {
                        readNode(cur_node.next);
                        cur.prev = tmp.index;
                        writeNode(cur, cur.index);
                    }
                    cur = tmp;
                }

                for (int j = 0; j < cur_node.size; j++) {
                    cur.storage[j + cur.size] = cur_node.storage[j];
                    cur.son[j + cur.size] = cur_node.son[j];
                }
                cur.size += cur_node.size;
                writeNode(cur, cur.index);
            }
            else {
                int next_index = cur.son[i + 1];
                cur.storage[i] = cur.storage[i + 1];
                for (int j = i + 1; j < cur.size - 1; j++) {
                    cur.storage[j] = cur.storage[j + 1];
                    cur.son[j] = cur.son[j + 1];
                }
                --cur.size;
                writeNode(cur, cur.index);

                readNode(next_index);
                for (int j = 0; j < cur.size; j++) {
                    cur_node.storage[j + cur_node.size] = cur.storage[j];
                    cur_node.son[j + cur_node.size] = cur.son[j];
                }

                cur_node.size += cur.size;
                cur_node.next = cur.next;
                writeNode(cur_node, cur_node.index);
                if (cur.is_leaf && cur.next != -1) {
                    readNode(cur.next);
                    cur.prev = cur_node.index;
                    writeNode(cur, cur.index);
                }
                cur = cur_node;
            }
            break;
        }
    }
}

template<typename T>
int BPT<T>::addData(const T &data) {
    int p = root, q = -1;
    while (true) {
        readNode(p);
        if (cur.father != q) {
            cur.father = q;
            writeNode(cur, cur.index);
        }
        if (cur.is_leaf) {
            break;
        }

        int l = 0, r = cur.size - 1, mid;
        while (l <= r) {
            mid = (l + r) / 2;
            if (data <= cur.storage[mid]) {
                r = mid - 1;
            }
            else {
                l = mid + 1;
            }
        }
        if (l == cur.size) {
            l = cur.size - 1;
        }
        q = p;
        p = cur.son[l];
    }

    int prev_size = cur.size;
    insert(data, -1);
    if (cur.size != prev_size) {
        writeNode(cur, cur.index);
        if (data == cur.storage[cur.size - 1]) {
            flush(cur.index);
        }
        while (cur.size > node_size) {
            splitNode();
        }
        return 0;
    }
    return -1;
}

template <typename T>
void BPT<T>::removeData(const T &data) {
    int p = root, q = -1;
    while (true) {
        readNode(p);
        if (cur.father != q) {
            cur.father = q;
            writeNode(cur, cur.index);
        }
        if (cur.is_leaf) {
            break;
        }

        int l = 0, r = cur.size - 1;
        while (l <= r) {
            int mid = (l + r) / 2;
            if (data <= cur.storage[mid]) {
                r = mid - 1;
            }
            else {
                l = mid + 1;
            }
        }
        if (l == cur.size) {
            return;
        }

        q = p;
        p = cur.son[l];
    }

    if (cur.size == 0) {
        return;
    }

    remove(data);
    writeNode(cur, cur.index);

    while(true) {
        if (cur.size < node_size / 2 && cur.index != root) {
            bool flag = borrowFromRight();
            if (!flag) {
                flag = borrowFromLeft();
                if (!flag) {
                    combine();
                    readNode(cur.father);
                    if (cur.index == root && !cur.is_leaf && cur.size <= 1) {
                        readNode(cur.son[0]);
                        root = cur.index;
                        return;
                    }
                }
            }
        }
        else {
            break;
        }
    }
}

template <typename T>
sjtu::vector<T> BPT<T>::findData(const T& obj) {
    sjtu::vector<T> ret;
    int p = root;
    while (true) {
        readNode(p);
        if (cur.is_leaf) {
            break;
        }
        int l = 0, r = cur.size - 1;
        while (l <= r) {
            int mid = (l + r) / 2;
            if (obj <= cur.storage[mid]) {
                r = mid - 1;
            }
            else {
                l = mid + 1;
            }
        }
        p = cur.son[l];
    }

    while (true) {
        for (int i = 0; i < cur.size; i++) {
            if (obj == cur.storage[i]) {
                ret.push_back(cur.storage[i]);
            }
            else if (obj < cur.storage[i]) {
                return ret;
            }
        }
        if (cur.next != -1) {
            readNode(cur.next);
        }
        else {
            break;
        }
    }
    return ret;
}

template <typename T>
void BPT<T>::modifyData(const T& obj, const T& newObj) {
    int p = root;
    while (true) {
        readNode(p);
        if (cur.is_leaf) {
            break;
        }
        int l = 0, r = cur.size - 1;
        while (l <= r) {
            int mid = (l + r) / 2;
            if (obj <= cur.storage[mid]) {
                r = mid - 1;
            }
            else {
                l = mid + 1;
            }
        }
        p = cur.son[l];
    }
    for (int i = 0; i < cur.size; i++) {
        if (obj == cur.storage[i]) {
            cur.storage[i] = newObj;
        }
    }
    writeNode(cur, cur.index);
}

template class BPT<User>;