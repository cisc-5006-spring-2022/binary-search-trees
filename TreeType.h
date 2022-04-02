#include <iostream>
#include <cstdlib>

using namespace std;

template <class ItemType>
struct TreeNode {
    ItemType info;
    TreeNode *left;
    TreeNode *right;
};

// Perform a depth-first traversal of the tree and print its contents.
template <class ItemType>
void DepthFirstPrint(TreeNode<ItemType> *root) {
    // Base case: We went past the end of the tree, and there is nothing to print.
    if (root == NULL) {
        return;
    }

    // First print the left subtree...
    DepthFirstPrint(root->left);
    // ...then print ourselves...
    cout << root->info << " ";
    // ...then print the right subtree.
    DepthFirstPrint(root->right);
}

// A recursive function InsertNode for inserting a new node at the correct position
// inside a binary search tree. It is intended to be called from inside a TreeType
// object.
//
// Note that this implementation differs from the book's, due to decisions we made
// in class. The book uses pointers to references, and I think it actually simplifies
// this process significantly (with the added overhead of having to reason about
// references to pointers). Please review both implementations.
//
// This function assumes that *root is not NULL (i.e., it is not trying to insert
// into an empty tree). It is the responsibility of the caller to ensure this is
// true. Consider how using references to pointers would remove this requirement.
template <class ItemType>
void InsertNode(TreeNode<ItemType> *root, ItemType item) {
    // When performing an insert, we have two significant things to consider:
    //
    //      1. Does the new node go into the left subtree or the right subtree from
    //         the root?
    //      2. Is there a left subtree root or not?
    //
    // Answering Question 1 allows us to decide which subtree to move into next.
    // Answering Question 2 allows us to decide if we can insert a new node, or
    // if we have to travel into the left subtree first.

    // Does the item belong in the left subtree?
    if (item < root->info) {
        // Is there a left subtree?
        if (root->left == NULL) {
            // If not, we can insert a new node to the left of the root.
            TreeNode<ItemType> *node = new TreeNode<ItemType> {item, NULL, NULL};
            root->left = node;
        } else {
            // If so, travel into it and recurse.
            InsertNode(root->left, item);
        }
    }
    // Otherwise, does the element belong in the right subtree?
    else {
        // Is there a right subtree?
        if (root->right == NULL) {
            // If not, we can insert a new node to the right of the root.
            TreeNode<ItemType> *node = new TreeNode<ItemType> {item, NULL, NULL};
            root->right = node;
        } else {
            // If so, travel into it and recurse.
            InsertNode(root->right, item);
        }
    }
}

// A recursive function CountNodes for finding the number of nodes in a tree.
// It is intended to be called from inside a TreeType object. 
template <class ItemType>
int CountNodes(TreeNode<ItemType> *root) {
    // Base case: We went past the end of the tree
    if (root == NULL) {
        // Return 0 because there is no node for us to count
        return 0;
    }

    // Recursive case: Count ourselves, then the number of nodes on both the
    // left and right subtrees.
    return 1 + CountNodes<ItemType>(root->left) + CountNodes<ItemType>(root->right);
}

// This function is used as part of the delete process to find the predecessor of
// a tree node. It does this by finding the largest value in the subtree whose root
// is *tree.
template<class ItemType>
ItemType GetPredecessor(TreeNode<ItemType> *tree) {
    // To find the largest value, travel as far to the right as possible
    while (tree->right != NULL) {
        tree = tree->right;
    }

    // Return the largest value.
    return tree->info;
}

// This function is used as part of the delete process. Its job is to decide how to
// delete a node based on how many children the node has.
template <class ItemType>
void DeleteNode(TreeNode<ItemType> *&tree) {
    // Set aside the current node to be deleted
    TreeNode<ItemType> *tempPtr = tree;

    // These two conditionals check to ensure the tree has at most one child.
    // If either left or right is NULL, we can simply replace the current node with
    // its only child.
    if (tree->left == NULL) {
        tree = tree->right;
        delete tempPtr;
    } else if (tree->right == NULL) {
        tree = tree->left;
        delete tempPtr;
    }
    
    // Otherwise, the node has two children and we must use the item replacement
    // procedure. This involves two steps:
    //
    //  1. Find the current node's predecessor (i.e., the largest node in the left
    //     subtree).
    //  2. Replace the current node's value with the value of its predecessor
    //  3. Using a normal delete procedure, delete the predecessor from the left
    //     subtree.
    else {
        ItemType data = GetPredecessor(tree->left);
        tree->info = data;
        Delete(tree->left, data);
    }
}

// This function is used as part of the delete process. Its job is to find a node to
// delete, then initiate the deletion process by calling DeleteNode().
template <class ItemType>
void Delete(TreeNode<ItemType> *&root, ItemType item) {
    // First step: Recursively try to find the node to delete.
    // Because of the preconditions, we can assume the item is in the
    // tree, and we do not need to perform any NULL checks.
    if (item < root->info) {
        // Item is in the left subtree
        Delete(root->left, item);
    } else if (item > root->info) {
        // Item is in the right subtree
        Delete(root->right, item);
    } else {
        // Item is contained at this node: delete it
        DeleteNode(root);
    }
}

template <class ItemType>
class TreeType {
    public:
        TreeType() {
            root = NULL;
        }

        /**
         * Determines the number of elements in the tree.
         * Preconditions: None
         * Postconditions: The number of elements in the tree is returned.
         */
        int GetLength() {
            // Because we can't make this recursive, immediately call out to
            // our recursive CountNodes function and pass along its return value.
            return CountNodes<ItemType>(root);
        }

        /**
         * Function: Adds item to the tree.
         * Preconditions:
         *  - Tree is not full.
         *  - Item is not in the tree.
         * Postconditions:
         *  - Item is in tree.
         *  - Binary search property is maintained.
         */
        void PutItem(ItemType item){
            // This function ultimately called InsertNode, but before we do, we have to check if the tree
            // is empty (i.e., the root pointer is NULL).
            //
            // This differs from the textbook's implementation, which uses pointers to references.
            // Performing this check would be unnecessary in an implementation using pointers to references,
            // with the added overhead of having to reason about them.
            if (root == NULL) {
                // If the root is null, we're done: insert the new node as the root and return.
                TreeNode<ItemType> *node = new TreeNode<ItemType>;
                node->info = item;
                node->left = node->right = NULL;
                root = node;
            } else {
                // If the root is not null, we have to find the right place to put it. Begin a recursive
                // search and insert process.
                InsertNode<ItemType>(root, item);
            }
        }

        /**
         * Deletes the element whose key matches item's key
         * 
         * Preconditions:
         *  - Key member of item is initialized
         *  - One and only one element in the tree has a key matchen item's key.
         * Postconditions:
         *  - No element in tree has a key matching item's key 
         */
        void DeleteItem(ItemType item) {
            // Because we can't make this recursive, immediately call out to
            // our recursive Delete function.
            //
            // Note that this implementation is identical to the book's: we are
            // using references to pointers!
            Delete(root, item);
        }
    
        // Perform a depth-first traversal to print the contents of the tree.
        void PrintTree() {
            cout << "Tree contents: ";
            DepthFirstPrint(root);
            cout << endl;
        }
    private:
        TreeNode<ItemType> *root;
};
