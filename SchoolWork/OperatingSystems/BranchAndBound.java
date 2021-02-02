import java.util.*;
import java.util.Comparator;
import java.util.PriorityQueue;

class BranchAndBound
{
	public static int maxProfit=0;
	public static int reference[];
	
	static int Bound(int pro, int level, int w, int W, int n, int matrix[][])
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
	
	public static void BestChildren(PriorityQueue<SSTNode> pQueue, int matrix[][], int W, int n)
	{
		SSTNode best = pQueue.peek();
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
			SSTNode newleftnode=new SSTNode(profitLeft,weightLeft,best.getBound(),currentlevel,positionLeft,null,null);
				
			if (newleftnode.getBound() > maxProfit && newleftnode.getWeight() <= W)
			{	
				pQueue.add(newleftnode);
				maxProfit= newleftnode.getProfit();
			}
					
			
			int positionRight=reference[currentlevel-1];
			reference[currentlevel-1]=(reference[currentlevel-1]+1);
			int boundRight = Bound(best.getProfit(),currentlevel,best.getWeight(),W,n,matrix);
			SSTNode newrightnode=new SSTNode(best.getProfit(),best.getWeight(),boundRight,currentlevel,positionRight,null,null);
				
			if (newrightnode.getBound() > maxProfit && newrightnode.getWeight() <= W)
			{
				pQueue.add(newrightnode);
				maxProfit= newrightnode.getProfit();
			}
		}
	}
	
	public static void main(String [] args)
	{
	int n=0;
	
	// int W = 30;
    // int matrix[][] = {{1,5,50,10},
					// {3,20,140,7},
					// {2,10,60,6}};			
	int W = 9;
    int matrix[][] = {{1,2,20,10},
					{2,5,30,6},
					{3,7,35,5},
					{4,3,12,4},
					{5,1,3,3}};	
					
	n= matrix.length;
	reference = new int [n];
		for (int j=0; j<n; j++)
		{
			reference[j]=1;
		}
	
	PriorityQueue<SSTNode> pQueue = new PriorityQueue<SSTNode>();
	boolean buildTree = true;
	
	int rootbound = Bound(0,0,0,W,n,matrix);
	SSTNode root = new SSTNode(0,0,rootbound,0,1,null,null);
	pQueue.add(root);
	SSTNode bestnode =root;
	SSTNode frontNode = null;
	
		while (buildTree==true)
		{
			BestChildren(pQueue, matrix, W, n);
			frontNode=pQueue.peek();
			if (pQueue.peek() ==null || frontNode.getBound() < maxProfit)
				buildTree=false;		
		}
	System.out.println("The max profit is: " + maxProfit);
	}
}

public class SSTNode implements Comparable<SSTNode>
{
	int profit;
	int weight;
	int bound;
	int level;
	int position;
	SSTNode leftChild;
	SSTNode rightChild;
	
	public SSTNode(int p, int w, int b, int l, int pos, SSTNode cl, SSTNode cr)
	{
		profit=p; weight=w; bound=b; level=l; position=pos; leftChild=cl; rightChild=cr;
	}
	
	public int getProfit() {return profit;}
	public int getWeight() {return weight;}
	public int getBound() {return bound;}
	public int getLevel() {return level;}
	public int getPosition() {return position;}
	public SSTNode getLeft() {return leftChild;}
	public SSTNode getRight() {return rightChild;}
	
	@Override
	public int compareTo(SSTNode x)
	{
		return x.getBound()-this.getBound();
	}
}