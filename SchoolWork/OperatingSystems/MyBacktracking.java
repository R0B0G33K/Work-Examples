import java.util.*;

public class MyBacktracking
{
	public static int maxP=0;
	public static int reference[];
	public static int n=0;				
	public static int W = 9;
	public static int matrix[][] =   {{1,2,20,10},
						{2,5,30,6},
						{3,7,35,5},
						{4,3,12,4},
						{5,1,3,3}};
	
	
	static Node Buildtree(Node r)
	{		
			int currentlevel= r.getLevel() +1;
			
			int profitLeft=(r.getProfit() + matrix[currentlevel-1][2]);
			int weightLeft = (r.getWeight() + matrix[currentlevel-1][1]);

		if (currentlevel<n+1)
		{
			if (weightLeft<=W)
			{
				int positionLeft=reference[currentlevel-1];
				reference[currentlevel-1]=(reference[currentlevel-1]+1);
				Node newleftnode=new Node(profitLeft,weightLeft,r.getBound(),currentlevel,positionLeft,null,null);
					System.out.println("Node(" + newleftnode.getLevel() + "," + newleftnode.getPosition() + ") has a profit, weight, and bound of " + 
								newleftnode.getProfit() + ", " + newleftnode.getWeight() + ", " + newleftnode.getBound());
				if (newleftnode.getProfit() > maxP && newleftnode.getBound() !=0)
					maxP= newleftnode.getProfit();
				if (((currentlevel+1)<(n)) && (newleftnode.getProfit()+ matrix[currentlevel][2]>maxP) && (newleftnode.getWeight()+ matrix[currentlevel+1][1]<=W))
				{
					newleftnode.leftChild = Buildtree(newleftnode);
				}
				if ((newleftnode.getBound()>maxP) && ((currentlevel+1)<=(n)) && (newleftnode.getProfit()+ matrix[currentlevel][2]>maxP))
				{
					newleftnode.rightChild = Buildtree(newleftnode);
				}
			}

			int positionRight=reference[currentlevel-1];
			reference[currentlevel-1]=(reference[currentlevel-1]+1);
			int boundRight = Bound(r.getProfit(),currentlevel,r.getWeight());
			Node newrightnode=new Node(r.getProfit(),r.getWeight(),boundRight,currentlevel,positionRight,null,null);
					System.out.println("Node(" + newrightnode.getLevel() + "," + newrightnode.getPosition() + ") has a profit, weight, and bound of " + 
							newrightnode.getProfit() + ", " + newrightnode.getWeight() + ", " + newrightnode.getBound());
			if (newrightnode.getProfit() > maxP && newrightnode.getBound() !=0)
				maxP= newrightnode.getProfit();
			if (((currentlevel+1)<=(n)) && (newrightnode.getBound()>maxP))
			{
				if (newrightnode.getWeight()+ matrix[currentlevel][1]<=W)
				{
				newrightnode.leftChild = Buildtree(newrightnode);
				}
			}
			if ((newrightnode.getBound()>maxP) && ((currentlevel+1)<=(n)) && (newrightnode.getProfit()+ matrix[currentlevel][2]>maxP))
			{
				newrightnode.rightChild = Buildtree(newrightnode);
			}
		}
		return r;	
	}
	
	
	static int Bound(int pro, int level, int w)
	{
		int sumweight=w;
		int sumprofit=pro;
		int k=0;
		int bound=0;
		int failure=w;
		if (sumweight<=W)
		{
			for(int i=level-1; i<n; i++)
			{
				if ((i+1)<n)
					failure= failure+matrix[i+1][1];
			}
			for(int i=level-1; i<n; i++)
			{
				if ((W-sumweight)>=0 && (i+1)<n)
				{ 
					if ((sumweight+matrix[i+1][1])>W)
					{	k=i+1;
						i=i+n;
					}
					else if ((sumweight+matrix[i+1][1])<=W)
					{
						sumweight= sumweight + matrix[i+1][1];
						sumprofit= sumprofit + matrix[i+1][2];
					}	
				}
			}
			if (failure<W)
				k=n+1;

			if (k>=n)
			{
				bound=( sumprofit);
			} else 
				bound=(( sumprofit) + ((W-sumweight) * matrix[k][3]));
		} else
			bound= 0;
		return bound;
	}					
						
	static void Plant(Node root){
		root = Buildtree(root); 
		System.out.println("maxP " + maxP);
	}						
						
	static Node root(){
		int rootbound = Bound(0,0,0);
		reference = new int [n];
		for (int j=0; j<n; j++)
		{
			reference[j]=1;
		}
		Node root = new Node(0,0,rootbound,0,1,null,null);
				System.out.println("Node(" + root.getLevel() + "," + root.getPosition() + ") has a profit, weight, and bound of " + 
						root.getProfit() + ", " + root.getWeight() + ", " + root.getBound());
		return root;
	}
	
	public static void main(String [] args)
	{
		n= matrix.length;
		Node seed = root();
		Plant(seed);
	}
}


//Node(0,1) has a profit, weight, and bound of 0, 0, 60
//Node(1,1) has a profit, weight, and bound of 20, 2, 60
//Node(2,1) has a profit, weight, and bound of 50, 7, 60
//Node(3,1) has a profit, weight, and bound of 50, 7, 58
//Node(4,1) has a profit, weight, and bound of 50, 7, 53
//Node(5,1) has a profit, weight, and bound of 53, 8, 53
//Node(5,2) has a profit, weight, and bound of 50, 7, 50
//Node(2,2) has a profit, weight, and bound of 20, 2, 55
//Node(3,2) has a profit, weight, and bound of 55, 9, 55
//Node(3,3) has a profit, weight, and bound of 20, 2, 35
//Node(1,2) has a profit, weight, and bound of 0, 0, 50
//maxProfit 55
