//
// Created by Zdziszkee on 18.11.2023.
//

#ifndef CURSORLIST_HPP
#define CURSORLIST_HPP


#include <stdexcept>


template<class T>
class CursorList {
private:
    struct Node {
        T value;
        int next_node_index;
    };

    Node* array = nullptr;
    int head = -1;
    int tail = -1;
    int current_size = 0;
    int spare = 0; //next index in array with free spot
    int max_size{};

public:
    struct Iterator {
        int current;
        Node* array;

        Iterator(const int& index, Node* array) : current(index), array(array) {
        }

        T& operator*() { return this->array[current].value; }

        Iterator& operator++() {
            this->current = this->array[current].next_node_index;
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return this->current == other.current;
        }

        bool operator!=(const Iterator& other) const {
            return this->current != other.current;
        }
    };

    Iterator begin() { return Iterator(this->head, this->array); }
    Iterator end() { return Iterator(-1, this->array); }

    explicit CursorList(int size) // DEFAULT CONSTRUCTOR
    {
        this->max_size = size;
        array = new Node[this->max_size];
        for (int i = 0; i < max_size - 2; ++i) {
            array[i].value = 0;
            array[i].next_node_index = i + 1;
        }
        array[max_size - 1].value = 0;
        array[max_size - 1].next_node_index = -1;
    }


    CursorList(CursorList& other) // COPY CONSTRUCTOR
        : CursorList() {
        Iterator iterator = other.begin();
        while (iterator != other.end()) {
            this->push_back(*iterator);
            ++iterator;
        }
    }

    CursorList(CursorList&& other) noexcept // MOVE CONSTRUCTOR
        : array(other.array), head(other.head), tail(other.tail), current_size(other.current_size), spare(other.spare),
          max_size(other.max_size) {
    }

    template<class U>
    void push_front(U&& x) // Dołącza element na początek listy
    {
        if (this->current_size >= max_size) {
            throw std::overflow_error("CursorList is full");
        }

        array[spare].value = std::forward<U>(x);
        int previous_head = head;
        head = spare;
        spare = array[spare].next_node_index;
        array[head].next_node_index = previous_head;
        if (current_size == 0) {
            tail = head;
        }
        ++current_size;
    }

    T pop_front() // Usuwa i zwraca element z początku listy
    {
        if (this->current_size <= 0) {
            throw std::out_of_range("CursorList is empty");
        }

        T x = array[head].value;
        int next_spare = head;
        head = array[head].next_node_index;
        array[next_spare].next_node_index = spare;
        spare = next_spare;
        --current_size;
        if (this->current_size == 0) {
            head = -1;
            tail = -1;
        }
        return x;
    }

    template<class U>
    void push_back(U&& x) // Dołącza element na koniec listy
    {
        if (this->current_size == max_size) {
            throw std::overflow_error("CursorList is full");
        }

        array[spare].value = std::forward<U>(x);
        array[tail].next_node_index = spare;
        tail = spare;
        spare = array[spare].next_node_index;
        array[tail].next_node_index = -1;
        if (current_size == 0) {
            head = tail;
        }
        ++current_size;
    }

    T pop_back() // Usuwa i zwraca element z końca listy
    {
        if (this->current_size <= 0) {
            throw std::out_of_range("CursorList is empty");
        }

        T x = array[tail].value;
        int new_spare = tail;
        int n = head;

        if (n == tail) {
            array[new_spare].next_node_index = spare;
            spare = new_spare;
            head = -1;
            tail = -1;
        } else {
            while (array[n].next_node_index != tail) {
                n = array[n].next_node_index;
            }
            tail = n;
            array[new_spare].next_node_index = spare;
            array[tail].next_node_index = -1;
            spare = new_spare;
        }
        --current_size;

        return x;
    }

    int size() // Zwraca liczbę elementów w liście
    const {
        return this->current_size;
    }

    bool empty() // Zwraca `true` gdy lista jest pusta
    const {
        return current_size == 0;
    }

    Iterator find(int x) // Zwraca pozycję pierwszego elementu o wartości x
    {
        Iterator iterator = this->begin();
        while (iterator != this->end()) {
            if (*iterator == x) {
                return iterator;
            }
            ++iterator;
        }

        throw std::out_of_range("Element not found");
    }

    Iterator erase(Iterator iterator) // Usuwa it zwraca element na pozycji `it`
    {
        if (this->current_size <= 0)
            throw std::out_of_range("CursorList is empty");

        if (iterator == this->begin) {
            this->pop_front();
            return this->begin();
        }
        int n = this->head;
        while (array[n].next_node_index != iterator.current) {
            n = array[n].next_node_index;
        }
        array[n].next_node_index = array[iterator.current].next_node_index;

        int next_spare = iterator.current;
        array[next_spare].next_node_index = spare;
        spare = next_spare;

        --current_size;

        return Iterator(this->array[n].next_node_index, this->array);
    }

    template<class U>
    Iterator insert(Iterator iterator, U&& x) // Wstawia element `x` przed pozycję `it`
    {
        if (this->current_size == max_size)
            throw std::overflow_error("List is full");

        if (iterator == this->begin()) {
            this->push_front(std::forward<U>(x));
            return this->begin();
        }
        int n = this->head;
        while (array[n].next_node_index != iterator.current) {
            n = array[n].next_node_index;
        }
        int node = spare;
        spare = array[spare].next_node_index;

        array[node].value = std::forward<U>(x);
        array[node].next_node_index = iterator.current;
        array[n].next_node_index = node;
        ++current_size;
        return Iterator(node, this->array);
    }

    int remove(int x) // Usuwa wystąpienia `x` i zwraca ich liczbę
    {
        int count = 0;
        Iterator iterator = this->begin();
        while (iterator != this->end()) {
            if (*iterator == x) {
                ++count;
                iterator = this->erase(iterator);
            } else {
                ++iterator;
            }
        }

        return count;
    }
};

#endif //CURSORLIST_HPP
