import java.util.Comparator;

class P implements Comparable<P>{
    int arrivalTime;
    int pid;
    int burst;
    int priority;
    boolean complete;

    public P(int pid, int arrivalTime, int burst, int priority) {
        this.pid = pid;
        this.arrivalTime = arrivalTime;
        this.burst = burst;
        this.priority = priority;
        this.complete = false;
    }

    public void work(int cputime) {
        if (burst > cputime) {
            burst -= cputime;
        } else {
            burst = 0;
            complete = true;
        }
    }

    public String toString() {
        return new String("PID: "+pid+" ["+burst+"]");
    }
	
	public boolean getCompletion() {
        return complete;
    }
	
	public int getBurst() {
        return burst;
    }
	
	public int getPriority() {
        return priority;
    }
	
	public int getArrival() {
        return arrivalTime;
    }

    // Overriding the compareTo method
    public int compareTo(P p){
        return this.burst - p.burst;
    }
}

class burstComparator implements Comparator<P> {
    public int compare(P p1, P p2) {
        return p1.burst - p2.burst;
    }
}

class priorityComparator implements Comparator<P> {
    public int compare(P p1, P p2) {
        return p1.priority - p2.priority;
    }
}
