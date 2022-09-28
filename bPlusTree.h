#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
using namespace std;
#define MAX 3 // TBC I DONT GET IT int is 4 ptr is 8


class Node {
	bool isLeaf;
	int size;  
	Node** ptr;
    int *key;
	friend class BPTree;
    
public:
	Node()
    {
        isLeaf = true;
	    ptr = new Node*[MAX + 1];
	    key = new int[MAX];
    }
};

// BP tree
class BPTree {
	Node* root;
	queue<Node*> accessedNodesQueue;
	void insertInternal(int,Node*,Node*);
	int removeInternal(int,Node*,Node*);
	Node* findParent(Node*, Node*);

public:
	BPTree(){
        root = NULL;
    }
	struct returnResults search(int,int);
	void insert(int);
	int remove(int);
	// void display(Node*,int,int);
	void display(Node*);
	void displayRootFirstChild(Node*);
	void displayAccessedNodes(queue<Node*>);
	int getHeight(Node*);
	int getNumberOfNodes(Node*);
	Node* getRoot()
    {
	    return root;
    };
};
