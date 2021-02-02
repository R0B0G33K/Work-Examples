import java.util.*;

public class Backtracking
{
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
	StateSpaceTree birch = new StateSpaceTree(matrix,W,n);
	}
}
public class StateSpaceTree
{
	public static int maxProfit;
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

	public StateSpaceTree(int matrix[][], int W, int n)
	{
		int rootbound = Bound(0,0,0,W,n,matrix);
		reference = new int [n];
		for (int j=0; j<n; j++)
		{
			reference[j]=1;
		}
		SSTNode root = new SSTNode(0,0,rootbound,0,1,null,null);
				System.out.println("Node(" + root.getLevel() + "," + root.getPosition() + ") has a profit, weight, and bound of " + 
						root.getProfit() + ", " + root.getWeight() + ", " + root.getBound());
		root = Buildtree(root, matrix, W, n); 
		System.out.println("maxProfit " + maxProfit);
	}
	
	static SSTNode Buildtree(SSTNode r, int matrix[][], int W, int n)
	{
		
			int currentlevel= r.getLevel() +1;
			
			int profitLeft=(r.getProfit() + matrix[currentlevel-1][2]);
			int weightLeft = (r.getWeight() + matrix[currentlevel-1][1]);
	//creates the children of the left child
		if (currentlevel<n+1)
		{
			if (weightLeft<=W)
			{
				int positionLeft=reference[currentlevel-1];
				reference[currentlevel-1]=(reference[currentlevel-1]+1);
				SSTNode newleftnode=new SSTNode(profitLeft,weightLeft,r.getBound(),currentlevel,positionLeft,null,null);
					System.out.println("Node(" + newleftnode.getLevel() + "," + newleftnode.getPosition() + ") has a profit, weight, and bound of " + 
								newleftnode.getProfit() + ", " + newleftnode.getWeight() + ", " + newleftnode.getBound());
				if (newleftnode.getProfit() > maxProfit && newleftnode.getBound() !=0)
					maxProfit= newleftnode.getProfit();
				if (((currentlevel+1)<(n)) && (newleftnode.getProfit()+ matrix[currentlevel][2]>maxProfit) && (newleftnode.getWeight()+ matrix[currentlevel+1][1]<=W))
				{
					newleftnode.leftChild = Buildtree(newleftnode, matrix, W, n);
				}
				if ((newleftnode.getBound()>maxProfit) && ((currentlevel+1)<=(n)) && (newleftnode.getProfit()+ matrix[currentlevel][2]>maxProfit))
				{
					newleftnode.rightChild = Buildtree(newleftnode, matrix, W, n);
				}
			}
	// creates the children of the right child
			int positionRight=reference[currentlevel-1];
			reference[currentlevel-1]=(reference[currentlevel-1]+1);
			int boundRight = Bound(r.getProfit(),currentlevel,r.getWeight(),W,n,matrix);
			SSTNode newrightnode=new SSTNode(r.getProfit(),r.getWeight(),boundRight,currentlevel,positionRight,null,null);
					System.out.println("Node(" + newrightnode.getLevel() + "," + newrightnode.getPosition() + ") has a profit, weight, and bound of " + 
							newrightnode.getProfit() + ", " + newrightnode.getWeight() + ", " + newrightnode.getBound());
			if (newrightnode.getProfit() > maxProfit && newrightnode.getBound() !=0)
				maxProfit= newrightnode.getProfit();
			if (((currentlevel+1)<=(n)) && (newrightnode.getBound()>maxProfit))
			{
				if (newrightnode.getWeight()+ matrix[currentlevel][1]<=W)
				{
				newrightnode.leftChild = Buildtree(newrightnode, matrix, W, n);
				}
			}
			if ((newrightnode.getBound()>maxProfit) && ((currentlevel+1)<=(n)) && (newrightnode.getProfit()+ matrix[currentlevel][2]>maxProfit))
			{
				newrightnode.rightChild = Buildtree(newrightnode, matrix, W, n);
			}
		}
		return r;	
	}
}
