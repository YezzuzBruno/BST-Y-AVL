#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED

#include "BinarySearchTree.h"
#include "BTreeNode.h"
#include <algorithm> // Necesario para std::max

template<typename T=int>
class AVLTree : public BinarySearchTree<T> {
public:
    AVLTree() : BinarySearchTree<T>() {}
    virtual ~AVLTree() { }

    virtual bool remove(const T);
    virtual bool insert(const T);
    virtual void print() const
    {
        this->inOrderTraversal(this->root);
    }

private:
    BTreeNode<T>* rightRotate(BTreeNode<T>* u);
    BTreeNode<T>* leftRotate(BTreeNode<T>* u);
    BTreeNode<T>* leftRightRotate(BTreeNode<T>* u);
    BTreeNode<T>* rightLeftRotate(BTreeNode<T>* u);
    BTreeNode<T>* balanceTree(BTreeNode<T>* u);

    BTreeNode<T>* insertAVL(BTreeNode<T>* u, T key);
    BTreeNode<T>* deleteAVL(BTreeNode<T>* u, T key);

protected:
    using BinarySearchTree<T>::root;

    void updateHeight(BTreeNode<T>* node);
    int getBalance(BTreeNode<T>* node) const;

    // --- NUEVA FUNCIÓN DE OPTIMIZACIÓN O(1) ---
    int getNodeHeight(BTreeNode<T>* node) const {
        return node == nullptr ? 0 : node->height;
    }
};

template<typename T>
BTreeNode<T>* AVLTree<T>::rightRotate(BTreeNode<T>* y) {
    BTreeNode<T>* x = y->left;
    BTreeNode<T>* z = x->right;
    x->right = y;
    y->left = z;
    updateHeight(y);
    updateHeight(x);
    return x;
}

template<typename T>
BTreeNode<T>* AVLTree<T>::leftRotate(BTreeNode<T>* x) {
    BTreeNode<T>* y = x->right;
    BTreeNode<T>* z = y->left;
    y->left = x;
    x->right = z;
    updateHeight(x);
    updateHeight(y);
    return y;
}

template<typename T>
BTreeNode<T>* AVLTree<T>::leftRightRotate(BTreeNode<T>* u) {
    u->left = leftRotate(u->left);
    return rightRotate(u);
}

template<typename T>
BTreeNode<T>* AVLTree<T>::rightLeftRotate(BTreeNode<T>* u) {
    u->right = rightRotate(u->right);
    return leftRotate(u);
}

template<typename T>
BTreeNode<T>* AVLTree<T>::insertAVL(BTreeNode<T>* u, T key) {
    if (u == nullptr) {
        return new BTreeNode<T>(key);
    }
    if (key < u->key) {
        u->left = insertAVL(u->left, key);
    } else if (key > u->key) {
        u->right = insertAVL(u->right, key);
    } else {
        return u;
    }
    updateHeight(u);
    return balanceTree(u);
}

template<typename T>
bool AVLTree<T>::insert(const T key) {
    root = insertAVL(root, key);
    return true;
}

template<typename T>
BTreeNode<T>* AVLTree<T>::deleteAVL(BTreeNode<T>* root, T key) {
    if (root == nullptr) return root;

    if (key < root->key) {
        root->left = deleteAVL(root->left, key);
    } else if (key > root->key) {
        root->right = deleteAVL(root->right, key);
    } else {
        if (root->left == nullptr || root->right == nullptr) {
            BTreeNode<T>* temp = root->left ? root->left : root->right;
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else {
                *root = *temp;
            }
            delete temp;
        } else {
            BTreeNode<T>* temp = this->findMin(root->right);
            root->key = temp->key;
            root->right = deleteAVL(root->right, temp->key);
        }
    }

    if (root == nullptr) return root;
    updateHeight(root);
    return balanceTree(root);
}

template<typename T>
bool AVLTree<T>::remove(const T key) {
    size_t initialSize = this->getSize();
    root = deleteAVL(root, key);
    return this->getSize() < initialSize;
}

// --- OPTIMIZACIÓN APLICADA: Usar getNodeHeight en lugar de this->height ---
template<typename T>
void AVLTree<T>::updateHeight(BTreeNode<T>* u) {
    if (u != nullptr) {
        u->height = 1 + std::max(getNodeHeight(u->left), getNodeHeight(u->right));
    }
}

template<typename T>
int AVLTree<T>::getBalance(BTreeNode<T>* u) const {
    if (u == nullptr) return 0;
    return getNodeHeight(u->left) - getNodeHeight(u->right);
}

template<typename T>
BTreeNode<T>* AVLTree<T>::balanceTree(BTreeNode<T>* u) {
    int balance = getBalance(u);

    if (balance > 1) {
        if (getBalance(u->left) >= 0) {
            return rightRotate(u);
        } else {
            return leftRightRotate(u);
        }
    } else if (balance < -1) {
        if (getBalance(u->right) <= 0) {
            return leftRotate(u);
        } else {
            return rightLeftRotate(u);
        }
    }
    return u;
}

#endif // AVLTREE_H_INCLUDED
