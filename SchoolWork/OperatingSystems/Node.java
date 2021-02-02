public class Node implements Comparable<Node>
{
	int profit;
	int weight;
	int bound;
	int level;
	int position;
	Node leftChild;
	Node rightChild;
	
	public Node(int p, int w, int b, int l, int pos, Node cl, Node cr)
	{
		profit=p; weight=w; bound=b; level=l; position=pos; leftChild=cl; rightChild=cr;
	}
	
	public int getProfit() {return profit;}
	public int getWeight() {return weight;}
	public int getBound() {return bound;}
	public int getLevel() {return level;}
	public int getPosition() {return position;}
	public Node getLeft() {return leftChild;}
	public Node getRight() {return rightChild;}
	
	@Override
	public int compareTo(Node x)
	{
		return x.getBound()-this.getBound();
	}
}