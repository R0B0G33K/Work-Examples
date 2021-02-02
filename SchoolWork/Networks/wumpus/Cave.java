package wumpus;

class Cave {
    Room[] rooms;

    public Cave() {
        this.rooms = new Room[20];

        this.rooms[0] = new Room(0, "zero", "room zero", new int[] {1, 4, 7});
        this.rooms[1] = new Room(1, "one", "room one", new int[] {0, 2, 9});
        this.rooms[2] = new Room(2, "two", "room two", new int[] {1, 3, 11});
        this.rooms[3] = new Room(3, "three", "room three", new int[] {2, 4, 13});
        this.rooms[4] = new Room(4, "four", "room four", new int[] {0, 3, 5});
        this.rooms[5] = new Room(5, "five", "room five", new int[] {4, 6, 14});
        this.rooms[6] = new Room(6, "six", "room six", new int[] {5, 7, 16});
        this.rooms[7] = new Room(7, "seven", "room seven", new int[] {0, 6, 8});
        this.rooms[8] = new Room(8, "eight", "room eight", new int[] {7, 9, 17});
        this.rooms[9] = new Room(9, "nine", "room nine", new int[] {1, 8, 10});
        this.rooms[10] = new Room(10, "ten", "room ten", new int[] {9, 11, 18});
        this.rooms[11] = new Room(11, "eleven", "room eleven", new int[] {2, 10, 12});
        this.rooms[12] = new Room(12, "twelve", "room twelve", new int[] {11, 13, 19});
        this.rooms[13] = new Room(13, "thirteen", "room thirteen", new int[] {3, 12, 14});
        this.rooms[14] = new Room(14, "fourteen", "room fourteen", new int[] {5, 13, 15});
        this.rooms[15] = new Room(15, "fifteen", "room fifteen", new int[] {14, 16, 19});
        this.rooms[16] = new Room(16, "sixteen", "room sixteen", new int[] {6, 15, 17});
        this.rooms[17] = new Room(17, "seventeen", "room seventeen", new int[] {8, 16, 18});
        this.rooms[18] = new Room(18, "eighteen", "room eighteen", new int[] {10, 17, 19});
        this.rooms[19] = new Room(19, "nineteen", "room nineteen", new int[] {12, 15, 18});
    }

    int[] getAdjacent(int room) { return this.rooms[room].getAdjacent(); }

    String getAdjacentString(int room) { 
        String a = "";
        boolean first = true;
        for (int rm: this.rooms[room].getAdjacent()) {
            if (first) { first = false; a += rm; }
            else { a += "," + rm; }
        } 
        return a;
    }
    
    boolean moveIsValid(int src, int dst) { 
        for (int r: this.rooms[src].getAdjacent()) {
            if ( r == dst ) { return true; }
        }
        return false;
    }
    
    String describe(int room) { return this.rooms[room].describe(); }
}
