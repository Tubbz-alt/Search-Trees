/*
* Matthew Buchanan
* CS-340
* Project 2: Binary Search Trees and Red-Black Trees
*/

#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <cassert>
using namespace std;

/* Node structure used by both tree types */
struct Node
{
	string key = "ZZZ";
	bool color = 0; // 0 = black, 1 = red ignore for BST trees
	Node * x = NULL;
	Node * y = NULL;
	Node * p = NULL;// ignore for BST trees
};


/* Binary Search Tree class */
class BST {
public:
	Node * root;

	BST()
	{
		root = NULL;
	};

	~BST()
	{
	};

	/* Create a new node with key value k */
	Node * newNode(string k)
	{
		Node * newNode = new Node;
		newNode->key = k;
		return newNode;
	}

	/*void deleteNode(Node * z)
	{
		delete z;
	}*/

	/* Create a binary search tree of strings from an input file */
	void buildTree(string f)
	{
		ifstream in;
		in.open(f);
		assert(in.is_open());
		while (!in.eof())
		{
			string temp;
			in >> temp;
			insert(newNode(temp));
		}
		in.close();
		in.clear();
		return;
	}

	/* Insert a node into the BST */
	void insert(Node *z)
	{
		z->x = root;
		z->y = NULL;
		if (root == NULL)
		{
			root = z;
			z->x = NULL;
			return;
		}
		while (z->x != NULL)
		{
			z->y = z->x;
			if (z->key < z->x->key)
			{
				if (z->x->x)
					z->x = z->x->x;
				else
				{
					z->x->x = z;
					z->x = z->y = NULL;
				}
			}
			else
			{
				if (z->x->y)
					z->x = z->x->y;
				else
				{
					z->x->y = z;
					z->x = z->y = NULL;
				}
			}
		}
	}

	/* Recursively traverse the tree */
	void inOrderWalk(Node *r)
	{
		if (r != NULL)
		{
			inOrderWalk(r->x);
			cout << r->key << endl;
			inOrderWalk(r->y);
		}
	}

	/* Iterative method to search the tree for a string and return true or false */
	bool search(Node *z, string k)
	{
		while (z && k != z->key)
		{
			if (k < z->key)
			{
				if (z->x)
					z = z->x;
				else
					return false;
			}
			else
			{
				if (z->y)
					z = z->y;
				else
					return false;
			}
		}
		if (z)
			return true;
		else
			return false;
	}
};

/* Red Black Tree class */
class RBT {
public:
	Node * root;

	RBT() 
	{
		root = NULL;
	};

	~RBT()
	{
	}

	/* Create a new node with key value k */
	Node * newNode(string k)
	{
		Node * newNode = new Node;
		newNode->key = k;
		return newNode;
	}

	/* Create a Red-Black tree of strings from an input file */
	void buildTree(string f)
	{
		ifstream in;
		in.open(f);
		assert(in.is_open());
		while (!in.eof())
		{
			string temp;
			in >> temp;
			insert(newNode(temp));
		}
		in.close();
		in.clear();
		return;
	}

	/* Insert new node into the tree */
	void insert(Node *z)
	{
		z->x = root;							//initialize pointer x to traverse tree
		z->y = NULL;
		if (root == NULL)					//check for empty tree
		{
			root = z;						//node z is root
			z->x = z->y = NULL;
			return;
		}
		while (z->x != NULL)
		{
			z->y = z->x;
			if (z->key < z->x->key)		//less than condition
			{
				if (z->x->x)		//Traverse down left subtree of
					z->x = z->x->x;
				else					// Insert node z as leaf
				{
					z->x->x = z;
					z->p = z->x;
					z->x = z->y = NULL;
				}
			}
			else
			{
				if (z->x->y)		//Traverse down right subtree x points at
					z->x = z->x->y;
				else                    // Insert node z as leaf
				{
					z->x->y = z;
					z->p = z->x;
					z->x = z->y = NULL;
				}
			}
		}
		z->color = 1;
		insertFixUp(z);
	}

	/* Fix Red/Red violations caused by insertion */
	void insertFixUp(Node *z)
	{
		Node * y;
		while (z->p && z->p->color)//while z is not root and z's parent is red
		{
			y = z->p->p;
			if (y->x == z->p)//if z's parent is a left child
			{
				if (y->y && y->y->color)//if z has an uncle and the uncle is a red right child
				{
					z->p->color = y->y->color = 0;//color z's parent and uncle black
					y->color = 1;//color z's grandparent red
					z = y;//move z pointer up the tree to z.p.p
				}
				else
				{
					if (z == z->p->y)//if z is a right child
					{
						z = z->p;//move z up the tree and left rotate
						leftRotate(z);
					}
					z->p->color = 0;//recolor 
					y->color = 1;
					rightRotate(y);
				}
			}
			else//mirror cases
			{
				if (y->x && y->x->color)
				{
					z->p->color = y->x->color = 0;
					y->color = 1;
					z = y;
				}
				else
				{
					if (z == z->p->x)
					{
						z = z->p;
						rightRotate(z);
					}
					z->p->color = 0;
					y->color = 1;
					leftRotate(y);
				}
			}
			root->color = 0;
		}
	}

	/* Perform left rotation on node z */
	void leftRotate(Node *z)
	{
		if (z->y)
		{
			Node * y = z->y;
			z->y = y->x;
			if (y->x)
				y->x->p = z;
			y->p = z->p;
			if (!(z->p))
				root = y;
			else if (z == z->p->x)
				z->p->x = y;
			else
				z->p->y = y;
			y->x = z;
			z->p = y;
		}
	}

	/* Perform right rotation on node z */
	void rightRotate(Node *z)
	{
		if (z->x)
		{
			Node * y = z->x;
			z->x = y->y;
			if (y->y)
				y->y->p = z;
			y->p = z->p;
			if (!(z->p))
				root = y;
			else if (z == z->p->y)
				z->p->y = y;
			else
				z->p->x = y;
			y->y = z;
			z->p = y;
		}
	}

	/* Recursively traverse the tree */
	void inOrderWalk(Node *r)
	{
		if (r != NULL)
		{
			inOrderWalk(r->x);
			cout << r->key << r->color << endl;
			inOrderWalk(r->y);
		}
	}

	/* Recursive method to search the tree for a string and return true or false */
	bool search(Node *z, string k)
	{
		if (!z)
			return false;
		if (k == z->key)
			return true;
		if (k < z->key)
		{
			if (z->x)
				return search(z->x, k);
			else 
				return false;
		}
		else
		{
			if (z->y)
				return search(z->y, k);
			else 
				return false;
		}
	}
};

void userInterface(int &treeChoice, int &fileSize, int &sorted, string &kw);
void output(double t1, double t2, bool s);

/********** Main **********
* Program creates word dictionaries from input files
* and searches for chosen word, displaying result of search
* and tree creation time.
*/
int main()
{	
	bool run = 1;
	while (run)
	{
		int treeType = -1;
		int size = 0;
		int sorted = 2;
		bool found = 0;
		string fileName, keyWord;
		double time, time2;

		userInterface(treeType, size, sorted, keyWord);
		if (!(sorted))
			fileName = "perm";
		else fileName = "sorted";
		fileName += to_string(size);
		fileName += "k.txt";
		cout << "Reading from file: " << fileName << endl;
		if (treeType == 0)
		{			
			DWORD start1 = GetTickCount();
			BST myBST;
			myBST.buildTree(fileName);
			DWORD end1 = GetTickCount();
			time = (end1 - start1) / 1000.0;
			DWORD start2 = GetTickCount();
			found = myBST.search(myBST.root, keyWord);
			DWORD end2 = GetTickCount();
			time2 = (end2 - start2) / 1000.0;
			output(time, time2, found);
		}
		else if (treeType)
		{
			DWORD start1 = GetTickCount();
			RBT myRBT;
			myRBT.buildTree(fileName);
			DWORD end1 = GetTickCount();
			time = (end1 - start1) / 1000.0;
			DWORD start2 = GetTickCount();
			found = myRBT.search(myRBT.root, keyWord);
			DWORD end2 = GetTickCount();
			time2 = (end2 - start2) / 1000.0;
			output(time, time2, found);
		}
		cout << endl << "Enter 1 to run again, 0 to quit." << endl;
		cin >> run;
		while (run != 1 && run != 0)
		{
			cout << endl << "Only 1 or 0 are allowed. Please try again.";
			cout << endl << "Enter 1 to run again, 0 to quit." << endl;
			cin >> run;
		}

	}
}

/* User interface */
void userInterface(int &treeChoice, int &fileSize, int &sorted, string &kw)
{
	while (treeChoice > 1 || treeChoice < 0)
	{
		cout << "Please select a tree structure by number." << endl;
		cout << "0: Binary Search Tree" << endl;
		cout << "1: Red Black Tree" << endl;
		cin >> treeChoice;
		if (treeChoice > 1 || treeChoice < 0)
			cout << "Please try again." << endl;
	}
	while ((fileSize > 150 || fileSize < 30) || ((fileSize % 30) != 0))
	{
		cout << endl << "Please select a file size in thousands. ie: 30 , 60 , 90 , 120 , 150" << endl;
		cin >> fileSize;
		if ((fileSize > 150 || fileSize < 30) || ((fileSize % 30) != 0))
			cout << "Please try again." << endl;
	}
	while (sorted < 0 || sorted > 1)
	{
		cout << endl << "Enter 0 to process an unsorted list, and 1 for a sorted list." << endl;
		cin >> sorted;
		if (sorted < 0 || sorted > 1)
			cout << "Please try again" << endl;
	}
	cout << endl << "Please enter the word you wish to search for IN ALL CAPITAL LETTERS: " << endl;
	cin >> kw;
}

/* Displays final results */
void output(double t1, double t2, bool s)
{
	cout << endl << "Time elapsed to construct the tree: " << t1 << " seconds." << endl;
	if (s)
		cout << endl << "Your word was found in " << t2 << " seconds!" << endl;
	else
		cout << endl << "Your word was not found, the search took " << t2 << " seconds." << endl;
}



