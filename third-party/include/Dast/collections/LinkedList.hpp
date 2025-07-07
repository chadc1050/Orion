#pragma once
#include <stdexcept>

namespace Dast {
    template<typename T>
    struct Node {
        T data;
        Node* next;

        explicit Node(T data) : data(data), next(nullptr) {}
        Node(T data, Node* next) : data(data), next(next) {}
        Node() : next(nullptr) {}

        bool operator==(Node other) {
            return next == other.next;
        }

        bool operator!=(Node other) {
            return next != other.next;
        }
    };

    template<typename T>
    class LinkedList {
    public:
        LinkedList() = default;

        LinkedList(std::initializer_list<T> init) {
            for (size_t i = init.size(); i > 0; --i) {
                push_front(init.begin()[i - 1]);
            }
        }

        LinkedList(const LinkedList& other) {
            for (Node<T>* curr = other.head; curr != nullptr; curr = curr->next) {
                push_back(curr->data);
            }
        }

        LinkedList& operator=(const LinkedList& other) {
            if (this != &other) {
                clear();
                for (Node<T>* curr = other.head; curr != nullptr; curr = curr->next) {
                    push_back(curr->data);
                }
            }
            return *this;
        }

        LinkedList(LinkedList&& other) noexcept : head(other.head) {
            other.head = nullptr;
        }

        LinkedList& operator=(LinkedList&& other) noexcept {
            if (this != &other) {
                clear();
                head = other.head;
                other.head = nullptr;
            }
            return *this;
        }

        struct Iterator {

            Node<T>* curr;

            explicit Iterator(Node<T>* curr) noexcept : curr(curr) {}

            Iterator& operator=(Node<T>* node) {
                curr = node;
                return *this;
            }

            Iterator& operator++() {
                if (curr != nullptr) {
                    curr = curr->next;
                }
                return *this;
            }

            Iterator operator++(int) {
                Iterator iterator = *this;
                ++*this;
                return iterator;
            }

            bool operator==(const Iterator& other) {
                return curr == other.curr;
            }

            bool operator!=(const Iterator& other) {
                return curr != other.curr;
            }

            int operator*() {
                if (curr == nullptr) {
                    throw std::runtime_error("Dereferencing end iterator");
                }
                return curr->data;
            }
        };

        Iterator begin() {
            return Iterator(head);
        }

        Iterator begin() const {
            return Iterator(head);
        }

        Iterator end() {
            return Iterator(nullptr);
        }

        Iterator end() const {
            return Iterator(nullptr);
        }

        Node<T>* get_ptr() {
            return head;
        }

        T get_head() {
            return head->data;
        }

        T get_tail() {
            Node<T>* curr = head;
            while (curr->next != nullptr) {
                curr = curr->next;
            }
            return curr->data;
        }

        T operator[](const size_t index) {
            return at(index);
        }

        T at(const size_t index) {
            Node<T>* curr = head;
            for (size_t i = 0; i < index; i++) {
                if (curr->next == nullptr) {
                    throw std::out_of_range("Index out of range");
                }
                curr = curr->next;
            }

            return curr->data;
        }

        bool contains(const T item) {
            Node<T>* curr = head;
            while (curr != nullptr) {
                if (curr->data == item) {
                    return true;
                }
                curr = curr->next;
            }
            return false;
        }

        void insert(const size_t index, T item) {
            Node<T>* curr = head;
            for (size_t i = 0; i < index; i++) {
                if (curr->next == nullptr) {
                    throw std::out_of_range("Index out of range");
                }
                curr = curr->next;
            }
            curr->next = new Node<T>(item, curr->next);
        }

        size_t size() {
            Node<T>* curr = head;
            size_t size = 0;
            while (curr != nullptr) {
                curr = curr->next;
                size++;
            }
            return size;
        }

        bool empty() {
            return head == nullptr;
        }

        void push_front(T item) {
            if (head == nullptr) {
                head = new Node<T>(item);
                return;
            }

            head = new Node<T>(item, head);
        }


        void push_back(T item) {

            if (head == nullptr) {
                head = new Node<T>(item);
                return;
            }

            Node<T>* curr = head;

            while (curr->next != nullptr) {
                curr = curr->next;
            }

            curr->next = new Node<T>(item);
        }

        void pop_front() {
            if (head != nullptr) {
                Node<T>* temp = head;
                head = head->next;
                delete temp;
            }
        }

        void remove(const size_t index) {
            Node<T>* curr = head;

            if (this->size() == 0 || index >= this->size()) {
                throw std::out_of_range("Index out of range");
            }

            // Delete the head
            if (index == 0) {
                Node<T>* next = curr->next;
                delete curr;
                head = next;
                return;
            }

            for (size_t i = 0; i < index - 1; i++) {
                if (curr->next == nullptr) {
                    throw std::out_of_range("Index out of range");
                }
                curr = curr->next;
            }

            Node<T>* next = curr->next;
            curr->next = next->next;
            delete next;
        }

        void clear() {
            while (head != nullptr) {
                pop_front();
            }
        }

        ~LinkedList() {
            clear();
        }

    private:
        Node<T>* head = nullptr;
    };
}
