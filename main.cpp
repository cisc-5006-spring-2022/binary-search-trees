#include <iostream>
#include "TreeType.h"

using namespace std;

int main() {
    cout << "I'm a tree" << endl;

    TreeType<int> t;

    t.PutItem(10);
    t.PutItem(3);
    t.PutItem(19);
    t.PutItem(14);

    t.PrintTree();
    cout << "Delete 10" << endl;
    t.DeleteItem(10);
    t.PrintTree();

}