#include <bits/stdc++.h>
#include <cmath>


bool CHANGED_BALANCE; //Проверка на изменение баланса при удалении
struct Room{
    char letter;
    int number;

    Room(){
        letter = 'A';
        number = 100;
    }

    Room(char let, int num){
        letter = let;
        number = num;
    }
};

struct Node{
    Room* room;
    Node* next;
    Node* prev;

    Node(){
        room = nullptr;
        next = nullptr;
        prev = nullptr;
    }

    Node(char let, int num){
        room = new Room(let, num);
        next = nullptr;
        prev = nullptr;
    }

    ~Node() = default;
};

struct TNode{
    int height;
    Node* list;
    TNode* left;
    TNode* right;

    TNode(){
        list = nullptr;
        left = nullptr;
        right = nullptr;
        height = 1;
    }

    TNode(char let, int num){
        list = new Node(let, num);
        left = nullptr;
        right = nullptr;
        height = 1;
    }

    ~TNode() = default;
};

char GetRoomLetter(Node* node){
    return node->room->letter;
}

int GetRoomNumber(Node* node){
    return node->room->number;
}

int NodeNumber(Node* node){
    int number = 1;
    if (node->next == nullptr)
        return number;
    Node* current = node->next;
    while(current != nullptr){
        number++;
        current = current->next;
    }
    return number;
}

void DeleteFromList(TNode* node){
    Node* listnode = node->list;
    Node* temp = listnode->prev;
    temp->prev->next = listnode;
    listnode->prev = temp->prev;
    delete(temp);
}

void AddToList(Node* &node){
    Node* newone = new Node(GetRoomLetter(node), GetRoomNumber(node));
    if (node != nullptr) {
        node->prev = newone;
        newone->next = node;
    }
    node = newone;
}

int height(TNode* node)
{
    return (node == nullptr ? 0 : node->height);
}

int GetBalance(TNode* node)
{
    if(node == nullptr)
        return 0;
    else
        return height(node->left) - height(node->right);
}

TNode* RightRotate(TNode* &node)
{
    TNode* u = node->left;
    node->left = u->right;
    u->right = node;
    node->height = std::max(height(node->left), height(node->right))+1;
    u->height = std::max(height(u->left), node->height)+1;
    return u;
}

TNode* LeftRotate(TNode* &node)
{
    TNode* u = node->right;
    node->right = u->left;
    u->left = node;
    node->height = std::max(height(node->left), height(node->right))+1;
    u->height = std::max(height(node->right), node->height)+1 ;
    return u;
}

TNode* RightLeftRotate(TNode* &node)
{
    node->right = RightRotate(node->right);
    return LeftRotate(node);
}

TNode* LeftRightRotate(TNode* &node)
{
    node->left = LeftRotate(node->left);
    return RightRotate(node);
}

int GetRoomNumber(TNode* p){
    return p->list->room->number;
}

char GetRoomLetter(TNode* p){
    return p->list->room->letter;
}

TNode* Max(TNode* node){
    if (node == nullptr) return nullptr;
    if (node->right == nullptr) return node;
    TNode* max = node->right;
    while (max->right != nullptr)
        max = max->right;
    return max;
}

TNode* Min(TNode* node){
    if (node == nullptr) return nullptr;
    if (node->left == nullptr) return node;
    TNode* min = node->left;
    while (min->left != nullptr)
        min = min->left;
    return min;
}

TNode* FindNode(TNode* root, char let, int num){
    if (root == nullptr) {
        std::cout << "Tree is emty.\n";
        return nullptr;
    }
    TNode* current = root;
    while((current != nullptr)&&(GetRoomNumber(current) != num || GetRoomLetter(current) != let)){
        if (GetRoomLetter(current) < let) current = current->left;
        else {
            if (GetRoomLetter(current) > let) current = current->right;
            else if (GetRoomLetter(current) == let) {
                if (GetRoomNumber(current) < num) current = current->left;
                else if (GetRoomNumber(current) > num) current = current->right;
            }
        }
    }
    if (current == nullptr) {
        std::cout << "Element couldn't be found.\n";
        return nullptr;
    }
    return current;
}


TNode* InsertInTree(TNode* root, char let, int num){
    auto newnode = new TNode(let, num);
    if (root == nullptr) return newnode;
    if ((let < GetRoomLetter(root))||
    (let == GetRoomLetter(root) && num < GetRoomNumber(root)))
        root->left = InsertInTree(root->left, let, num);
    else if ((let > GetRoomLetter(root))||
             (let == GetRoomLetter(root) && num > GetRoomNumber(root)))
        root->right = InsertInTree(root->right, let, num);
    else if (let == GetRoomLetter(root) && num == GetRoomNumber(root)) {
        AddToList(root->list);
    }
        root->height = 1 + std::max(height(root->left), height(root->right));
        int balanceFactor = GetBalance(root);
        if (balanceFactor > 1){
            if ((let < GetRoomLetter(root->left))||
                (let == GetRoomLetter(root->left) && num < GetRoomNumber(root->left))){
                return RightRotate(root);
            }
            else if ((let > GetRoomLetter(root))||
                     (let == GetRoomLetter(root) && num > GetRoomNumber(root))){
                return LeftRightRotate(root);
            }
        }
        if (balanceFactor < -1){
            if ((let > GetRoomLetter(root->left))||
                (let == GetRoomLetter(root->left) && num > GetRoomNumber(root->left))){
                return LeftRotate(root);
            }
            else if ((let < GetRoomLetter(root))||
                     (let == GetRoomLetter(root) && num < GetRoomNumber(root))){
                return RightLeftRotate(root);
            }
        }
    return root;
}

TNode* DeleteFromTree(TNode* root, char let, int num){
    if (root == nullptr){
        std::cout << "Tree is empty.\n";
        return root;
    }
    CHANGED_BALANCE = true;
    if ((let < GetRoomLetter(root))||
        (let == GetRoomLetter(root) && num < GetRoomNumber(root)))
        root->left = DeleteFromTree(root->left, let, num);
    if ((let > GetRoomLetter(root))||
        (let == GetRoomLetter(root) && num > GetRoomNumber(root)))
        root->right = DeleteFromTree(root->right, let, num);
    else{
        if (NodeNumber(root->list) > 1) {
            DeleteFromList(root);
            CHANGED_BALANCE = false;
        }
        if (CHANGED_BALANCE){
            if ((root->left == nullptr)||(root->right == nullptr)){
                TNode* temp = (root->left == nullptr) ? root->left : root->right;
                if (temp == nullptr){
                    temp = root;
                    root = nullptr;
                }
                else
                    *root = *temp;
                delete(temp);
            }
            else{
                TNode* temp = Max(root->left);
                root->list = temp->list;
                root->right = DeleteFromTree(root->right, GetRoomLetter(temp), GetRoomNumber(temp));
            }
        }
    }
    if (root == nullptr)
    return root;

    root->height = 1 + std::max(height(root->left), height(root->right));
    int balanceFactor = GetBalance(root);
    if (balanceFactor > 1){
        if (GetBalance(root->left) >= 0){
            return RightRotate(root);
        }
        else {
            return LeftRightRotate(root);
        }
    }
    if (balanceFactor < -1){
        if (GetBalance(root->right) <= 0){
            return LeftRotate(root);
        }
        else {
            return RightLeftRotate(root);
        }
    }
    return root;
}

void DeleteTree(TNode* node){
    if (node != nullptr) {
        if (node->left)
            DeleteTree(node->left);
        if (node->right)
            DeleteTree(node->right);
        delete(node->list);
        delete(node);
    }
}

void PrintOnSide(int h, TNode* node){
    if (node != nullptr){
        PrintOnSide(h + 4, node->right);
        for (int i = 0; i < h; ++i) std::cout << " ";
        std::cout << GetRoomLetter(node) << GetRoomNumber(node) << " " << NodeNumber(node->list) << std::endl;
        PrintOnSide(h + 4, node->left);
    }
}

int main() {
    TNode* tree;
    tree = InsertInTree(tree, 'A', 100);
    tree = InsertInTree(tree, 'A', 100);
    PrintOnSide(0, tree);
}
