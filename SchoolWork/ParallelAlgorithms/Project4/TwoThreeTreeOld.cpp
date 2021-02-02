//by Matthew O'Hern, Parker Winters, and Alex McGrath

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  
#include <time.h>
#include <algorithm>
#include <cmath>

using namespace std;

// g++ -o hello hello.cpp
// /hello


//Tree:
//tree is an array




//Nodes:
//three child policy
//left pointer, middle pointer, right pointer
//each node has largest value of descendants (grandpa forgets about unimportant descendants)
//                                                                      5,  999, 3000
//                            3, null, 5                                       7, 10 ,999                                                1002, 2001 3000
//                    1   2   3       4 null 5            6   7        8    9   10       420    666    999           1001  1002        2000  2001        2008  3000
// leaves             1   2   3       4      5            6   7        8    9   10       420    666    999           1001  1002        2000  2001        2008  3000
//red wedding on root if they are bad rulers


class Node {
	
	public:
	
	int value[6] = {0};
	Node * parent;
	Node * child[6];
	
	
	
	int numChildren;
	
	Node();
	
	Node(Node * n1, Node * n2){
		child[0] = n1;
		child[2] = n2;
		value[0] = n1->value[2];
		value[2] = n2->value[2];
		numChildren = 2;
	}
	
	Node(Node * n1, Node * n2, Node * n3){
		child[0] = n1;
		child[1] = n2;
		child[2] = n3;
		value[0] = n1->value[2];
		value[1] = n2->value[2];
		value[2] = n3->value[2];
		numChildren = 3;
	}
	
	
	Node(int val){
		value[2] = val;
		numChildren = 0;
	}
	
	void print(){
		if(numChildren == 2){
			cout << value[0] << ", " << value[1] << ", " << value[2] << endl;
			child[0] -> print();
			child[2] -> print();
		}
		else if(numChildren == 3){
			cout << value[0] << ", " << value[1] << ", " << value[2] << endl;
			child[0] -> print();
			child[1] -> print();
			child[2] -> print();
		}
		else if(numChildren == 0){
			cout << value[2] << endl;
		}
		else{
			cout<<value[0]<<value[1]<<value[2]<<endl;
			cout << "HELP" << endl;
		}
	}
	
	
	
};


class Tree{
	
	Node * root;
	
	public:
	
	void print(){
		root->print();
	}
	
	void print(Node * start){
		start->print();
	}
	
	
	Tree(Node * r){
		root = r;
	}
	
	Node * search(int valToFind, Node * n){		
		if(n->numChildren == 0)	
			return n;
		if(valToFind <= n->value[0]) 	
			return search(valToFind, n->child[0]);
		else if(valToFind <= n->value[1])	
			return search(valToFind, n->child[1]);
		else if(valToFind <= n->value[2])	
			return search(valToFind, n->child[2]);
		else{
			Node iloveyou(3000);
			Node * jack = &iloveyou;
			return jack;
		}			
	}
	
	int counter = 0;
	void absorb(Node * b, Node * a){
		
		cout << "Absorption is occuring" << endl;
		//cout << a->value[0] <<" "<< a->value[1]<<" "<<a->value[2] << "; " <<b->value[0]<<" "<<b->value[1]<<" "<< b->value[2] << endl;
		
		if(b == root){
			Node groot(a, b);
			Node * jill = &groot;
			a->parent = jill;
			b->parent = jill;
			root = jill;
			return;
		}

		Node * p = b->parent;
		if(p->numChildren == 2){
			if(a->value[2] < p->child[0]->value[2]){
				p->child[0] = a;
				p->child[1] = b;
			}
			else if(a->value[2] < p->child[2]->value[2]){
				p->child[1] = a;
				p->value[1] = a->value[2];
			}
			else if(a->value[2] > p->child[2]->value[2]){
				p->child[1] = p->child[2];
				p->child[2] = a;
			}
			p->numChildren = 3;
			for(int i =0; i < p->numChildren; i++)
				p->value[i] = p->child[i]->value[2];
			
		}
		else if(p->numChildren == 3){
			Node * pleft;
			counter++;
			if(a->value[2] < p->child[0]->value[2]){
				Node ihatecreatingnodes(a,p->child[0]);
				pleft = &ihatecreatingnodes;
				p->child[0]->parent =pleft;
				a->parent = pleft;
				p->child[0] = p->child[1];
			}
			else if(a->value[2] < p->child[1]->value[2]){
				Node ihatecreatingnodes(p->child[0],a);
				pleft = &ihatecreatingnodes;
				p->child[0]->parent =pleft;
				a->parent = pleft;
				p->child[0] = p->child[1];
			}
			else if(a->value[2] > p->child[2]->value[2]){
				Node ihatecreatingnodes(p->child[0],p->child[1]);
				pleft = &ihatecreatingnodes;
				p->child[0]->parent = pleft;
				p->child[1]->parent = pleft;
				p->child[0] = p->child[2];
				p->child[2] = a;
				a->parent = p;
			}
			else {
				Node ihatecreatingnodes(p->child[0],p->child[1]);
				pleft = &ihatecreatingnodes;
				p->child[0]->parent = pleft;
				p->child[1]->parent = pleft;
				p->child[0] = a;
				a->parent = p;
			}
			
			p->value[0] = p->child[0]->value[2];
			p->value[2] = p->child[2] ->value[2];
			cout<<"pleft: "<< pleft->value[0] << " " << pleft->value[2] << endl;
			cout<<"p: "<< p->value[0] << " " << p->value[2] << endl;
			
			p->numChildren = 2;		
			pleft->numChildren = 2;
			
			if(counter < 2)
				absorb(p,pleft);
			
		}
	}
	
	void insert(Node * a){
		cout << "Insertion is happening" <<endl;
		Node * b = search(a->value[2],root);
		if(a->value[2] != b->value[2])
			absorb(b,a);
		cout << "Insertion is Over" <<endl;
		return;
	}
	
};

int main (int argc, char * argv[]) {


	//MAKE TREE WORK
	Node a(39);
	Node b(36);
	Node c(33);
	Node d(31);
	Node e(28);
	Node f(24);
	Node g(21);
	Node h(19);
	Node i(17);
	Node j(14);
	Node k(11);
	Node l(8);
	Node m(5);
	Node n(3);
	Node o(1);
	
	Node ab(&b, &a);
	a.parent = &ab;
	b.parent = &ab;
	Node cd(&d, &c);
	c.parent = &cd;
	d.parent = &cd;
	Node ef(&f, &e);
	e.parent = &ef;
	f.parent = &ef;
	Node gh(&h, &g);
	h.parent = &gh;
	g.parent = &gh;
	Node ijk(&k, &j, &i);
	i.parent = &ijk;
	j.parent = &ijk;
	k.parent = &ijk;
	Node lm(&m, &l);
	l.parent = &lm;
	m.parent = &lm;
	Node no(&o, &n);
	n.parent = &no;
	o.parent = &no;
	
	Node ad(&cd, &ab);
	cd.parent = &ad;
	ab.parent = &ad;
	Node eh(&gh, &ef);
	gh.parent = &eh;
	ef.parent = &eh;
	Node ilo(&no, &lm, &ijk);
	no.parent = &ilo;
	lm.parent = &ilo;
	ijk.parent = &ilo;
	
	Node root(&ilo, &eh, &ad);
	ilo.parent = &root;
	eh.parent = &root;
	ad.parent = &root;
	
	Tree tree(&root);
	Node valToAdd(12);
	tree.insert(&valToAdd);
	tree.print();

	
	
	return 0;
}