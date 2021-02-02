import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.PriorityQueue;


public class Sim {
    public static int processes;
    public static List<P> procs = new ArrayList<P>();

    public static void main(String args[]) {
        init(); // read process data
        System.out.println("Read " + processes + " processes to schedule");
        fcfs(); // first come first served
        sjf();  // shortest job first
        rr();   // round robin
    }

    // Read process data from a file
    public static void init() {
        String line = null;
        try {
            FileReader fileReader = new FileReader("procs.txt");
            BufferedReader bufferedReader = new BufferedReader(fileReader);
            int i=0;

            // input file format:
            // int arrivalTime <space> int burst <space> int priority
            while((line = bufferedReader.readLine()) != null) {
                String[] parts = line.split(" ");
                procs.add(new P(i, Integer.parseInt(parts[0]), Integer.parseInt(parts[1]), Integer.parseInt(parts[2])));
                i++;
            }
            bufferedReader.close();

            // set our static number of processes
            processes = i;

        } catch(FileNotFoundException ex) {
            System.out.println( "File not found.");
        } catch(IOException ex) {
            ex.printStackTrace();
        }
    }
	
	

    // First Come First Served, a.k.a. FIFO
    public static void fcfs() {
        System.out.print("\nFCFS:\n");
		double wait = 0;
		double turnaround = 0;
		double response = 0;
		double totaltime = 0;
		for (int i = 0; i < procs.size(); i++){ 
			wait = wait + totaltime;
			response = response + (totaltime - procs.get(i).getArrival());
			totaltime = totaltime + procs.get(i).getBurst();
			turnaround = turnaround + (totaltime - procs.get(i).getArrival());
		}
		System.out.println("avg turnaround time: " + turnaround/procs.size());
		System.out.println("avg response time: " + response/procs.size());
		System.out.println("avg wait time: " + wait/procs.size());
    }

    // Shortest Job First
    public static void sjf() {
		Collections.sort(procs, new burstComparator());
        System.out.print("\nSJF:\n");
		double wait = 0;
		double turnaround = 0;
		double response = 0;
		double totaltime = 0;
		for (int i = 0; i < procs.size(); i++){ 
			wait = wait + totaltime;
			response = response + (totaltime - procs.get(i).getArrival());
			totaltime = totaltime + procs.get(i).getBurst();
			turnaround = turnaround + (totaltime - procs.get(i).getArrival());
		}
		System.out.println("avg turnaround time: " + turnaround/procs.size());
		System.out.println("avg response time: " + response/procs.size());
		System.out.println("avg wait time: " + wait/procs.size());
    }

    // Round Robin
    public static void rr() {
		Collections.sort(procs, new priorityComparator());
        System.out.print("\nRR:\n");
		int tq = 10;
		double wait = 0;
		double turnaround = 0;
		double response = 0;
		double totaltime = 0;
		int btimeLeft;
		int finished = 0;
		int loop = 1;
		while(finished != procs.size()){
			for (int i = 0; i < procs.size(); i++){
				if(procs.get(i).getCompletion()){}
				else{
					if(loop ==1){
						response = response + totaltime;
					}
					totaltime = totaltime + tq;
					//System.out.println("total time "+totaltime);
					btimeLeft = procs.get(i).getBurst();
					procs.get(i).work(tq);
					//System.out.println("burst "+procs.get(i).getBurst());
					if(procs.get(i).getCompletion()){
						totaltime = totaltime-(tq-btimeLeft); //fixes the time mistake
						if(loop ==1){
							response = response - tq + totaltime; //fixes response time if process < tq
						}
						//System.out.println("total time (completed) "+totaltime);
						turnaround = turnaround + (totaltime - procs.get(i).getArrival());
						wait = wait + (totaltime-(tq*loop-(tq-btimeLeft)));
						finished++;
					}
				}
			}
			loop++;
		}
		System.out.println("avg turnaround time: " + turnaround/procs.size());
		System.out.println("avg response time: " + response/procs.size());
		System.out.println("avg wait time: " + wait/procs.size());
    }
}