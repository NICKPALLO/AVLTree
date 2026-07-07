#ifndef AVLTREE_H
#define AVLTREE_H

template<class T>
class Node{
    public:
    Node(const T& val) : data(val) {}
    Node<T>* left{nullptr};
    Node<T>* right{nullptr};
    Node<T>* parent{nullptr};
    T data;
};

template<class T>
Node<T>* RotateRight(Node<T>* tree)
{
    if(!tree->left)
    {
        return tree;
    }
    Node<T>* newHead = tree->left;
    Node<T>* tmp = newHead->right;


    newHead->parent = tree->parent; 
    if(tree->parent)
    {
        if(tree->parent->left == tree)
        {
            tree->parent->left = newHead;
        }
        else
        {
            tree->parent->right = newHead;
        }
    }
    newHead->right = tree;
    tree->parent = newHead;
    tree->left = tmp;
    if(tmp)
    {
        tmp->parent = tree->left;
    }
    return newHead; 
}

template<class T>
Node<T>* RotateLeft(Node<T>* tree)
{
    if(!tree->right)
    {
        return tree;
    }
    Node<T>* newHead = tree->right;
    Node<T>* tmp = newHead->left;


    newHead->parent = tree->parent; 
    if(tree->parent)
    {
        if(tree->parent->left == tree)
        {
            tree->parent->left = newHead;
        }
        else
        {
            tree->parent->right = newHead;
        }
    }
    newHead->left = tree;
    tree->parent = newHead;
    tree->right = tmp;
    if(tmp)
    {
        tmp->parent = tree->left;
    }
    return newHead; 
}

template<class T>
class AVL{
    public:
    AVL(){}

    void insert(const T& val)
    {
        insert_into(val,head,nullptr);
    }
    Node<T>* find(const T& val)
    {
        return find_in(val,head);
    }
    bool erase(const T& val)
    {
        Node<T>* node = find(val);
        if(!node)
        {
            return false;
        }
        erase_node(head,node);
        return true;
    }

    ~AVL(){
        deleteNode(head);
    }

    Node<T>* head{nullptr};

    private:
    void deleteNode(Node<T>* node)
    {
        if(node)
        {
            if(node->left)
            {
                deleteNode(node->left);
            }   
            if(node->right)
            {
                deleteNode(node->right);
            }
            delete node;
        }
    }
    void insert_into(const T& val, Node<T>*& node, Node<T>* parent)
    {
        if(!node)
        {
            node = new Node<T>(val);
            node->parent = parent;
        }
        if(val == node->data)
        {
            return;
        }
        else if(val<node->data)
        {
            insert_into(val, node->left,node);
        }
        else
        {
            insert_into(val, node->right,node);
        }
    }
    Node<T>* find_in(const T& val, Node<T>* node)
    {
        if(!node)
        {
            return nullptr;
        }
        if(node->data == val)
        {
            return node;
        }
        if(val < node->data)
        {
            return find_in(val,node->left);
        }
        else
        {
            return find_in(val,node->right);
        }
    }
    Node<T>* findLeft(Node<T>* node)
    {
        return node->left ? findLeft(node->left) : node;
    }
    void transplant(Node<T>*& root, Node<T>* u, Node<T>* v)
    {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;

        if (v != nullptr)
            v->parent = u->parent;
    }
    void erase_node(Node<T>*& root, Node<T>* z)
    {
        if (z->left == nullptr)
        {
            transplant(root, z, z->right);
            delete z;
        }
        else if (z->right == nullptr)
        {
            transplant(root, z, z->left);
            delete z;
        }
        else
        {
            Node<T>* y = findLeft(z->right);

            if (y->parent != z)
            {
                transplant(root, y, y->right);

                y->right = z->right;
                y->right->parent = y;
            }

            transplant(root, z, y);

            y->left = z->left;
            y->left->parent = y;

            delete z;
        }
    }
};

#endif //AVLTREE_H