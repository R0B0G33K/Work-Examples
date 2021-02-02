package wumpus;

import java.util.Random;

class Player {
    int id;
    String name;
    int location;
    int arrows;

    Player(int id, String name) {
      this.id = id;
      this.name = name;
      this.arrows = 5;

      Random rand = new Random();
      int rm = rand.nextInt(20);
      //while(rm == game.wumpus.location) { rm = rand.nextInt(20); }
      this.location = rm;
    }

    int getLocation() { return this.location; }
    void setLocation(int loc) { this.location = loc; }
    int getArrows() { return this.arrows; }
    void setArrows(int arrows) { this.arrows = arrows; }
    String getName() { return this.name; }

    void move(int location) { this.location = location; }

    boolean shoot() { 
        if (this.arrows > 0) {
            this.arrows -= 1;
            return true;
        }
        return false;
    }
}
