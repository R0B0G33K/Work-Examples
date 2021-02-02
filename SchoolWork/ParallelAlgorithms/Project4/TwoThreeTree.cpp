//Parker Winters, Matthew O'Hern, and Alex McGrath

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

class Node {
	
	public:
	
	int value[3];
	Node * children[3];
	Node * parent;

	Node (int val) {
		value[0] = 0;
		value[1] = 0;
		value[2] = val;
		for (int i = 0; i<3; i++)
			children[i] = NULL;
		parent = NULL;
	}
	
	Node (Node * n1, Node * n2){
		children[0] = n1;
		children[1] = NULL;
		children[2] = n2;
		n1->parent = this;
		n2->parent = this;
		value[0] = n1->value[2];
		value[1] = 0;
		value[2] = n2->value[2];
		parent = NULL;
	}
	
	Node (Node * n1, Node * n2, Node * n3){
		children[0] = n1;
		children[1] = n2;
		children[2] = n3;
		n1->parent = this;
		n2->parent = this;
		n3->parent = this;
		value[0] = n1->value[2];
		value[1] = n2->value[2];
		value[2] = n3->value[2];
		parent = NULL;
	}
	
	//returns the number of children of a node
	int numChildren(){
		int tempChild=0;
		for (int i = 0; i<3; i++)
			if (children[i] != NULL && value[i]!=0)
				tempChild++;
		return tempChild;
	}
	
	//prints out the number of children and the values of a node
	void printNode() {
		cout << "NC: " << this->numChildren();
		cout << setw(7) <<"V: ";
		for (int i=0; i<3; i++)
			if (value[i] == 0)
				cout << setw(3) << " ";
			else
				cout << setw(3) << value[i];
		cout << endl;
	}

	// returns the number of cousins and siblings of a node-to-be-deleted
	int totalCousins() {
		int tempCousins=0;
		if (this->parent != NULL && this->parent->parent != NULL){
			Node * g = this->parent->parent;
			if (this->parent != g->children[0])
				tempCousins += g->children[0]->numChildren();
			if (g->numChildren()==3 && this->parent != g->children[1])
				tempCousins += g->children[1]->numChildren();
			if (this->parent != g->children[2])
				tempCousins += g->children[2]->numChildren();
		}
		if (this->parent != NULL)
			return tempCousins + this->parent->numChildren();
		
		return tempCousins;
	}
};

class Tree {
	
	public:
	Node * root;
	
	
	Tree (Node * newRoot) {
		root = newRoot;
	}
	
	//prints out the tree
	void print(Node * start) {
		start->printNode();
		if (start->numChildren() != 0) {
			this->print(start->children[0]);
			if (start->numChildren() == 3)
				this->print(start->children[1]);
			this->print(start->children[2]);
		}	
	}
	
	//searches in the tree to see if a value is in it
	// if yes returns the node that holds that value
	// if no returns the next largest node
	Node * search (int valToFind, Node * node) {
		if (node->numChildren() == 0)
			return node;
		else 
			if (valToFind <=  node->value[0])
				this->search(valToFind, node->children[0]);
			else if (node->numChildren() ==3 && valToFind <=  node->value[1])
				this->search(valToFind, node->children[1]);
			else if (valToFind <=  node->value[2])
				this->search(valToFind, node->children[2]);
			else {
				return new Node(3000);
			}
	}
	
	//disconects a node from the tree
	void disconect(Node * a, int posToDis) {
		Node * p = a->parent;
			p->children[posToDis] = NULL;
			p->value[posToDis] = 0;
			a->parent = NULL;
	}
	
	//finds the next leftmost grandchild of a node that is conected to the tree
	Node * findNextChild(Node * p) {
		Node * g = p->parent;
		Node * runt = NULL;
		for (int i=0; i<3; i++)
			if (g->children[i] != NULL)
				for (int j=0; j<3; j++)
					if (g->children[i]->children[j] != NULL && g->children[i]->value[j] != 0){
						runt = g->children[i]->children[j];
						disconect(runt, j);
						return runt;
					}
	}

	//updates the parent node with all the proper values of its children nodes
	void updateTree(Node * a) {
		Node * p = NULL;
		if (a->parent != NULL) {
			p = a->parent;
			for (int i =0; i<3; i++)
				if (p->children[i] != NULL && p->children[i] == a && p->value[i] != a-> value[2])
				{
					 p->value[i] = a->value[2];
				}
		}
		if (p!= NULL)
			updateTree(p);
	}
	
	//helper function to delete values from the tree
	void discard(Node * a) {
		Node * p = a->parent;
		int tempValToDelete;
		
											//easy deletes
											
		if (p->numChildren() == 3) {
			if (p->children[0] == a) {				//left child deleted
		
				p->children[0] = p->children[1];			// change middle child to left child
				p->value[0] = p->value[1];
				
				disconect(a, 1);						// delete old middle child
			}
			else if (p->children[1] == a) {			//middle child deleted
			
				disconect(a, 1);						// delete old middle child
			}
			else if (p->children[2] == a) {			//right child deleted
			
				tempValToDelete = p->value[2];				//store old value of right child
				
				p->children[2] = p->children[1];			//change middle child to right child
				p->value[2] = p->value[1];
				
				updateTree(a);				//update the tree
				disconect(a, 1);							//delete old middle child
			}
		}
		
											//Hard deletes
													
		else if (p->numChildren() == 2){
			int totChildren = a->totalCousins()-1; 			//total number of children - 1 for deleted node
			if (totChildren > 3){							//what cases to follow when the totChildren > 3
				Node * g = p->parent;
				for (int i=0; i<3; i++)						//disconect the node-to-be-deleted from the tree
					if (p->children[i] == a)
						disconect(a,i);
				Node * child1 = findNextChild(p);			//finding all of the grandchildren nodes
				int ch1 = child1->value[2];					//and their max values
				Node * child2 = findNextChild(p);
				int ch2 = child2->value[2];
				Node * child3 = findNextChild(p);
				int ch3 = child3->value[2];
				Node * child4 = findNextChild(p);
				 int ch4 = child4->value[2];
				int ch5 = 0;
				Node * child5 = NULL;
				int ch6 = 0;
				Node * child6 = NULL;
				int ch7 = 0;
				Node * child7 = NULL;
				if (totChildren >= 5) {
					child5 = findNextChild(p);
					ch5 = child5->value[2];
				}
				if (totChildren >= 6) {
					child6 = findNextChild(p);
					ch6 = child6->value[2];
				}
				if (totChildren == 7){
					child7 = findNextChild(p);
					ch7 = child7->value[2];
				}
				
				if (totChildren ==4 || totChildren == 6) {				//assigning all the grandchildren nodes 
					g->children[0]->children[0] = child1;				//to their proper parents and positions
					g->children[0]->children[0]->parent = g->children[0];//and assigns the nodes parents
					g->children[0]->value[0] = ch1;				
					g->children[0]->children[2] = child2;		
					g->children[0]->children[2]->parent = g->children[0];		
					g->children[0]->value[2] = ch2;
					updateTree(g->children[0]);			//then updating the tree with the correct values
					if (totChildren==4){						//the case for 4 grandchildren nodes
						g->children[2]->children[0] = child3;
						g->children[2]->children[0]->parent = g->children[2];
						g->children[2]->value[0] = ch3;
						g->children[2]->children[2] = child4;
						g->children[2]->children[2]->parent = g->children[2];
						g->children[2]->value[2] = ch4;
						updateTree(g->children[2]);
					}
					else {										//the case for 6 grandchildren nodes
						g->children[1]->children[0] = child3;
						g->children[1]->children[0]->parent = g->children[1];
						g->children[1]->value[0] = ch3;
						g->children[1]->children[2] = child4;
						g->children[1]->children[2]->parent = g->children[1];
						g->children[1]->value[2] = ch4;
						updateTree(g->children[1]);
						g->children[2]->children[0] = child5;
						g->children[2]->children[0]->parent = g->children[2];
						g->children[2]->value[0] = ch5;
						g->children[2]->children[2] = child6;
						g->children[2]->children[2]->parent = g->children[2];
						g->children[2]->value[2] = ch6;
						updateTree(g->children[2]);
					}	
				}
				else {
					g->children[0]->children[0] = child1;
					g->children[0]->children[0]->parent = g->children[0];
					g->children[0]->value[0] = ch1;
					g->children[0]->children[1] = child2;
					g->children[0]->children[1]->parent = g->children[0];
					g->children[0]->value[1] = ch2;
					g->children[0]->children[2] = child3;
					g->children[0]->children[2]->parent = g->children[0];
					g->children[0]->value[2] = ch3;
					updateTree(g->children[0]->children[2]);
					if (totChildren==5){						//the case for 5 grandchildren nodes
						g->children[2]->children[0] = child4;
						g->children[2]->children[0]->parent = g->children[2];
						g->children[2]->value[0] = ch4;
						g->children[2]->children[2] = child5;
						g->children[2]->children[2]->parent = g->children[2];
						g->children[2]->value[2] = ch5;
						updateTree(g->children[2]->children[2]);
						discard(g->children[1]);
					}
					else {										//the case for 7 grandchildren nodes
						g->children[1]->children[0] = child4;
						g->children[1]->children[0]->parent = g->children[1];
						g->children[1]->value[0] = ch4;
						g->children[1]->children[2] = child5;
						g->children[1]->children[2]->parent = g->children[1];
						g->children[1]->value[2] = ch5;
						updateTree(g->children[1]->children[2]);
						g->children[2]->children[0] = child6;
						g->children[2]->children[0]->parent = g->children[2];
						g->children[2]->value[0] = ch6;
						g->children[2]->children[2] = child7;
						g->children[2]->children[2]->parent = g->children[2];
						g->children[2]->value[2] = ch7;
						updateTree(g->children[2]->children[2]);
					}
				}
			}
			else if (totChildren ==3){							//what case to follow when the totChildren = 3
				
				Node * g = p->parent;
				for (int i=0; i<3; i++)						//disconect the node-to-be-deleted from the tree
					if (p->children[i] == a)
						disconect(a,i);
				Node * child1 = findNextChild(p);			//finding all of the grandchildren nodes
				int ch1 = child1->value[2];					//and their max values
				Node * child2 = findNextChild(p);
				int ch2 = child2->value[2];
				Node * child3 = findNextChild(p);
				int ch3 = child3->value[2];
				g->children[0]->children[0] = child1;		//assigning all the grandchildren nodes
				g->children[0]->children[0]->parent = g->children[0];
				g->children[0]->value[0] = ch1;				//to their proper parents and positions
				g->children[0]->children[1] = child2;
				g->children[0]->children[1]->parent = g->children[0];
				g->children[0]->value[1] = ch2;
				g->children[0]->children[2] = child3;
				g->children[0]->children[2]->parent = g->children[0];
				g->children[0]->value[2] = ch3;
				updateTree(g->children[0]->children[2]);	//then updating the tree 
																//with the correct values
				if (p->parent->parent != NULL)				//if the grandparent of the node-to-be-deleted
					discard(g->children[2]);				//is not the root of the tree
				else {
					g->children[0]->parent=NULL;
					this->root = g->children[0];			//if it is the root 
				}

			}
			else {	//disconect the node-to-be-deleted from the tree				
				if (p->children[0] == a) {
					disconect(a,0);
					p->children[2]->parent=NULL;
					this->root = p->children[2];
				}
				else if (p->children[2] == a) {
					disconect(a,2);
					p->children[0]->parent=NULL;
					this->root = p->children[0];
				}
					
			}
		}
		
	}
	
	//delete a value from the tree
	bool seppuku (int valToKill) {
		// cout << valToKill << " is commiting seppuku" <<endl;
		Node * b = search(valToKill, root);
		if(valToKill == b->value[2])
			discard(b);
		else 
			cout << valToKill << " wasn't in the tree" << endl;
		// cout << valToKill <<" is dead, Jim" <<endl;
	}
	
	Node * pleft;
	Node * megaRoot;
	void absorb(Node * a, Node * b) {
		if(b->value[2] == 3000){
			b = root;
			while(b->numChildren() != 0){
				b = b->children[2];
			}
			absorb(a,b);
			return;
		}

		if(b == root){
			if (a->value[2] > b->value[2])
				megaRoot = new Node(b,a);
			else 
				megaRoot = new Node(a, b);
			root = megaRoot;
			return;
		}
		
		Node * p = b->parent;
		
		if(p->numChildren() == 2){
			if(p->children[0] == b){
				p->children[1] = p->children[0];
				p->value[1] = p->value[0];
				
				a->parent = p;
				p->children[0] = a;
				p->value[0] = a->value[2];
				
				updateTree(p);
			}
			else if(a->value[2] > b->value[2]){
				p->children[1] = p->children[2];
				p->value[1] = p->value[2];
				
				a->parent = p;
				p->children[2] = a;
				p->value[2] = a->value[2];
				
				updateTree(p);
			}	
			else if(p->children[2] == b){
				a->parent = p;
				p->children[1] = a;
				p->value[1] = a->value[2];
				
				updateTree(p);
			}
		}
		else if(p->numChildren() == 3){
			if(p->children[0] == b){
				pleft= new Node(a,p->children[0]);
				
				p->children[0] = p->children[1];
				p->value[0] = p->value[1];
				p->value[1] = 0;
				p->children[1] = NULL;
				updateTree(p);
			}
			else if(a->value[2] > b->value[2]){
				pleft = new Node(p->children[0],p->children[1]);
				
				p->children[0] = p->children[2];
				p->value[0] = p->value[2];
				p->children[2] = a;
				a->parent = p;
				p->value[2] = a->value[2];
				p->value[1] = 0;
				p->children[1] = NULL;
				updateTree(p);
			}
			else if(p->children[1] == b){
				pleft = new Node(p->children[0],a);
				
				p->children[0] = p->children[1];
				p->value[0] = p->value[1];
				p->value[1] = 0;
				p->children[1] = NULL;
				updateTree(p);
			}
			
			else {
				pleft = new Node(p->children[0],p->children[1]);
				
				p->children[0] = a;
				a->parent = p;
				p->value[0] = a->value[2];
				p->value[1] = 0;
				p->children[1] = NULL;
				updateTree(p);					
			}
			absorb(pleft, p);		
		}
	}
	
	//insert a value into the tree
	bool insert (int valToAdd) {
		// cout << "Insertion of " << valToAdd <<" is happening" <<endl;
		Node * b = search(valToAdd, root);
		if(valToAdd != b->value[2]) {
			Node * a = new Node(valToAdd);
			absorb(a,b);
		}
		else 
			cout << valToAdd << " was already in the tree" << endl;
		// cout << "Insertion is Over" <<endl;
	}
};

int main (int argc, char * argv[]) {
	
	Node i(21);
	
	Tree tree(&i);
	cout << "Old tree" << endl;
	tree.print(tree.root);

	cout << "Inserting into the tree" << endl;
	// tree.insert(33);
	// tree.insert(1);
	 tree.insert(19);
	 tree.insert(24);
	 tree.insert(28);
	// tree.insert(28);
	// tree.insert(31);
	// tree.insert(39);
	// tree.insert(24);
	// tree.insert(36);
	// tree.insert(3);
	// tree.insert(5);
	// tree.insert(11);
	// tree.insert(14);
	// tree.insert(8);
	// tree.insert(19 -);
	// tree.insert(21);
	
	 tree.print(tree.root);
	
	 cout << "Deleting from the tree" << endl;
	// tree.seppuku(11);
	// tree.seppuku(5);
	// tree.seppuku(28);
	// tree.seppuku(4);
	// tree.seppuku(21);
	 tree.seppuku(24);
	// tree.seppuku(1);
	// tree.seppuku(8);
	// tree.seppuku(36);
	// tree.seppuku(39);
	// tree.seppuku(17);
	// tree.seppuku(3);
	// tree.seppuku(31);
	// tree.seppuku(33);
	// tree.seppuku(14);

	cout << "New Tree" << endl;
	tree.print(tree.root);
	
	return 0;
}



