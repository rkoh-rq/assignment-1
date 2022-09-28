#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include "storage.h"
using namespace std;
#define MAX 3 // TBC I DONT GET IT int is 4 ptr is 8


class Node {
	bool isLeaf;
    int *key;
	int size;
	union Pointer{
		Node* ptr;
		reviewChain* rc;
	};
	vector<Pointer> ptr;
	friend class BPTree;
    
public:
	Node()
    {
        isLeaf = true;
	    key = new int[MAX];
	    ptr = vector<Pointer>(MAX + 1);
    }
};

union Pointer{
	Node* ptr;
	reviewChain* rc;
};

struct searchResults{
		queue<Node*> accessedNodesQueue;
		reviewChain* reviewResults;
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
	struct searchResults search(int,int);
	void insert(int, reviewAddress);
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
