import java.util.*;

public class MyBacktrackingBranchBound
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
	
	public static void BestChildren(PriorityQueue<Node> pQueue)
	{
		Node best = pQueue.peek();
		pQueue.remove(best);
		System.out.println("Node(" + best.getLevel() + "," + best.getPosition() + ") has a profit, weight, and bound of " + 
							best.getProfit() + ", " + best.getWeight() + ", " + best.getBound());
		if (best.getLevel() < n)
		{
			int currentlevel= best.getLevel() +1;
		
			int profitLeft=(best.getProfit() + matrix[currentlevel-1][2]);
			int weightLeft = (best.getWeight() + matrix[currentlevel-1][1]);
			int positionLeft=reference[currentlevel-1];
			reference[currentlevel-1]=(reference[currentlevel-1]+1);
			Node newleftnode=new Node(profitLeft,weightLeft,best.getBound(),currentlevel,positionLeft,null,null);
				
			if (newleftnode.getBound() > maxP && newleftnode.getWeight() <= W)
			{	
				pQueue.add(newleftnode);
				maxP= newleftnode.getProfit();
			}
					
			
			int positionRight=reference[currentlevel-1];
			reference[currentlevel-1]=(reference[currentlevel-1]+1);
			int boundRight = Bound(best.getProfit(),currentlevel,best.getWeight());
			Node newrightnode=new Node(best.getProfit(),best.getWeight(),boundRight,currentlevel,positionRight,null,null);
				
			if (newrightnode.getBound() > maxP && newrightnode.getWeight() <= W)
			{
				pQueue.add(newrightnode);
				maxP= newrightnode.getProfit();
			}
		}
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
	
	public static void main(String [] args)
	{
		PriorityQueue<Node> pQueue = new PriorityQueue<Node>();
		boolean buildTree = true;
		n= matrix.length;
		
		int rootbound = Bound(0,0,0);
	Node root = new Node(0,0,rootbound,0,1,null,null);
	pQueue.add(root);
	Node bestnode =root;
	Node frontNode = null;
	
		while (buildTree==true)
		{
			BestChildren(pQueue);
			frontNode=pQueue.peek();
			if (pQueue.peek() ==null || frontNode.getBound() < maxP)
				buildTree=false;		
		}
	System.out.println("The max profit is: " + maxP);
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