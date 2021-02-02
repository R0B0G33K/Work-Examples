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
	bool hasParent = true;
	
	
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
		else if(numChildren == 1){
			cout << "1 child policy" << endl;
			cout << value[2] << endl;
		}
		else{
			cout<<value[0]<<value[1]<<value[2]<<endl;
			cout << "HELP" << endl;
		}
	}
	
	
	
};


class Tree{
	
	
	public:
	
	Node * root;
	
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
	

	void absorb(Node * b, Node * a){
		
		cout << "Absorption is occuring" << endl;
		cout << a->value[0] <<" "<< a->value[1]<<" "<<a->value[2] << "; " <<b->value[0]<<" "<<b->value[1]<<" "<< b->value[2] << endl;
		if(b->value[2] == 3000){
			b = root;
			while(b->numChildren != 0){
				b = b->child[2];
			}
			absorb(a,b);
		}
		
		if(b->hasParent == false){
			cout << "ROOT CASE" <<endl;
			Node groot(a, b);
			a->parent = &groot;
			b->parent = &groot;
			b->hasParent = true;
			groot.hasParent = false;
			root = &groot;
			root->print();
			return;
		}

		Node * p = b->parent; //2: 3,8,17
		if(p->numChildren == 0){
			cout<<"I have no children" <<endl;
		}
		
		if(p->numChildren == 2){
			cout<<"easyInsert" <<endl;
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
				for(int i = 0; i <3; i+=2){
					p->value[i] = p->child[i]->value[2];
					if(p->numChildren == 3){
						p->value[1] = p->child[1]->value[2];
					}
					if(i == 2){
						if(p->hasParent){
							i=0;
							p = p->parent;
						}
					}
				}
				
				p = b->parent;
			}
			p->numChildren = 3;
			for(int i =0; i < p->numChildren; i++)
				p->value[i] = p->child[i]->value[2];
			
			root->print();
			
		}
		else if(p->numChildren == 3){
			Node * pleft;
			Node * newP;
			if(a->value[2] < p->child[0]->value[2]){
				cout << "DEBUG CASE 1" <<endl;
				Node ihatecreatingnodes(a,p->child[0]);
				pleft = &ihatecreatingnodes;
				pleft->child[0]->parent = pleft;
				pleft->child[2]->parent = pleft;

				Node defaceMemoryNode (p->child[1],p->child[2]);
				newP = &defaceMemoryNode;
				newP->child[0]->parent = newP;
				newP->child[2]->parent = newP;
				
				p->child[0]->parent = pleft;
				p->child[0] = p->child[1];
			}
			else if(a->value[2] < p->child[1]->value[2]){ //1. 11, 12
				cout << "DEBUG CASE 2" <<endl;
				Node ihatecreatingnodes(p->child[0],a);
				pleft = &ihatecreatingnodes;
				pleft->child[0]->parent = pleft;
				pleft->child[2]->parent = pleft;

				Node defaceMemoryNode (p->child[1],p->child[2]);//1.14,17
				newP = &defaceMemoryNode;
				newP->child[0]->parent = newP;
				newP->child[2]->parent = newP;
				
				p->child[0]->parent =pleft;
				a->parent = pleft;
				p->child[0] = p->child[1];
			}
			else if(a->value[2] > p->child[2]->value[2]){
				cout << "DEBUG CASE 4" <<endl;
				Node ihatecreatingnodes(p->child[0],p->child[1]);
				pleft = &ihatecreatingnodes;
				pleft->child[0]->parent = pleft;
				pleft->child[2]->parent = pleft;
				
				Node defaceMemoryNode (p->child[2],a);
				newP = &defaceMemoryNode;
				newP->child[0]->parent = newP;
				newP->child[2]->parent = newP;
				
				p->child[0]->parent = pleft;
				p->child[1]->parent = pleft;
				p->child[0] = p->child[2];
				p->child[2] = a;
				a->parent = p;
			}
			else {
				cout << "DEBUG CASE 3" <<endl;
				Node ihatecreatingnodes(p->child[0],p->child[1]);
				pleft = &ihatecreatingnodes;
				pleft->child[0]->parent = pleft;
				pleft->child[2]->parent = pleft;
				
				Node defaceMemoryNode (a,p->child[2]);
				newP = &defaceMemoryNode;
				newP->child[0]->parent = newP;
				newP->child[2]->parent = newP;

				p->child[0]->parent = pleft;
				p->child[1]->parent = pleft;
				p->child[0] = a;
				a->parent = p;				
				
			}
			
			if(p->hasParent){
				newP->parent = p->parent;
				pleft->parent = p->parent;
			}
			else
				newP->hasParent = false;

			
			p->value[0] = p->child[0]->value[2];
			p->value[2] = p->child[2] ->value[2];
			
			p->numChildren = 2;		
			pleft->numChildren = 2;
			
			//cout<<"pleft: "<< pleft->value[0] << " " << pleft->value[2] << endl;
			//cout<<"p: "<< newP->value[0] << " " << newP->value[2] << endl;
			
			
			absorb(newP,pleft);
			
		}
	};
	
	int * fillSiblings(int allSiblings[], Node * g){
		Node * p = g->parent;
		int size1 = 0;
		int size2 = 0;
		int size3 = 0;
		if(p->child[0]->numChildren != 1)
			size1 = p->child[0]->numChildren;
		if( p->numChildren == 3 && p->child[1]->numChildren != 1)
			size2 = p->child[1]->numChildren;
		if(p->child[2]->numChildren != 1)
			size3 = p->child[2]->numChildren;
		for(int i = 0; i< size1; i++)
			allSiblings[i] = p->child[0]->value[i];
		for(int i = size1; i< size2; i++)
			allSiblings[i] = p->child[0]->value[i];
		for(int i = size2; i< size3; i++)
			allSiblings[i] = p->child[0]->value[i];	
		
		cout << size1 << ", " << size2 << ", " << size3 <<endl;
		allSiblings[6] = size1+size2+size3;
		
		return allSiblings;
	};
	
	
	void discard(Node * a){
		//cout<<"it is discarding" <<endl;
		Node * p = a->parent;
		//cout<<p->value[0]<<" "<<p->value[1]<<" "<<p->value[2]<<endl;
		if(p->numChildren == 3){
			if(a->value[2] == p->child[0]->value[2]){
				a->parent = 0;
				p->child[0] = p->child[1];
				p->numChildren = 2;
				p->value[0] = p->value[1];
			}
			if(a->value[2] == p->child[1]->value[2]){
				a->parent = 0;
				p->child[1] = 0;
				p->numChildren = 2;
				p->value[1] = 0;
			}
			if(a->value[2] == p->child[2]->value[2]){
				int newNorm = p->child[1]->value[2];
				a->parent = 0;
				p->child[2] = p->child[1];
				p->numChildren = 2;	
				for(int i = 0; i <3; i++){
					if(p->value[i] == a->value[2]){
						p->value[i] = newNorm;
						if(i == 2){
							i=0;
							p=p->parent;
						}
					}
				}
				if(!p->hasParent)
					for(int i = 0; i<3; i++)
						p->value[i] = p->child[i]->value[2];
			}
			root->print();
			return;
		}
		else if(p->numChildren == 2){

			//cout<<"it got called" <<endl;
			//cout<<p->numChildren<<endl;
			if(a->value[2] == p->child[0]->value[2]){
				a->parent = 0;
				p->numChildren = 1;
			}
			if(a->value[2] == p->child[2]->value[2]){
				int newNorm = p->child[0]->value[2];
				
				p->child[2] = p->child[0];
				p->numChildren = 1;	
				for(int i = 0; i <3; i++){
					if(p->value[i] == a->value[2]){
						p->value[i] = newNorm;
						if(i == 2){
							i=0;
							p=p->parent;
						}						
					}
				}
				//cout<<p->value[0]<<" "<<p->value[1]<<" "<<p->value[2]<<endl;
				if(!p->hasParent)
					for(int i = 0; i<3; i++)
						p->value[i] = p->child[i]->value[2];								
				p = a->parent;
				a->parent = 0;
			}
			//cout<<p->value[0]<<" "<<p->value[1]<<" "<<p->value[2]<<endl;
			//cout<<p->numChildren<<endl;
			cout<<"close but no cigar" <<endl;
			int siblings[7] = {0};
			int * sib = fillSiblings(siblings,p);
			if(sib[6] >= 3){
				// cout<<"GOAL" <<endl;
				// cout <<sib[6] <<endl;
				
				if(sib[6] == 3){
					if(p == p->parent->child[2]){
						//cout<<"p is child 2 motherfucker" <<endl;
						p->child[0] = p->parent->child[0]->child[2];
						p->parent->child[0]->child[2] = p->parent->child[0]->child[1];
						p->child[0]->parent = p;
						p->numChildren = 2;
						p->parent->child[0]->numChildren = 2;
						for(int i = 0; i <3; i+=2){
							p->parent->child[0]->value[i] = p->parent->child[0]->child[i]->value[2];
							p->parent->child[2]->value[i] = p->parent->child[2]->child[i]->value[2];
						}
						int newNorm = p->parent->child[0]->value[2];
						int oldNorm = p->child[0]->value[2];
						for(int i = 0; i <3; i++){
							if(p->parent->value[i] == oldNorm){
								p->parent->value[i] = newNorm;
								if(i == 2){
									i=0;
									p=p->parent;
								}
							}
						}
						
						
					}
					else if(p == p->parent->child[0]){
						//cout<<"p is child 0 Sam" <<endl;
						p->child[0] = p->child[2];
						p->child[2] = p->parent->child[2]->child[0];
						p->parent->child[2]->child[0] = p->parent->child[2]->child[1];
						p->child[2]->parent = p;
						p->numChildren = 2;
						p->parent->child[2]->numChildren = 2;
						for(int i = 1; i <3; i++){
							p->parent->child[0]->value[i] = p->parent->child[0]->child[i]->value[2];
							p->parent->child[2]->value[i] = p->parent->child[2]->child[i]->value[2];
						}
					}
					else{
						//cout<<"youve done fucked up A-A-RON"<<endl;
					}
				}
				
				if(sib[6] == 4){
					//cout<<"super mega hockey goal"<<endl;
					if(p == p->parent->child[2]){
						//cout<<"p is ICE" <<endl;
						p->child[0] = p->parent->child[1]->child[1];
						p->child[1] = p->parent->child[1]->child[2];
						p->child[0]->parent = p;
						p->child[1]->parent = p;
						p->numChildren = 3;
						p->parent->numChildren = 2;
					}
					else if(p == p->parent->child[0]){
						//cout<<"p is Kevin, fuck you kevin" <<endl;
						p->child[1] = p->parent->child[1]->child[1];
						p->child[2] = p->parent->child[1]->child[0];
						p->child[1]->parent = p;
						p->child[2]->parent = p;
						p->numChildren = 3;
						p->parent->numChildren = 2;
						p->parent->value[0] = p->child[2]->value[2];
					}
					else if(p == p->parent->child[1]){
						//cout<<"I fucking hate kids"<<endl;
						p->parent->child[2]->child[1] = p->parent->child[2]->child[0];
						p->parent->child[2]->child[0] = p->child[2];
						p->parent->child[2]->child[0]->parent = p->parent->child[2];
						p->parent->child[2]->numChildren = 3;
						p->parent->numChildren = 2;
						p->parent->child[2]->value[1] = p->parent->child[2]->child[1]->value[2];
						p->parent->child[2]->value[0] = p->parent->child[2]->child[0]->value[2];
					}					
				}
				if(sib[6] == 5){
					if(p == p->parent->child[2]){
						if(p->parent->child[1]->numChildren == 3){
							p->child[0] = p->parent->child[1]->child[2];
							p->parent->child[1]->child[2] = p->parent->child[1]->child[1];
							p->child[0]->parent = p;
							p->numChildren = 2;
							p->parent->child[1]->numChildren = 2;
							p->parent->value[1] = p->parent->child[1]->child[2]->value[2];
							
							for(int i = 0; i <3; i+=2){
								p->parent->child[1]->value[i] = p->parent->child[1]->child[i]->value[2];
								p->parent->child[2]->value[i] = p->parent->child[2]->child[i]->value[2];
							}
						}
						else if(p->parent->child[1]->numChildren == 2){
							p->child[0] = p->parent->child[1]->child[2];
							p->parent->child[1]->child[2] = p->parent->child[1]->child[0];
							p->parent->child[1]->child[0] = p->parent->child[0]->child[2];
							p->parent->child[0]->child[2] = p->parent->child[0]->child[1];
							p->child[0]->parent = p;
							p->parent->child[1]->child[0]->parent = p->parent->child[1];
							p->numChildren = 2;
							p->parent->child[1]->numChildren = 2;
							p->parent->child[0]->numChildren = 2;
							p->parent->value[1] = p->parent->child[1]->child[2]->value[2];
							p->parent->value[0] = p->parent->child[0]->child[2]->value[2];
							
							for(int i = 0; i <3; i+=2){
								p->parent->child[0]->value[i] = p->parent->child[0]->child[i]->value[2];
								p->parent->child[1]->value[i] = p->parent->child[1]->child[i]->value[2];
								p->parent->child[2]->value[i] = p->parent->child[2]->child[i]->value[2];
							}
						}
					}
					else if(p == p->parent->child[0]){
						if(p->parent->child[1]->numChildren == 3){
							p->child[0] = p->child[2];
							p->child[2] = p->parent->child[1]->child[0];
							p->parent->child[1]->child[0] = p->parent->child[1]->child[1];
							p->child[2]->parent = p;
							p->numChildren = 2;
							p->parent->child[1]->numChildren = 2;
							p->parent->value[0] = p->value[2];
							
							for(int i = 0; i <3; i+=2){
								p->parent->child[1]->value[i] = p->parent->child[1]->child[i]->value[2];
								p->value[i] = p->child[i]->value[2];
							}
						}
						else if(p->parent->child[2]->numChildren == 3){
							p->child[0] = p->child[2];
							p->child[2] = p->parent->child[1]->child[0];
							p->parent->child[1]->child[0] = p->parent->child[1]->child[2];
							p->parent->child[1]->child[2] = p->parent->child[2]->child[0];
							p->parent->child[2]->child[0] = p->parent->child[2]->child[1];
							p->child[2]->parent = p;
							p->parent->child[1]->child[2]->parent = p->parent->child[1];
							p->numChildren = 2;
							p->parent->child[2]->numChildren = 2;
							p->parent->value[0] = p->value[2];
							p->parent->value[1] = p->parent->child[1]->value[2];
							
							for(int i = 0; i <3; i+=2){
								p->parent->child[2]->value[i] = p->parent->child[2]->child[i]->value[2];
								p->parent->child[1]->value[i] = p->parent->child[1]->child[i]->value[2];
								p->value[i] = p->child[i]->value[2];
							}
						}
					}
					else if(p == p->parent->child[1]){
						if(p->parent->child[0]->numChildren == 3){
							p->child[0] = p->parent->child[0]->child[2];
							p->parent->child[0]->child[2] = p->parent->child[0]->child[1];
							p->child[0]->parent = p;
							p->numChildren = 2;
							p->parent->child[0]->numChildren = 2;
							p->parent->value[0] = p->parent->child[0]->child[2]->value[2];
							
							for(int i = 0; i <3; i+=2){
								p->value[i] = p->child[i]->value[2];
								p->parent->child[0]->value[i] = p->parent->child[0]->child[i]->value[2];
							}
						}
						else if(p->parent->child[2]->numChildren == 3){
							p->child[0] = p->child[2];
							p->child[2] = p->parent->child[2]->child[0];
							p->parent->child[2]->child[0] = p->parent->child[2]->child[1];
							p->child[2]->parent = p;
							p->numChildren = 2;
							p->parent->child[2]->numChildren = 2;
							p->parent->value[1] = p->child[2]->value[2];
							
							for(int i = 0; i <3; i+=2){
								p->parent->child[1]->value[i] = p->parent->child[1]->child[i]->value[2];
								p->parent->child[2]->value[i] = p->parent->child[2]->child[i]->value[2];
							}
						}
					}	
				}
				if(sib[6] == 6){
					cout<<"666 hail santa and satan, equally."<<endl;
					if(p == p->parent->child[0]){
						p->child[0] = p->child[2];
						p->child[2] = p->parent->child[1]->child[0];
						p->parent->child[1]->child[0] = p->parent->child[1]->child[1];
						p->parent->child[1]->numChildren = 2;
						p->numChildren = 2;
						p->child[2]->parent = p;
						p->parent->value[0] = p->child[2]->value[2];
						
						for(int i = 0; i <3; i+=2){
							p->parent->child[0]->value[i] = p->parent->child[0]->child[i]->value[2];
							p->parent->child[1]->value[i] = p->parent->child[1]->child[i]->value[2];
						}
					} 
					else if(p == p->parent->child[1]){
						p->child[0] = p->child[2];
						p->child[2] = p->parent->child[2]->child[0];
						p->parent->child[2]->child[0] = p->parent->child[2]->child[1];
						p->parent->child[2]->numChildren = 2;
						p->numChildren = 2;
						p->child[2]->parent = p;
						p->parent->value[1] = p->child[2]->value[2];
						
						for(int i = 0; i <3; i+=2){
							p->parent->child[1]->value[i] = p->parent->child[1]->child[i]->value[2];
							p->parent->child[2]->value[i] = p->parent->child[2]->child[i]->value[2];
						}
					}
					else if(p == p->parent->child[2]){
						p->child[0] = p->parent->child[1]->child[2];
						p->parent->child[1]->child[2] = p->parent->child[1]->child[1];
						p->child[0]->parent = p;
						p->numChildren = 2;
						p->parent->child[1]->numChildren = 2;
						p->parent->value[1] = p->parent->child[1]->child[2]->value[2];
						
						for(int i = 0; i <3; i+=2){
							p->parent->child[1]->value[i] = p->parent->child[1]->child[i]->value[2];
							p->parent->child[2]->value[i] = p->parent->child[2]->child[i]->value[2];
						}
					}
				}
				//distribute the children among parents evenly
					//will have to remove one of the parents if 5 siblings (1,2,2)
				//readjust p values and higher levels
				root->print();
				return;
			}	
			else if(sib[6] == 2){
				cout<<"GOAL2" <<endl;
				Node * b = p->child[2]; //only child
				if(!p->hasParent){
					cout<<"p orphan"<<endl;
					b->parent = 0;
					b->hasParent = false;
					b = root; 
					
					root->print();
					return;
				}
				//put b (p's kid) as p's only sibling's kid
				if(p == p->parent->child[2]){
					cout<<"p's only sibling's kid"<<endl;
					p->parent->child[0]->child[1] = p->parent->child[0]->child[2];
					p->parent->child[0]->child[2] = b;
					p->parent->child[0]->numChildren = 3;
					b->parent = p->parent->child[0];
					p->parent->numChildren = 1;
					
					p->parent->child[0]->value[2] = p->parent->child[0]->child[2]->value[2];
					
					p->parent->value[0] = b->value[2];
				
				}
				else if(p == p->parent->child[0]){
					p->parent->child[2]->child[1] = p->parent->child[0]->child[0];
					p->parent->child[2]->child[0] = b;
					p->parent->child[2]->numChildren = 3;
					b->parent = p->parent->child[2];
					p->parent->numChildren = 1;
					
					p->parent->child[2]->value[2] = p->parent->child[2]->child[2]->value[2];
									
				}
				return discard(p);
			}
			else{
				//cout<<"missed it" <<endl;
				cout<< sib[7] <<endl;
			}
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
	
	void suicide(Node * a){
		cout << "Seppuku is happening" <<endl;
		Node * b = search(a->value[2],root);
		if(a->value[2] == b->value[2])
			discard(b);
		cout << "He's dead, Jim" <<endl;
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
	root.hasParent = false;
	
	Tree tree(&root);
	//cout << "OLD TREE:" <<endl;
	//tree.print();
	//Node valToAdd(0);
	//Node valToAdd(2);
	//Node valToAdd2(20);
	Node valToKill(31);
	//Node valToKill2(11);
	//tree.insert(&valToAdd);
	tree.suicide(&valToKill);
	//tree.suicide(&valToKill2);
	//cout << "NEW TREE:" <<endl;
	//tree.insert(&valToAdd);
	//tree.insert(&valToAdd2);
	//tree.suicide(&valToKill2);
	
	
	return 0;
}