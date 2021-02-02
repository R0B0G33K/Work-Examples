package wumpus;

import java.util.Collection;

class Room {
    int id;
    String name;
    String description;
    int [] adjacent;

    public Room(int id, String name, String description, int [] adjacent) {
        this.id = id;
        this.name = name;
        this.description = description;
        this.adjacent = new int [adjacent.length];
        for(int i=0; i<adjacent.length; i++) { this.adjacent[i] = adjacent[i]; }
    }

    public int[] getAdjacent() { return this.adjacent; }
    
    public boolean isAdjacent(int roomId) {
        // for (int room : this.adjacent) {
        for(int i=0; i<this.adjacent.length; i++) {
            if (adjacent[i] == roomId) { return true; }
        }
        return false;
    }


    public void setDescription(String description) {
        this.description = description;
    }

    public String describe() {
        String d = new String(this.name + "\nadjacent");
        for (int room : this.adjacent) {
            d += " " + room;
        }

        return d;
    }

    public String toString() {
        String s = "Room[" + id + "]: adjacent: [" + this.adjacent[0]; 
        for (int i=1; i<this.adjacent.length; i++) { s += ", " + this.adjacent[i]; }
        s += "] - " + this.name;
        return s;
    }
}
