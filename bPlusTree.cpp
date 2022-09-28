#include "bPlusTree.h"

void BPTree::insert(int x) 
{	
	
	// If root is null then return
	// newly created node
	if (root == NULL) {
		root = new Node;
		root->key[0] = x;
		root->isLeaf = true;
		root->size = 1;
		cout << "Key " << x << " successfully inserted!" <<endl;
	}

	// Traverse the B+ Tree
	else {
		Node* cursor = root;
		Node* parent;

		// //dup checking THIS IS NOT COMPLETED, INTEGRATION TEAM TO SEE IF NEEDED
		// queue<Node*> q = search(x,x);
		// Node *lastAccessedNode = q.back();
        //     if ( lastAccessedNode != nullptr){
        //         for(int i = 0; i < lastAccessedNode->size; i++)
        //         {
        //             if(lastAccessedNode->key[i] == x)
        //             {
        //                 lastAccessedNode->key[i].dup_vector.push_back(x);
        //                 break;
        //             }
        //         }
        //         return;
        //     }


		// Till cursor reaches the
		// leaf node
		while (cursor->isLeaf == false) {

			parent = cursor;
			for (int i = 0; i < cursor->size; i++) {

				// If found the position
				// where we have to insert
				// node
					cursor = cursor->ptr[i];
				if (x < cursor->key[i]) {
					break;
				}

				// If reaches the end
				if (i == cursor->size - 1) {
					cursor	= cursor->ptr[i + 1];
					break;
				}
			}
		}

		//integration team
		// Check for duplicate Keys case - TODO add in the extra pointer to the storage space
		for (int i = 0; i < cursor->size; i++) {
				//TODO link to the storage blk idk
				//sth like
					// cursor->key[i].add_vector.push_back(revAddress) 
			if (cursor->key[i] == x) {
				return;
			}
		}

		if (cursor->size < MAX) {
			int i = 0;
			while (x > cursor->key[i] && i < cursor->size) {
				i++;
			}
			

			for (int j = cursor->size; j > i; j--) {
				cursor->key[j] = cursor->key[j - 1];
			}
			
			cursor->key[i] = x;
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
				virtualNode[i]	= cursor->key[i];
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
				cursor->key[i] = virtualNode[i];
			}

			// Update the newLeaf key to
			// virtual Node
			for (i = 0, j = cursor->size;i < newLeaf->size;i++, j++) {
				newLeaf->key[i] = virtualNode[j];
			}

			// If cursor is the root node
			if (cursor == root) {

				// Create a new Node
				Node* newRoot = new Node;

				// Update rest field of
				// B+ Tree Node
				newRoot->ptr[0] = cursor;
				newRoot->ptr[1] = newLeaf;
				newRoot->key[0] = newLeaf->key[0];
				newRoot->isLeaf = false;
				newRoot->size = 1;
				root = newRoot;
			}
			else {
				// Recursive Call for
				// insert in internal
				insertInternal(newLeaf->key[0], parent,newLeaf);
			}
		}

		cout << "Key " << x << " successfully inserted!" <<endl;
	}
}

//integration team to update
struct returnResults{
		queue<Node*> accessedNodesQueue;
		// reviewAddress revAdd; // to be impl
};

returnResults BPTree::search(int lowerBound, int upperBound)
{
	struct returnResults res;
    bool found = false;
	int numAccessed = 0;

	// If tree is empty
	if (root == NULL) {
		cout << "Tree is empty\n";
	}

	//If tree is not empty
	// Traverse to find the value
	else {

		Node* cursor = root;
		// Traversing internal nodes till we reach leaf node
		while (!cursor->isLeaf) {
			
			//Counting and pushing nodes accessed to queue
				//case for traversing internal nodes
			numAccessed++;
			res.accessedNodesQueue.push(cursor);

			//for each key in the cursor
			for (int i = 0; i < cursor->size; i++) {
				
				// If the element to be
				// found is in left subtree
					cursor = cursor->ptr[i];
				if (lowerBound < cursor->key[i]) {

					if (cursor->isLeaf){
						//Counting and pushing nodes accessed to queue
							//case for 1st leaf node reached
						numAccessed++;
						res.accessedNodesQueue.push(cursor);
					}
					break;
				}

				// If reaches end of the
				// cursor node, ie the last key
				// Go to rightmost child
				if (i == cursor->size - 1) {
					cursor = cursor->ptr[i + 1];

					if (cursor->isLeaf){
						//Counting and pushing nodes accessed to queue
							//case for 1st leaf node reached
						numAccessed++;
						res.accessedNodesQueue.push(cursor);
					}
					break;
				}
			}
		}

		// Traverse the cursor and find
		// the node with value x
		for (int i = 0; i < cursor->size; i++) {
            // TODO - change to allow for range queries
			// If found then return
			
			if (cursor->key[i] >= lowerBound && cursor->key[i] <= upperBound) {
				// TODO For integration - it wouldnt be found key here but instead it would return to the data block
				cout << "Found Key: " << cursor->key[i] << endl;
                found = true;
			}
			if (cursor->key[i] >= upperBound){
				break;
			}
			
			//traverse neighbouring leaf nodes
			if (i == cursor->size - 1 && cursor->ptr[i+1] != nullptr){
				cursor = cursor->ptr[i+1];
				i = -1;

				//Counting and pushing nodes accessed to queue
					//case for traversing neighbouring leaf ndoes
				numAccessed++;
				res.accessedNodesQueue.push(cursor);
			}
		}
		// Else element is not present
        if (!found){
            cout << "Not found\n";
        }
	}
	return res;
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
		while (x > cursor->key[i] && i < cursor->size) {
			i++;
		}

		// Traverse the cursor node
		// and update the current key
		// to its previous node key
		for (int j = cursor->size; j > i; j--) {

			cursor->key[j] = cursor->key[j - 1];
		}

		// Traverse the cursor node
		// and update the current ptr
		// to its previous node ptr
		for (int j = cursor->size + 1; j > i + 1; j--) {
			cursor->ptr[j] = cursor->ptr[j - 1];
		}

		cursor->key[i] = x;
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
			virtualKey[i] = cursor->key[i];
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
            newInternal->key[i] = virtualKey[j];
		}

		for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++) {
            newInternal->ptr[i]	= virtualPtr[j];
		}

		// If cursor is the root node
		if (cursor == root) {
			
			// Create a new root node
			Node* newRoot = new Node;
			Node* newInternalChild = newInternal;
			// Update key value
			while (newInternalChild->isLeaf == false){
				newInternalChild = newInternalChild->ptr[0];
			}
			newRoot->key[0] = newInternalChild->key[0];

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
			insertInternal(cursor->key[cursor->size], findParent(root,cursor),newInternal);
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
//remove
int BPTree::remove(int x)
{
	int numOfDeletedNodes = 0;

	//if null return empty
	if(root==NULL)
	{
		cout<<"Tree empty\n";
		return numOfDeletedNodes;
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

				if(x < cursor->key[i])
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
			if(cursor->key[pos] == x)
			{
				found = true;
				break;
			}
		}
		if(!found) //if key does not exist in that leaf node
		{
			cout << "Key does not exist!";
			return numOfDeletedNodes;
		}

		//deleting the key
		for(int i = pos; i < cursor->size; i++)
		{
			cursor->key[i] = cursor->key[i+1];
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
				numOfDeletedNodes++;
				cout << "Deleted one node" <<"\n";
				root = NULL;
			}
			return numOfDeletedNodes;
		}
		cursor->ptr[cursor->size] = cursor->ptr[cursor->size+1];
		cursor->ptr[cursor->size+1] = NULL;
		cout<<"Deleted "<< x << " " <<" from leaf node successfully\n";
		if(cursor->size >= (MAX+1)/2)//no underflow
		{
			for (int i = 0; i < parent->size; i++){
					if (parent->key[i] == x){
							parent->key[i] = cursor->key[0];
							cout<<"Deleted "<< x << " " <<" from internal node successfully\n";
							break;
					}
			}
			return numOfDeletedNodes;
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
					cursor->key[i] = cursor->key[i-1];
				}
				//shift pointer to next leaf
				cursor->size++;
				cursor->ptr[cursor->size] = cursor->ptr[cursor->size-1];
				cursor->ptr[cursor->size-1] = NULL;
				//transfer
				cursor->key[0] = leftNode->key[leftNode->size-1];
				//shift pointer of leftsibling
				leftNode->size--;
				leftNode->ptr[leftNode->size] = cursor;
				leftNode->ptr[leftNode->size+1] = NULL;
				//update parent
				parent->key[leftSibling] = cursor->key[0];
				//cout<<"Transferred "<<cursor->key[0].key_value << " " << cursor->key[0].add_vect[0] <<" from left sibling of leaf node\n";
				return numOfDeletedNodes;
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
				cursor->key[cursor->size-1] = rightNode->key[0];
				//shift pointer of rightsibling
				rightNode->size--;
				rightNode->ptr[rightNode->size] = rightNode->ptr[rightNode->size+1];
				rightNode->ptr[rightNode->size+1] = NULL;
				//shift conent of right sibling
				for(int i = 0; i < rightNode->size; i++)
				{
					rightNode->key[i] = rightNode->key[i+1];
				}
				//update parent
				parent->key[rightSibling-1] = rightNode->key[0];
				//cout<<"Transferred "<<cursor->key[cursor->size-1].key_value <<" "<< cursor->key[cursor->size-1].add_vect[0] <<" from right sibling of leaf node\n";
				return numOfDeletedNodes;
			}
		}
		//must merge and delete a node
		if(leftSibling >= 0)//if left sibling exist
		{
			Node* leftNode = parent->ptr[leftSibling];
			// transfer all keys to leftsibling and then transfer pointer to next leaf node
			for(int i = leftNode->size, j = 0; j < cursor->size; i++, j++)
			{
				leftNode->key[i] = cursor->key[j];
			}
			leftNode->ptr[leftNode->size] = NULL;
			leftNode->size += cursor->size;
			leftNode->ptr[leftNode->size] = cursor->ptr[cursor->size];

			//cout<<"Merging two leaf nodes\n";
			int addDeletedNodes = removeInternal(parent->key[leftSibling],parent,cursor);// delete parent node key
			numOfDeletedNodes += addDeletedNodes;
			// delete[] cursor->key;
			// delete[] cursor->ptr;
			// delete cursor;
			numOfDeletedNodes++;
			cout << "Deleted one node" <<"\n";
		}
		else if(rightSibling <= parent->size)//if right sibling exist
		{
			Node* rightNode = parent->ptr[rightSibling];
			// transfer all keys to cursor and then transfer pointer to next leaf node
			for(int i = cursor->size, j = 0; j < rightNode->size; i++, j++)
			{
				cursor->key[i] = rightNode->key[j];
			}
			cursor->ptr[cursor->size] = NULL;
			cursor->size += rightNode->size;
			cursor->ptr[cursor->size] = rightNode->ptr[rightNode->size];
			//cout<<"Merging two leaf nodes\n";
			int addDeletedNodes = removeInternal(parent->key[rightSibling-1],parent,rightNode);// delete parent node key
			numOfDeletedNodes += addDeletedNodes;
			// delete[] rightNode->key;
			// delete[] rightNode->ptr;
			// delete rightNode;
			numOfDeletedNodes++;
			cout << "Deleted one node \n";
		}
	}
	return numOfDeletedNodes;
}


int BPTree::removeInternal(int x, Node* cursor, Node* child)
{
	int addDeletedNodes = 0; 
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
				addDeletedNodes++;
				cout << "Deleted one node" <<"\n";
				root = cursor->ptr[0];
				delete[] cursor->key;
				delete[] cursor->ptr;
				delete cursor;
				addDeletedNodes++;
				cout << "Deleted one node" <<"\n";
				//cout<<"Changed root node\n";
				return addDeletedNodes;
			}
			else if(cursor->ptr[0] == child)
			{
				delete[] child->key;
				delete[] child->ptr;
				delete child;
				addDeletedNodes++;
				cout << "Deleted one node" <<"\n";
				root = cursor->ptr[1];
				delete[] cursor->key;
				delete[] cursor->ptr;
				delete cursor;
				addDeletedNodes++;
				cout << "Deleted one node" <<"\n";
				//cout<<"Changed root node\n";
				return addDeletedNodes;
			}
		}
	}
	int pos;
	for(pos = 0; pos < cursor->size; pos++)
	{
		if(cursor->key[pos] == x)
		{
			break;
		}
	}
	for(int i = pos; i < cursor->size; i++)
	{
		cursor->key[i] = cursor->key[i+1];
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
		return addDeletedNodes;
	}
	//cout<<"Underflow in internal node!\n";
	//underflow, try to transfer first
	if( cursor == root )return addDeletedNodes;

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
				cursor->key[i] = cursor->key[i-1];
			}
			//transfer key from left sibling through parent
			cursor->key[0] = parent->key[leftSibling];
			parent->key[leftSibling] = leftNode->key[leftNode->size-1];
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
			return addDeletedNodes;
		}
	}
	if(rightSibling <= parent->size)//check if right sibling exist
	{
		Node *rightNode = parent->ptr[rightSibling];
		//check if it is possible to transfer
		if(rightNode->size >= (MAX+1)/2)
		{
			//transfer key from right sibling through parent
			cursor->key[cursor->size] = parent->key[pos];
			parent->key[pos] = rightNode->key[0];
			for (int i = 0; i < rightNode->size -1; i++)
			{
				rightNode->key[i] = rightNode->key[i+1];
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
			return addDeletedNodes;
		}
	}
	//transfer wasnt posssible hence do merging
	if(leftSibling >= 0)
	{
		//leftnode + parent key + cursor
		Node *leftNode = parent->ptr[leftSibling];
		leftNode->key[leftNode->size] = parent->key[leftSibling];
		for(int i = leftNode->size+1, j = 0; j < cursor->size; j++)
		{
			leftNode->key[i] = cursor->key[j];
		}
		for(int i = leftNode->size+1, j = 0; j < cursor->size+1; j++)
		{
			leftNode->ptr[i] = cursor->ptr[j];
			cursor->ptr[j] = NULL;
		}
		leftNode->size += cursor->size+1;
		cursor->size = 0;
		//delete cursor
		addDeletedNodes += removeInternal(parent->key[leftSibling], parent, cursor);
		//cout<<"Merged with left sibling\n";

	}
	else if(rightSibling <= parent->size)
	{
		//cursor + parent key + rightnode
		Node *rightNode = parent->ptr[rightSibling];
		cursor->key[cursor->size] = parent->key[rightSibling-1];
		for(int i = cursor->size+1, j = 0; j < rightNode->size; j++)
		{
			cursor->key[i] = rightNode->key[j];
		}
		for(int i = cursor->size+1, j = 0; j < rightNode->size+1; j++)
		{
			cursor->ptr[i] = rightNode->ptr[j];
			rightNode->ptr[j] = NULL;
		}
		cursor->size += rightNode->size+1;
		rightNode->size = 0;
		//delete cursor
		addDeletedNodes += removeInternal(parent->key[rightSibling-1], parent, rightNode);
		//cout<<"Merged with right sibling\n";
	}
	return addDeletedNodes;
}


/* Display level by level
*/
void BPTree::display(Node* cursor) 
{
	if (cursor == NULL) 
		return;

	queue<Node*> q;
	q.push(cursor);

	while (!q.empty()) {

		int queueSize = q.size();

		//for each level
		for (int i = 0; i < queueSize; i++) {

			Node* temp = q.front(); 
			q.pop();

			//displaying a node
			for (int j = 0; j < temp->size; j++)
			{ 
				cout << temp->key[j] << " ";
			}

			cout << "|| ";//to seperate next neighbouring nodes
			
			//if internal node, push child node
			if (!temp->isLeaf) {

				for (int k = 0; k <= temp->size; k++)
				{
					q.push(temp->ptr[k]);
				}
			}
		}
		cout << endl;
	}
}

/* Displat root node & its 1st child (pt 3)
*/
void BPTree::displayRootFirstChild(Node* cursor){

	//assuming node passed in is always root node
	Node* root = cursor;

	if (cursor == NULL) 
		return;

	queue<Node*> q;
	q.push(cursor);

	while (!q.empty()) {

		Node* temp = q.front(); 
		q.pop();

		//displaying a node
		for (int j = 0; j < temp->size; j++){ 
			cout << temp->key[j] << " ";
		}
		cout << "|| ";//to seperate next neighbouring nodes

		//push once (ie the 1st child node)
		if(temp == root && temp->ptr[0] != nullptr)
			q.push(temp->ptr[0]);

		cout << endl;
	}
}

/* Displaying all the key values in a single node */
void BPTree::displayAccessedNodes(queue<Node*> q){

	cout<<"Total Number of Nodes Accessed: " << q.size()<<endl;

	cout<<"Displaying up to first 5 Nodes Accessed:\n";

	int i = 0; 
	while(!q.empty()  && i < 5){

		Node* temp = q.front();
		q.pop();
		i++;

		for (int j = 0; j < temp->size; j++)
		{ 
			cout << temp->key[j] << " ";
		}
		cout << " || \n";
	}
	
}

int BPTree::getHeight(Node* cursor){
	if (cursor == NULL) return 0;

	int height = 1;
	
	while (!cursor->isLeaf){
		cursor = cursor->ptr[0];
		height++;
	}

	return height;
}

int BPTree::getNumberOfNodes(Node* cursor){
	
	if (cursor == NULL) 
		return 0;

	queue<Node*> q;
	q.push(cursor);
	int numOfNodes = 0;

	while (!q.empty()) {

		int queueSize = q.size();

		//for each level
		for (int i = 0; i < queueSize; i++) {

			Node* temp = q.front(); 
			q.pop();
			numOfNodes++;

			if (!temp->isLeaf) {

				for (int k = 0; k <= temp->size; k++)
				{
					q.push(temp->ptr[k]);
				}
			}
		}
	}
	return numOfNodes;
}


// // Driver Code
// int main()
// {   
//     BPTree node;
// 	cout<<"Size of BPTree: " << sizeof(node)<<" bytes";
// 	// //Initialising B+ Tree
// 	// cout<<"==Initialising B+Tree (6,16,26)==\n";
// 	// node.insert(6);
// 	// node.insert(16);
// 	// node.insert(26);
// 	// node.display(node.getRoot());
// 	// cout <<"Height: " << node.getHeight(node.getRoot()) <<" Num of Nodes: " << node.getNumberOfNodes(node.getRoot());

// 	// //Inserting keys
// 	// cout<<"\n==Inserting key 36==\n";
// 	// node.insert(36);
// 	// node.display(node.getRoot());
// 	// cout <<"Height: " << node.getHeight(node.getRoot()) <<" Num of Nodes: " << node.getNumberOfNodes(node.getRoot());

// 	// cout<<"\n==Inserting key 46==\n";
// 	// node.insert(46);
// 	// node.display(node.getRoot());
// 	// cout <<"Height: " << node.getHeight(node.getRoot()) <<" Num of Nodes: " << node.getNumberOfNodes(node.getRoot());

// 	// //Display root node only
// 	// cout<<"\n==Display root node and 1st child node only==\n";
// 	// node.displayRootFirstChild(node.getRoot());

// 	// // Search keys
// 	// cout<<"\n==Find key 16==\n";
// 	// node.search(16,16);

// 	// cout<<"\n==Find keys from range 6-47==\n";
// 	// node.search(6,47);

// 	// //Remove keys
// 	// cout<<"\n==Remove key 6==\n";	
// 	// node.remove(6);
// 	// node.display(node.getRoot());

// 	// cout<<"\n==Remove key 26==\n";		
// 	// node.remove(26);
// 	// node.display(node.getRoot());

// 	cout<<"\n==Testing==\n";
// 	node.insert(1);
// 	node.insert(4);
// 	node.insert(7);
// 	node.insert(10);
// 	// node.insert(17);
// 	// node.insert(19);
// 	node.insert(20);
// 	node.insert(21);
// 	node.insert(25);
// 	// node.insert(28);
// 	node.insert(31);
// 	// node.insert(5);
// 	// node.insert(16);

// 	// node.insert(6);
// 	// node.insert(16);
// 	node.insert(26);
// 	node.insert(36);

// 	//display B+Tree
// 	cout<<"B+Tree:\n";
// 	node.display(node.getRoot());

// 	//display height and num of nodes of B+Tree
// 	cout <<"Height: " << node.getHeight(node.getRoot()) <<" Num of Nodes: " << node.getNumberOfNodes(node.getRoot()) << "\n";
	
// 	// //test 1: displaying accessed nodes
// 	// queue<Node*> q = node.search(5,20);
// 	// node.displayAccessedNodes(q);

// 	// //test 2: displaying accessed nodes
// 	// queue<Node*> q = node.search(20,25);
// 	// node.displayAccessedNodes(q);

// 	//test 3: displaying accessed nodes
// 	struct returnResults res = node.search(0,9);
// 	node.displayAccessedNodes(res.accessedNodesQueue);

// 	// //test 4: deleting nodes
// 	// cout <<"Height: " << node.getHeight(node.getRoot()) <<" Num of Nodes: " << node.getNumberOfNodes(node.getRoot()) << "\n";
// 	// int numOfDeletedNodes = node.remove(36);
// 	// cout << "Number of Deleted Nodes: " << numOfDeletedNodes << endl;
// 	// cout <<"New Height: " << node.getHeight(node.getRoot()) <<" New Num of Nodes: " << node.getNumberOfNodes(node.getRoot()) << "\n";
// 	// node.display(node.getRoot());

// 	// // cout <<"Height: " << node.getHeight(node.getRoot()) <<" Num of Nodes: " << node.getNumberOfNodes(node.getRoot()) << "\n";
// 	// // numOfDeletedNodes = node.remove(26);
// 	// // cout << "Number of Deleted Nodes: " << numOfDeletedNodes << endl;
// 	// // cout <<"New Height: " << node.getHeight(node.getRoot()) <<" New Num of Nodes: " << node.getNumberOfNodes(node.getRoot()) << "\n";
// 	// // node.display(node.getRoot());

// 	// cout <<"Height: " << node.getHeight(node.getRoot()) <<" Num of Nodes: " << node.getNumberOfNodes(node.getRoot()) << "\n";
// 	// numOfDeletedNodes = node.remove(20);
// 	// cout << "Number of Deleted Nodes: " << numOfDeletedNodes << endl;
// 	// cout <<"New Height: " << node.getHeight(node.getRoot()) <<" New Num of Nodes: " << node.getNumberOfNodes(node.getRoot()) << "\n";
// 	// node.display(node.getRoot());

// 	// //size of node
// 	// cout<<"Size of 1 Node: " << sizeof(node)<<" bytes\n";
// 	// cout<<"size: "<<sizeof(BPTree)<<"bytes";
	
// 	// node.remove(36);
// 	// node.display(node.getRoot());
// 	// cout <<"Height: " << node.getHeight(node.getRoot()) <<" Num of Nodes: " << node.getNumberOfNodes(node.getRoot()) << "\n";

// 	//node.displayRootFirstChild(node.getRoot());

// 	return 0;
// }
