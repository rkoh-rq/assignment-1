#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
using namespace std;
#define MAX 3 // TBC I DONT GET IT int is 4 ptr is 8

struct key_struct {
    int value;
    vector <void*> add_vector; //to hold the "record ptrs"
};


class Node {
	bool isLeaf;
    key_struct *key;
	int size;  
	Node** ptr;
	friend class BPTree;
    
public:
	Node()
    {
        isLeaf = true;
	    key = new key_struct[MAX];
	    ptr = new Node*[MAX + 1];
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
