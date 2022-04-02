#include <iostream>
#include <cstdlib>

using namespace std;

template <class ItemType>
struct TreeNode {
    ItemType item;
    TreeNode *left;
    TreeNode *right;
};

template <class ItemType>
void Insert(TreeNode<ItemType> *root, ItemType item) {
    if (item < root->item) {
        if (root->left == NULL) {
            TreeNode<ItemType> *node = new TreeNode<ItemType> {item, NULL, NULL};
            root->left = node;
        } else {
            Insert(root->left, item);
        }
    } else {
        if (root->right == NULL) {
            TreeNode<ItemType> *node = new TreeNode<ItemType> {item, NULL, NULL};
            root->right = node;
        } else {
            Insert(root->right, item);
        }
    }
}

template <class ItemType>
int Count(TreeNode<ItemType> *root) {
    if (root == NULL) {
        return 0;
    }

    return 1 + Count<ItemType>(root->left) + Count<ItemType>(root->right);
}

template <class ItemType>
class TreeType {
    public:
        TreeType() {
            root = NULL;
        }
        int GetLength() {
            return Count<ItemType>(root);
        }
        void MakeEmpty();
        void PutItem(ItemType item){
            if (root == NULL) {
                TreeNode<ItemType> *node = new TreeNode<ItemType>;
                node->item = item;
                node->left = node->right = NULL;
                root = node;
            } else {
                Insert<ItemType>(root, item);
            }
        }
        void DeleteItem(ItemType item);
        bool Find(ItemType item);
    private:
        TreeNode<ItemType> *root;
};
