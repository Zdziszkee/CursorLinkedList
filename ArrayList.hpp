//
// Created by Zdziszkee on 18.11.2023.
//

#ifndef ARRAYLIST_HPP
#define ARRAYLIST_HPP
#include <stdexcept>

template<class T>
class ArrayList {
private:
    T* array;
    unsigned int max_size{};
    int front = 0; // first empty
    int back = 0; //first empty
    int current_size = 0;

    int get_index_in_range(int index) {
        return index % (max_size);
    }

public:
    struct Iterator {
        int current_index;
        T* array;
        unsigned int max_size;

        Iterator(const int& index, T* array, unsigned int max_size) : current_index(index), array(array),
                                                                      max_size(max_size) {
        }

        T& operator*() { return this->array[this->current_index % max_size]; }

        Iterator& operator++() {
            ++current_index;
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return (this->current_index % max_size) == (other.current_index % max_size);
        }

        bool operator!=(const Iterator& other) const {
            return (this->current_index % max_size) != (other.current_index % max_size);
        }
    };

    Iterator begin() { return Iterator(front - 1, this->array, max_size); }
    Iterator end() { return Iterator(back, this->array, max_size); }
    //konstruktor
    explicit ArrayList(int max_size) {
        this->array = new T[max_size];
        this->max_size = max_size;
    }

    // COPY CONSTRUCTOR
    ArrayList(ArrayList& other)
        : ArrayList() {
        auto it = other.begin();
        for (; it != other.end(); ++it) {
            this->push_back(*it);
        }
    }

    // MOVE CONSTRUCTOR
    ArrayList(ArrayList&& other)
        noexcept : array(other.array) {
    }

    template<class U>
    void push_front(U&& x) // Dołącza element na początek listy
    {
        if (size() == max_size) {
            throw std::overflow_error("Arraylist is full");
        }
        if (front == back) {
            ++back;
        }
        this->array[get_index_in_range(front--)] = std::forward<U>(x);
        ++current_size;
    }

    T pop_front() // Usuwa i zwraca element z początku listy
    {
        if (empty()) {
            throw std::underflow_error("Arraylist is empty");
        }
        --current_size;
        return this->array[get_index_in_range(++front)];
    }

    template<class U>
    void push_back(U&& x) // Dołącza element na koniec listy
    {
        if (size() == max_size) {
            throw std::overflow_error("Arraylist is full");
        }
        if (front == back) {
            --front;
        }
        this->array[get_index_in_range(back++)] = std::forward<U>(x);
        ++current_size;
    }

    T pop_back() // Usuwa i zwraca element z końca listy
    {
        if (empty()) {
            throw std::underflow_error("Arraylist is empty");
        }
        --current_size;

        return this->array[get_index_in_range(--back)];
    }

    unsigned int size() // Zwraca liczbę elementów w liście
    const {
        return current_size;
    }

    bool empty() // Zwraca `true` gdy lista jest pusta
    const {
        return size() == 0;
    }

    void clear() // Czyści listę
    {
        front = 0;
        back = 0;
        current_size = 0;
    }

    Iterator find(int x) // Zwraca pozycję pierwszego elementu o wartości x
    {
        Iterator iterator = this->begin();
        Iterator end = this->end();
        while (iterator != end) {
            if (*iterator == x) {
                return iterator;
            }
            ++iterator;
        }
        throw std::out_of_range("Element not found");
    }

    Iterator erase(Iterator iterator) // Usuwa i zwraca element na pozycji `it` i zwraca iterator do kolejnego elementu
    {
        for (int i = iterator.current_index; i < size(); ++i) {
            array[get_index_in_range(front + i)] = array[get_index_in_range(front + i + 1)];
        }
        --back;

        return ++iterator;
    }

    template<class U>
    Iterator insert(Iterator iterator, U&& x) // Wstawia element `x` przed pozycję `i`
    {
        if (size() == max_size) {
            throw std::overflow_error("Arraylist is full");
        }
        for (int i = back - 1; i > iterator.current_index; --i) {
            array[i] = array[i - 1];
        }
        ++back;
        array[iterator.current] = std::forward<U>(x);
        return iterator;
    }

    int remove(int x) // Usuwa wystąpienia `x` i zwraca ich liczbę
    {
        int count = 0;
        auto iterator = this->begin();
        auto end = this->end();
        while (iterator != end) {
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
#endif //ARRAYLIST_HPP
