#include <iostream>
#include <vector>
using namespace std;

template <typename T>
struct Node {
    T value;
    Node* left{ nullptr }, * right{ nullptr }, * parent{ nullptr };

    Node(T value) : value(value) {}

    Node(T value, Node<T>* left, Node<T>* right) : value(value), left(left), right(right) {
        if (left)
            left->parent = this;
        if (right)
            right->parent = this;
    }

    struct NodeIterator {
        Node* current = nullptr;

        NodeIterator(Node* current) : current{ current } {}
        ~NodeIterator() = default;

        bool operator==(const NodeIterator& other) const {
            return current == other.current;
        }
        bool operator!=(const NodeIterator& other) const {
            return current != other.current;
        }

        Node* r = nullptr;
        bool find_r_recursively() const {
            for (Node* tmp = current; tmp->right; tmp = tmp->right) {
                if (tmp->right == r) {
                    return true;
                }
            }
            return false;
        }
        NodeIterator& operator++() {
            if (current->left) {
                current = current->left;
            }
            else if (current->right) {
                current = r = current->right;
            }
            else {
                if (!current->parent) {
                    current = nullptr;
                }
                else {
                    while (true) {
                        if (!current->parent)
                            break;
                        else if (current->right && current->right != r)
                            break;
                        current = current->parent;
                    }

                    if (!current->parent && find_r_recursively()) {
                        current = nullptr;
                    }
                    else {
                        current = r = current->right;
                    }
                }
            }
            return *this;
        }
    };

    NodeIterator begin() const {
        Node* root = (parent) ? parent : const_cast<Node*>(this);
        while (root->parent) {
            root = root->parent;
        }
        return NodeIterator{ root };
    }

    NodeIterator end() const {
        return NodeIterator{ nullptr };
    }

    void preorder_traversal(vector<Node<T>*>& result) {
        for (auto it = begin(); it != end(); ++it) {
            result.push_back(it.current);
        }
    }
};

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("test 1") {
    /**
    *    A
    *   / \
    *  B   C
    */
    const vector<char> expected = { 'a', 'b', 'c' };
    vector<Node<char>*> result;

    Node<char> b{ 'b' }, c{ 'c' };
    Node<char> a{ 'a', &b, &c };

    a.preorder_traversal(result);

    CHECK(expected.size() == result.size());

    for (size_t i = 0; i < expected.size(); i++) {
        CHECK(expected[i] == result[i]->value);
    }
}

TEST_CASE("test 2") {
    /**
    *    A
    *   / \
    *  B   C
    *     /
    *    D
    */
    const vector<char> expected = { 'a', 'b', 'c', 'd'};
    vector<Node<char>*> result;

    Node<char> b{ 'b' }, d{ 'd' };
    Node<char> c{ 'c', &d, nullptr };
    Node<char> a{ 'a', &b, &c };

    a.preorder_traversal(result);

    CHECK(expected.size() == result.size());

    for (size_t i = 0; i < expected.size(); i++) {
        CHECK(expected[i] == result[i]->value);
    }
}

TEST_CASE("test 3") {
    /**
    *    A
    *   / \
    *  B   C
    *       \
    *        D
    */
    const vector<char> expected = { 'a', 'b', 'c', 'd' };
    vector<Node<char>*> result;

    Node<char> b{ 'b' }, d{ 'd' };
    Node<char> c{ 'c', nullptr, &d };
    Node<char> a{ 'a', &b, &c };

    a.preorder_traversal(result);

    CHECK(expected.size() == result.size());

    for (size_t i = 0; i < expected.size(); i++) {
        CHECK(expected[i] == result[i]->value);
    }
}

TEST_CASE("test 4") {
    /**
    *      A
    *     / \
    *    B   E
    *   / \
    *  C   D
    */
    const vector<char> expected = { 'a', 'b', 'c', 'd', 'e'};
    vector<Node<char>*> result;

    Node<char> e{ 'e' };
    Node<char> c{ 'c' }, d{ 'd' };
    Node<char> b{ 'b', &c, &d };
    Node<char> a{ 'a', &b, &e };

    a.preorder_traversal(result);

    CHECK(expected.size() == result.size());

    for (size_t i = 0; i < expected.size(); i++) {
        CHECK(expected[i] == result[i]->value);
    }
}

TEST_CASE("test 5") {
    /**
    *         A
    *       /   \
    *      B     E
    *     / \     \
    *    C   D     F
    *     \
    *      G
    */
    const vector<char> expected = { 'a', 'b', 'c', 'g', 'd', 'e', 'f'};
    vector<Node<char>*> result;

    Node<char> g{ 'g' }, f{ 'f' }, d{ 'd' };
    Node<char> e{ 'e', nullptr, &f };
    Node<char> c{ 'c', nullptr, &g };
    Node<char> b{ 'b', &c, &d };
    Node<char> a{ 'a', &b, &e };

    a.preorder_traversal(result);

    CHECK(expected.size() == result.size());

    for (size_t i = 0; i < expected.size(); i++) {
        CHECK(expected[i] == result[i]->value);
    }
}
