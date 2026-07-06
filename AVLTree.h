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

    // bool erase(const T& val)
    // {
    //     if(!head)
    //     {
    //         return false;
    //     }
    //     Node<T>* node = find(val);
    //     if(!node)
    //     {
    //         return false;
    //     }
    //     if(node->left && node->right)
    //     {
    //         Node<T>* newHead = findLeft(node->right);
    //         Node<T>* rightTemp = newHead->right;
    //         Node<T>* parentTemp = newHead->parent == node ? newHead : newHead->parent;
    //         insertInstead(newHead,node);
    //         if(parentTemp == newHead)
    //         {
    //             parentTemp->right = rightTemp;    
    //         }
    //         else
    //         {
    //             parentTemp->left = rightTemp;
    //         }
    //         if(rightTemp)
    //         {
    //             rightTemp->parent = parentTemp;
    //         }
    //     }
    //     else if(node->left)
    //     {
    //         if(node->parent)
    //         {
    //             if(node->parent->left == node)
    //             {
    //                 node->parent->left = node->left;
    //             }
    //             else
    //             {
    //                 node->parent->right = node->left;
    //             }
    //             node->left->parent = node->parent;
    //         }
    //         else
    //         {
    //             head = node->left;
    //             node->left->parent = nullptr;
    //         }
    //     }
    //     else if(node->right)
    //     {
    //         if(node->parent)
    //         {
    //             if(node->parent->left == node)
    //             {
    //                 node->parent->left = node->right;
    //             }
    //             else
    //             {
    //                 node->parent->right = node->right;
    //             }
    //             node->right->parent = node->parent;
    //         }
    //         else
    //         {
    //             head = node->right;
    //             node->right->parent = nullptr;
    //         }
    //     }
    //     else
    //     {
    //         if(node->parent)
    //         {
    //             if(node->parent->left == node)
    //             {
    //                 node->parent->left = nullptr;
    //             }
    //             else
    //             {
    //                 node->parent->right = nullptr;
    //             }
    //         }
    //         else
    //         {
    //             head = nullptr;
    //         }
    //     }
    //     delete node;
    //     return true;
    // }
    
    void erase(Node<T>*& root, Node<T>* z)
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
    void insertInstead(Node<T>* insertNode, Node<T>* insteadNode)
    {
        if(insteadNode == insertNode)
        {
            return;
        }

        //Если есть родитель, то мы разъединяем с ним связь
        if(insertNode->parent)
        {
            //если родитель это тот с кем мы меняемся то удаляем только у нас связь
            //так как связь у insteadNode еще понадобитсья
            if(insertNode->parent != insteadNode)
            {
                if(insertNode->parent->left == insertNode)
                {
                    insertNode->parent->left = nullptr;
                }
                else
                {
                    insertNode->parent->right = nullptr;
                }
            }
            insertNode->parent = nullptr;
        }


        //отсоединяем родительские связи у потомков
        if(insertNode->left)
        {
            insertNode->left->parent = nullptr;
        }
        if(insertNode->right)
        {
            insertNode->right->parent = nullptr;   
        }



        insertNode->left = insteadNode->left == insertNode ? nullptr : insteadNode->left;
        insertNode->right = insteadNode->right == insertNode ? nullptr : insteadNode->right;

        if(insertNode->left)
        {
            insertNode->left->parent = insertNode;
        }

        if(insertNode->right)
        {
            insertNode->right->parent = insertNode;
        }

        if(insteadNode->parent)
        {
            if(insteadNode->parent->left == insteadNode)
            {
                insteadNode->parent->left = insertNode;
            }
            else
            {
                insteadNode->parent->right = insertNode;
            }
        }
        else
        {
            head = insertNode;
        }
        //delete insteadNode;
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
};

#endif //AVLTREE_H