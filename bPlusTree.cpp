#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;
int MAX = 3; // TBC I DONT GET IT int is 4 ptr is 8

struct record {
    int value;
    vector <void*> vector;
};

class Node {
	bool isLeaf;
    record *key;
	int size;  
	Node** ptr;
	friend class BPTree;
    
public:
	Node()
    {
        isLeaf = true;
	    key = new record[MAX];
	    ptr = new Node*[MAX + 1];
    }
};



// BP tree
class BPTree {
	Node* root;
	void insertInternal(int,Node*,Node*);
	void removeInternal(int,Node*,Node*);
	Node* findParent(Node*, Node*);

public:
	BPTree(){
        root = NULL;
    }
	void search(int,int);
	void insert(int);
	void remove(int);
	void display(Node*,int,int);
	Node* getRoot()
    {
	    return root;
    };
};

void BPTree::insert(int x)
{	
	
	// If root is null then return
	// newly created node
	if (root == NULL) {
		root = new Node;
		root->key[0].value = x;
		root->isLeaf = true;
		root->size = 1;
	}

	// Traverse the B+ Tree
	else {
		Node* cursor = root;
		Node* parent;

		// Till cursor reaches the
		// leaf node
		while (cursor->isLeaf == false) {

			parent = cursor;

			for (int i = 0; i < cursor->size; i++) {

				// If found the position
				// where we have to insert
				// node
				if (x < cursor->key[i].value) {
					cursor = cursor->ptr[i];
					break;
				}

				// If reaches the end
				if (i == cursor->size - 1) {
					cursor	= cursor->ptr[i + 1];
					break;
				}
			}
		}

		if (cursor->size < MAX) {
			int i = 0;
			while (x > cursor->key[i].value && i < cursor->size) {
				i++;
			}

			for (int j = cursor->size; j > i; j--) {
				cursor->key[j].value = cursor->key[j - 1].value;
			}
			
			cursor->key[i].value = x;
			cursor->size++;

			cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
			cursor->ptr[cursor->size - 1] = NULL;
		}

		else {

			// Create a newLeaf node
			Node* newLeaf = new Node;

			int virtualNode[MAX + 1];

			// Update cursor to virtual
			// node created
			for (int i = 0; i < MAX; i++) {
				virtualNode[i]	= cursor->key[i].value;
			}
			int i = 0, j;

			// Traverse to find where the new
			// node is to be inserted
			while (x > virtualNode[i] && i < MAX) {
				i++;
			}

			// Update the current virtual
			// Node to its previous
			for (int j = MAX + 1; j > i; j--) {
				virtualNode[j] = virtualNode[j - 1];
			}

			virtualNode[i] = x;
			newLeaf->isLeaf = true;

			cursor->size = (MAX + 1) / 2;
			newLeaf->size = MAX + 1 - (MAX + 1) / 2;

			cursor->ptr[cursor->size] = newLeaf;

			newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];

			cursor->ptr[MAX] = NULL;

			// Update the current virtual
			// Node's key to its previous
			for (i = 0; i < cursor->size; i++) {
				cursor->key[i].value = virtualNode[i];
			}

			// Update the newLeaf key to
			// virtual Node
			for (i = 0, j = cursor->size;i < newLeaf->size;i++, j++) {
				newLeaf->key[i].value = virtualNode[j];
			}

			// If cursor is the root node
			if (cursor == root) {

				// Create a new Node
				Node* newRoot = new Node;

				// Update rest field of
				// B+ Tree Node
				newRoot->key[0].value = newLeaf->key[0].value;
				newRoot->ptr[0] = cursor;
				newRoot->ptr[1] = newLeaf;
				newRoot->isLeaf = false;
				newRoot->size = 1;
				root = newRoot;
			}
			else {
				// Recursive Call for
				// insert in internal
				insertInternal(newLeaf->key[0].value, parent,newLeaf);
			}
		}
	}
}

void BPTree::search(int lowerBound, int upperBound)
{
    bool found = false;
	// If tree is empty
	if (root == NULL) {
		cout << "Tree is empty\n";
	}

	// Traverse to find the value
	else {

		Node* cursor = root;

		// Till we reach leaf node
		while (cursor->isLeaf == false) {

			for (int i = 0; i < cursor->size; i++) {

				// If the element to be
				// found is not present
				if (lowerBound < cursor->key[i].value) {
					cursor = cursor->ptr[i];
					break;
				}

				// If reaches end of the
				// cursor node
				if (i == cursor->size - 1) {
					cursor = cursor->ptr[i + 1];
					break;
				}
			}
		}

		// Traverse the cursor and find
		// the node with value x
		for (int i = 0; i < cursor->size; i++) {
            // TODO - change to allow for range queries
			// If found then return
			
			if (cursor->key[i].value >= lowerBound && cursor->key[i].value <= upperBound) {
				cout << cursor->key[i].value;
                cout << "\n";
                found = true;
			}
			if (cursor->key[i].value >= upperBound){
				break;
			}
			
			if (i == cursor->size - 1 && cursor->ptr[i+1] != nullptr){
				cursor = cursor->ptr[i+1];
				i = -1;
			}
		}
		// Else element is not present
        if (!found){
            cout << "Not found\n";
        }
	}
}



// Function to implement the Insert
// Internal Operation in B+ Tree
void BPTree::insertInternal(int x,Node* cursor, Node* child)
{

	// If we doesn't have overflow
	if (cursor->size < MAX) {
		int i = 0;
		// Traverse the child node
		// for current cursor node
		while (x > cursor->key[i].value && i < cursor->size) {
			i++;
		}

		// Traverse the cursor node
		// and update the current key
		// to its previous node key
		for (int j = cursor->size; j > i; j--) {

			cursor->key[j].value = cursor->key[j - 1].value;
		}

		// Traverse the cursor node
		// and update the current ptr
		// to its previous node ptr
		for (int j = cursor->size + 1; j > i + 1; j--) {
			cursor->ptr[j] = cursor->ptr[j - 1];
		}

		cursor->key[i].value = x;
		cursor->size++;
		cursor->ptr[i + 1] = child;
	}

	// For overflow, break the node
	else {

		// For new Interval
		Node* newInternal = new Node;
		int virtualKey[MAX + 1];
		Node* virtualPtr[MAX + 2];

		// Insert the current list key
		// of cursor node to virtualKey
		for (int i = 0; i < MAX; i++) {
			virtualKey[i] = cursor->key[i].value;
		}

		// Insert the current list ptr
		// of cursor node to virtualPtr
		for (int i = 0; i < MAX + 1; i++) {
			virtualPtr[i] = cursor->ptr[i];
		}

		int i = 0, j;

		// Traverse to find where the new
		// node is to be inserted
		while (x > virtualKey[i] && i < MAX) {
			i++;
		}

		// Traverse the virtualKey node
		// and update the current key
		// to its previous node key
		for (int j = MAX + 1;j > i; j--) {
            virtualKey[j] = virtualKey[j - 1];
		}

		virtualKey[i] = x;

		// Traverse the virtualKey node
		// and update the current ptr
		// to its previous node ptr
		for (int j = MAX + 2; j > i + 1; j--) {
			virtualPtr[j] = virtualPtr[j - 1];
		}

		virtualPtr[i + 1] = child;
		newInternal->isLeaf = false;

		cursor->size = (MAX + 1) / 2;

		newInternal->size = MAX - (MAX + 1) / 2;

		// Insert new node as an
		// internal node
		for (i = 0, j = cursor->size + 1;i < newInternal->size;i++, j++) {
            newInternal->key[i].value = virtualKey[j];
		}

		for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++) {
            newInternal->ptr[i]	= virtualPtr[j];
		}

		// If cursor is the root node
		if (cursor == root) {

			// Create a new root node
			Node* newRoot = new Node;

			// Update key value
			newRoot->key[0].value = cursor->key[cursor->size].value;

			// Update rest field of
			// B+ Tree Node
			newRoot->ptr[0] = cursor;
			newRoot->ptr[1] = newInternal;
			newRoot->isLeaf = false;
			newRoot->size = 1;
			root = newRoot;
		}

		else {

			// Recursive Call to insert
			// the data
			insertInternal(cursor->key[cursor->size].value, findParent(root,cursor),newInternal);
		}
	}
}

// Function to find the parent node
Node* BPTree::findParent(Node* cursor, Node* child)
{
	Node* parent;

	// If cursor reaches the end of Tree
	if (cursor->isLeaf || (cursor->ptr[0])->isLeaf) {
		return NULL;
	}

	// Traverse the current node with
	// all its child
	for (int i = 0; i < cursor->size + 1; i++) {

		// Update the parent for the
		// child Node
		if (cursor->ptr[i] == child) {
			parent = cursor;
			return parent;
		}

		// Else recursively traverse to
		// find child node
		else {
			parent = findParent(cursor->ptr[i], child);

			// If parent is found, then
			// return that parent node
			if (parent != NULL)
				return parent;
		}
	}
	// Return parent node
	return parent;
}
//TODO - make a remove, fix search and have a display

//remove
void BPTree::remove(int x)
{
	//if null return empty
	if(root==NULL)
	{
		cout<<"Tree empty\n";
		return;
	}

	else
	{
		Node* cursor = root;
		Node* parent;

		int leftSibling, rightSibling;
		//in the following while loop, cursor will travel to the leaf node possibly consisting the key
		while(cursor->isLeaf == false)
		{
			for(int i = 0; i < cursor->size; i++)
			{
				parent = cursor;
				leftSibling = i-1; //leftSibling is the index of left sibling in the parent node
				rightSibling =  i+1; //rightSibling is the index of right sibling in the parent node
				
				
				// If found the position
				// where we have to remove

				if(x < cursor->key[i].value)
				{
					cursor = cursor->ptr[i];
					break;
				}

				//if reaches the end
				if(i == cursor->size - 1)
				{
					leftSibling = i;
					rightSibling = i+2;
					cursor = cursor->ptr[i+1];
					break;
				}
			}
		}

		//in the following for loop, we search for the key if it exists
		bool found = false;
		int pos;
		for(pos = 0; pos < cursor->size; pos++)
		{
			if(cursor->key[pos].value == x)
			{
				found = true;
				break;
			}
		}
		if(!found) //if key does not exist in that leaf node
		{
			cout << "Key does not exist!";
			return;
		}

		//deleting the key
		for(int i = pos; i < cursor->size; i++)
		{
			cursor->key[i].value = cursor->key[i+1].value;
		}
		cursor->size--;
		if(cursor == root)//if it is root node, then make all pointers NULL
		{
			cout<<"Deleted "<< x << " " <<  "from leaf node successfully\n";
			for(int i = 0; i < MAX+1; i++)
			{
				cursor->ptr[i] = NULL;
			}
			if(cursor->size == 0)//if all keys are deleted
			{
				//cout<<"Tree is now empty\n";
				delete[] cursor->key;
				delete[] cursor->ptr;
				delete cursor;
				cout << "Deleted 1" <<"\n";
				root = NULL;
			}
			return;
		}
		cursor->ptr[cursor->size] = cursor->ptr[cursor->size+1];
		cursor->ptr[cursor->size+1] = NULL;
		cout<<"Deleted "<< x << " " <<" from leaf node successfully\n";
		if(cursor->size >= (MAX+1)/2)//no underflow
		{	
			for (int i = 0; i < parent->size; i++){
				if (parent->key[i].value == x){
					parent->key[i].value = cursor->key[0].value;
					cout<<"Deleted "<< x << " " <<" from internal node successfully\n";
					break;
				}
			}
			return;
		}

		//first we try to transfer a key from sibling node
		//check if left sibling exists
		if(leftSibling >= 0)
		{
			Node *leftNode = parent->ptr[leftSibling];
			//check if it is possible to transfer
			if(leftNode->size >= (MAX+1)/2+1)
			{
				//make space for transfer
				for(int i = cursor->size; i > 0; i--)
				{
					cursor->key[i].value = cursor->key[i-1].value;
				}
				//shift pointer to next leaf
				cursor->size++;
				cursor->ptr[cursor->size] = cursor->ptr[cursor->size-1];
				cursor->ptr[cursor->size-1] = NULL;
				//transfer
				cursor->key[0].value = leftNode->key[leftNode->size-1].value;
				//shift pointer of leftsibling
				leftNode->size--;
				leftNode->ptr[leftNode->size] = cursor;
				leftNode->ptr[leftNode->size+1] = NULL;
				//update parent
				parent->key[leftSibling].value = cursor->key[0].value;
				//cout<<"Transferred "<<cursor->key[0].key_value << " " << cursor->key[0].add_vect[0] <<" from left sibling of leaf node\n";
				return;
			}
		}
		if(rightSibling <= parent->size)//check if right sibling exist
		{
			Node *rightNode = parent->ptr[rightSibling];
			//check if it is possible to transfer
			if(rightNode->size >= (MAX+1)/2+1)
			{
				//shift pointer to next leaf
				cursor->size++;
				cursor->ptr[cursor->size] = cursor->ptr[cursor->size-1];
				cursor->ptr[cursor->size-1] = NULL;
				//transfer
				cursor->key[cursor->size-1].value = rightNode->key[0].value;
				//shift pointer of rightsibling
				rightNode->size--;
				rightNode->ptr[rightNode->size] = rightNode->ptr[rightNode->size+1];
				rightNode->ptr[rightNode->size+1] = NULL;
				//shift conent of right sibling
				for(int i = 0; i < rightNode->size; i++)
				{
					rightNode->key[i].value = rightNode->key[i+1].value;
				}
				//update parent
				parent->key[rightSibling-1].value = rightNode->key[0].value;
				//cout<<"Transferred "<<cursor->key[cursor->size-1].key_value <<" "<< cursor->key[cursor->size-1].add_vect[0] <<" from right sibling of leaf node\n";
				return;
			}
		}
		//must merge and delete a node
		if(leftSibling >= 0)//if left sibling exist
		{
			Node* leftNode = parent->ptr[leftSibling];
			// transfer all keys to leftsibling and then transfer pointer to next leaf node
			for(int i = leftNode->size, j = 0; j < cursor->size; i++, j++)
			{
				leftNode->key[i].value = cursor->key[j].value;
			}
			leftNode->ptr[leftNode->size] = NULL;
			leftNode->size += cursor->size;
			leftNode->ptr[leftNode->size] = cursor->ptr[cursor->size];

			//cout<<"Merging two leaf nodes\n";
			removeInternal(parent->key[leftSibling].value,parent,cursor);// delete parent node key
			//delete[] cursor->key;
			//delete[] cursor->ptr;
			//delete cursor;
			cout << "Deleted 1" <<"\n";
		}
		else if(rightSibling <= parent->size)//if right sibling exist
		{
			Node* rightNode = parent->ptr[rightSibling];
			// transfer all keys to cursor and then transfer pointer to next leaf node
			for(int i = cursor->size, j = 0; j < rightNode->size; i++, j++)
			{
				cursor->key[i].value = rightNode->key[j].value;
			}
			cursor->ptr[cursor->size] = NULL;
			cursor->size += rightNode->size;
			cursor->ptr[cursor->size] = rightNode->ptr[rightNode->size];
			//cout<<"Merging two leaf nodes\n";
			removeInternal(parent->key[rightSibling-1].value,parent,rightNode);// delete parent node key
			//delete[] rightNode->key; 
			//delete[] rightNode->ptr;
			//delete rightNode;
			cout << "Deleted 1 \n";
		}
	}
}


void BPTree::removeInternal(int x, Node* cursor, Node* child)
{
	//deleting the key x first
	//checking if key from root is to be deleted
	if(cursor == root)
	{
		if(cursor->size == 1)//if only one key is left, change root
		{
			if(cursor->ptr[1] == child)
			{
				delete[] child->key;
				delete[] child->ptr;
				delete child;
				cout << "Deleted 1" <<"\n";
				root = cursor->ptr[0];
				delete[] cursor->key;
				delete[] cursor->ptr;
				delete cursor;
				cout << "Deleted 1" <<"\n";
				//cout<<"Changed root node\n";
				return;
			}
			else if(cursor->ptr[0] == child)
			{
				delete[] child->key;
				delete[] child->ptr;
				delete child;
				cout << "Deleted 1" <<"\n";
				root = cursor->ptr[1];
				delete[] cursor->key;
				delete[] cursor->ptr;
				delete cursor;
				cout << "Deleted 1" <<"\n";
				//cout<<"Changed root node\n";
				return;
			}
		}
	}
	int pos;
	for(pos = 0; pos < cursor->size; pos++)
	{
		if(cursor->key[pos].value == x)
		{
			break;
		}
	}
	for(int i = pos; i < cursor->size; i++)
	{
		cursor->key[i].value = cursor->key[i+1].value;
	}
	//now deleting the pointer child
	for(pos = 0; pos < cursor->size+1; pos++)
	{
		if(cursor->ptr[pos] == child)
		{
			break;
		}
	}
	for(int i = pos; i < cursor->size+1; i++)
	{
		cursor->ptr[i] = cursor->ptr[i+1];
	}
	cursor->size--;
	if(cursor->size >= (MAX+1)/2-1)//no underflow
	{
		cout<<"Deleted "<< x << " " <<" from internal node successfully\n";
		return;
	}
	//cout<<"Underflow in internal node!\n";
	//underflow, try to transfer first
	if(cursor==root)return;
	Node* parent = findParent(root, cursor);
	int leftSibling, rightSibling;
	//finding left n right sibling of cursor
	for(pos = 0; pos < parent->size+1; pos++)
	{
		if(parent->ptr[pos] == cursor)
		{
			leftSibling = pos - 1;
			rightSibling = pos + 1;
			break;
		}
	}
	//try to transfer
	if(leftSibling >= 0)//if left sibling exists
	{
		Node *leftNode = parent->ptr[leftSibling];
		//check if it is possible to transfer
		if(leftNode->size >= (MAX+1)/2)
		{
			//make space for transfer of key
			for(int i = cursor->size; i > 0; i--)
			{
				cursor->key[i].value = cursor->key[i-1].value;
			}
			//transfer key from left sibling through parent
			cursor->key[0].value = parent->key[leftSibling].value;
			parent->key[leftSibling].value = leftNode->key[leftNode->size-1].value;
			//transfer last pointer from leftnode to cursor
			//make space for transfer of ptr
			for (int i = cursor->size+1; i > 0; i--)
			{
				cursor->ptr[i] = cursor->ptr[i-1];
			}
			//transfer ptr
			cursor->ptr[0] = leftNode->ptr[leftNode->size];
			cursor->size++;
			leftNode->size--;
			//cout<<"Transferred "<<cursor->key[0].key_value<<" "<< x.add_vect[0] <<" from left sibling of internal node\n";
			return;
		}
	}
	if(rightSibling <= parent->size)//check if right sibling exist
	{
		Node *rightNode = parent->ptr[rightSibling];
		//check if it is possible to transfer
		if(rightNode->size >= (MAX+1)/2)
		{
			//transfer key from right sibling through parent
			cursor->key[cursor->size].value = parent->key[pos].value;
			parent->key[pos].value = rightNode->key[0].value;
			for (int i = 0; i < rightNode->size -1; i++)
			{
				rightNode->key[i].value = rightNode->key[i+1].value;
			}
			//transfer first pointer from rightnode to cursor
			//transfer ptr
			cursor->ptr[cursor->size+1] = rightNode->ptr[0];
			for (int i = 0; i < rightNode->size; ++i)
			{
				rightNode->ptr[i] = rightNode->ptr[i+1];
			}
			cursor->size++;
			rightNode->size--;
			//cout<<"Transferred "<<cursor->key[0].key_value << " " << x.add_vect[0] <<" from right sibling of internal node\n";
			return;
		}
	}
	//transfer wasnt posssible hence do merging
	if(leftSibling >= 0)
	{
		//leftnode + parent key + cursor
		Node *leftNode = parent->ptr[leftSibling];
		leftNode->key[leftNode->size].value = parent->key[leftSibling].value;
		for(int i = leftNode->size+1, j = 0; j < cursor->size; j++)
		{
			leftNode->key[i].value = cursor->key[j].value;
		}
		for(int i = leftNode->size+1, j = 0; j < cursor->size+1; j++)
		{
			leftNode->ptr[i] = cursor->ptr[j];
			cursor->ptr[j] = NULL;
		}
		leftNode->size += cursor->size+1;
		cursor->size = 0;
		//delete cursor
		removeInternal(parent->key[leftSibling].value, parent, cursor);
		//cout<<"Merged with left sibling\n";

	}
	else if(rightSibling <= parent->size)
	{
		//cursor + parent key + rightnode
		Node *rightNode = parent->ptr[rightSibling];
		cursor->key[cursor->size].value = parent->key[rightSibling-1].value;
		for(int i = cursor->size+1, j = 0; j < rightNode->size; j++)
		{
			cursor->key[i].value = rightNode->key[j].value;
		}
		for(int i = cursor->size+1, j = 0; j < rightNode->size+1; j++)
		{
			cursor->ptr[i] = rightNode->ptr[j];
			rightNode->ptr[j] = NULL;
		}
		cursor->size += rightNode->size+1;
		rightNode->size = 0;
		//delete cursor
		removeInternal(parent->key[rightSibling-1].value, parent, rightNode);
		//cout<<"Merged with right sibling\n";
	}
}


//display
void BPTree::display(Node* cursor, int level, int child) //child to be hardcoded as zero?
{
	if (cursor != NULL and level >= 0)
	{
		if (child == 0)
			cout << "Content of Root Node = ";
		else
			cout << "Content of " << child << " Child Node = ";

		for (int i = 0; i < cursor->size; i++)
		{ 
			cout << cursor->key[i].value << " ";
		}
		cout << "\n";
		if (cursor->isLeaf != true)
		{
			for (int i = 0; i < cursor->size + 1; i++)
			{
				display(cursor->ptr[i], --level, ++child);
			}
		}
	}
}





// Driver Code
int main()
{   
    BPTree node;
	// Create B+ Tree
	node.insert(6);
	node.insert(16);
	node.insert(26);

	node.display(node.getRoot(),8,0);

	node.insert(36);
	node.insert(46);
	// Function Call to search node
	// with value 16
	node.search(6,46);
	node.display(node.getRoot(),8,0);
	//node.display(node.getRoot(),1,0);

	node.remove(6);
	node.display(node.getRoot(),8,0);
	node.remove(26);
	node.display(node.getRoot(),8,0);
	return 0;
  	/*node.insert(5);
  	node.insert(15);
  	node.insert(25);
  	node.insert(35);
  	node.insert(45);

  	node.display(node.getRoot(),8,0);
	node.search(35,35);

  	node.remove(25);

  	node.display(node.getRoot(),8,0);*/
}
