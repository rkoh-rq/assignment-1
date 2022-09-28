#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include "storage.h"
using namespace std;

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
	Node(int m)
    {
        isLeaf = true;
	    key = (int *)malloc(sizeof(int) * m);
	    ptr = vector<Pointer>(m + 1);
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
	void insertInternal(Node*,Node*);
	int removeInternal(int,Node*,Node*);
	Node* findParent(Node*, Node*);
	vector<Node*> visited;
	int MAX;

public:
	BPTree(int m){
		MAX = m;
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
	int getParameterN(){
		return MAX;
	}
	Node* getRoot()
    {
	    return root;
    };
};
