package wumpus;

import java.io.*;
import java.net.*;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Map;
import java.util.Random;
import java.util.Vector;

public class Game {
  Cave cave;
  // static Wumpus wumpus;
  int wumpusLocation;
  Vector<Player> players;
  HashMap<Integer, Integer> playerDirectory = new HashMap<Integer, Integer>();
  int playerId;

  public static final String welcome =
      new String(
          "You're in a cave with 20 rooms and 3 tunnels leading from each room. \n"
	  + "There are 3 bats and 3 pits scattered throughout the cave, and your\n"
	  + "quiver holds 5 custom super anti-evil Wumpus arrows. Good luck.");

  public static final String eatenArrows =
      new String(
          "You turn and look at your quiver, and realize with a sinking feeling that you've just"
              + " shot your last arrow (figuratively, too).  Sensing this with its psychic powers,"
              + " the evil Wumpus rampagees through the cave, finds you, and with a mighty *ROAR*"
              + " eats you alive!");

  public static final String eatenMove =
      new String(
          "*ROAR* *chomp* *snurfle* *chomp*! "
              + "Much to the delight of the Wumpus, you walked right into his mouth, "
              + "making you one of the easiest dinners he's ever had!  For you, however, "
              + "it's a rather unpleasant death.  The only good thing is that it's been "
              + "so long since the evil Wumpus cleaned his teeth that you immediately "
              + "passed out from the stench!");

  public static final String winner = new String("You win!");
  public static final String wall = new String("*Oof!*  (You hit the wall)");
  public static final String toobig = new String("What?  The cave surely isn't quite that big!");

  public static final String help =
      new String(
          "Welcome to the game of Hunt the Wumpus.\n\n"
              + "The Wumpus typically lives in a cave of twenty rooms, with each room having\n"
              + "three tunnels connecting it to other rooms in the cavern.  Caves may vary,\n"
              + "however, depending on options specified when starting the game.\n\n"
              + "The game has the following hazards for intrepid adventurers to wind their\n"
              + "way through:\n\n"
              + " Pits   -- If you fall into one of the bottomless pits, you find yourself\n"
              + "           slung back out on the far side of the Earth and in very poor\n"
              + "           shape to continue your quest since you're dead.\n\n"
              + " Bats   -- As with any other cave, the Wumpus cave has bats in residence.\n"
              + "           These are a bit more potent, however, and if you stumble into\n"
              + "           one of their rooms they will rush up and carry you elsewhere in\n"
              + "           the cave.\n\n"
              + " Wumpus -- If you happen to walk into the room the Wumpus is in you'll find\n"
              + "           that he has quite an appetite for young adventurous humans!  Not\n"
              + "           recommended.\n\n"
              + "The Wumpus, by the way, is not bothered by the hazards since he has sucker\n"
              + "feet and is too big for a bat to lift.  If you try to shoot him and miss,\n"
              + "there's also a chance that he'll up and move himself into another cave,\n"
              + "though by nature the Wumpus is a sedentary creature.\n\n"
              + "Each turn you may either move or shoot a crooked arrow.  Moving is done\n"
              + "simply by specifying \"m\" for move and the number of the room that you'd\n"
              + "like to move down a tunnel towards.  Shooting is done similarly; indicate\n"
              + "that you'd like to shoot one of your magic arrows with an \"s\" for shoot,\n"
              + "then list a set of connected room numbers through which the deadly shaft\n"
              + "should fly!\n\n"
              + "If your path for the arrow is incorrect, however, it will flail about in\n"
              + "the room it can't understand and randomly pick a tunnel to continue\n"
              + "through.  You might just end up shooting yourself in the foot if you're\n"
              + "not careful!  On the other hand, if you shoot the Wumpus you've WON!\n\n"
              + "Good luck.");

  public static final String pattern = new String("dd/MMM/yyyy hh:mm:ss");
  public static final Hashtable<Integer, String> statuses;

  static {
    statuses =
        new Hashtable<Integer, String>() {
          {
            put(200, "Ok");
            put(201, "Created");
            put(301, "Moved permanently");
            put(400, "Bad request");
            put(404, "File not found");
            put(409, "Conflict");
            put(429, "Too many requests");
            put(500, "Internal Server Error");
          }
        };
  }

  public Game() {
    this.cave = new Cave();
    // wumpus = new Wumpus();
    this.players = new Vector<Player>();
    this.playerId = 1;
    Random rand = new Random();
    this.wumpusLocation = rand.nextInt(20);
    System.out.println("Wumpus location: " + this.wumpusLocation);
  }

  synchronized int getNextId() {
    int i = playerId;
    playerId += 1;
    return i;
  }

  synchronized boolean add_player(Player p) {
    if (this.players.add(p)) {
      int index = this.players.indexOf(p);
      this.playerDirectory.put(p.id, index);
      System.out.println(p.name + " has joined the hunt! [id: " + p.id + "]");
      return true;
    }

    return false;
  }

  synchronized boolean remove_player(int pid) {
    int index = this.playerDirectory.get(pid);
    String name = this.players.get(index).getName();
    this.players.removeElementAt(index);
    this.playerDirectory.remove(pid);
    System.out.println(name + " has abandoned the hunt!");
    return true;
  }

  String adjacentToString(int room) {
    String[] adjacent = cave.getAdjacentString(room).split(",");
    String msg = "There are tunnels to rooms ";
    int i=0;
    for (i=0; i < adjacent.length-1; i++) {
        msg += adjacent[i] + ", ";
    }
    msg += "and " + adjacent[i] + ".\n";
    return msg;
  }

  // static public HashMap<String, int> commands = new HashMap<String, int> ();
  public static Map<String, Integer> commands =
      Map.of("JOIN", 0, "MOVE", 1, "SHOOT", 2, "HELP", 3, "QUIT", 4);

  String playTurn(int pid, String command, String target) {
    String res = "";
    switch (commands.get(command)) {
      case 0:
        Player p = new Player(pid, target);
        if (this.add_player(p)) {
          System.out.println(target + " is in room: " + p.getLocation());
        }
        String m = this.welcome + "\n\n" + "You are in room "+ p.getLocation() +" of the cave, and have "+ 
		   p.getArrows() +" arrows left.\n" + adjacentToString(p.getLocation());
        res += "201|" + cave.getAdjacentString(p.getLocation()) + "|" + m.length() + "\n" + m;
        break;
      case 1: // MOVE
        int dst = Integer.parseInt(target);
        int index = this.playerDirectory.get(pid);
        int src = this.players.get(index).getLocation();
        if (cave.moveIsValid(src, dst)) {
          this.players.get(index).setLocation(dst);
	      String status = "You are in room "+ dst +" of the cave, and have "+ this.players.get(index).getArrows() +" arrows left.\n" + adjacentToString(dst);
          if (this.wumpusLocation == dst) {
            res +=
                "300|"
                    + cave.getAdjacentString(dst)
                    + "|"
                    + this.eatenMove.length()
                    + "\n"
                    + this.eatenMove;
          } else {
            res +=
                "200|"
                    + cave.getAdjacentString(dst)
                    + "|"
                    + status.length()
                    + "\n"
                    + status;
          }
        } else {
	      String status = "You are in room "+ src +" of the cave, and have "+ this.players.get(index).getArrows() +" arrows left.\n" + adjacentToString(src);
          if (dst > 20) {
            status += this.toobig;
            res += "500|" + cave.getAdjacentString(src) + "|" + status.length() + "\n" + status;
          } else {
            status += this.wall;
            res += "500|" + cave.getAdjacentString(src) + "|" + status.length() + "\n" + status;
          }
        }
        break;
      case 2: // SHOOT
        int dst2 = Integer.parseInt(target);
        int index2 = this.playerDirectory.get(pid);
        int src2 = this.players.get(index2).getLocation();
        if (cave.moveIsValid(src2, dst2)) {
          boolean hasArrows = this.players.get(index2).shoot();
	      String status = "You are in room "+ src2 +" of the cave, and have "+ this.players.get(index2).getArrows() +" arrows left.\n" + adjacentToString(src2);
          if ((this.wumpusLocation == dst2) && hasArrows) {
            res +=
                "100|"
                    + cave.getAdjacentString(src2)
                    + "|"
                    + this.winner.length()
                    + "\n"
                    + this.winner;
          } else if (hasArrows) {
            res +=
                "100|"
                    + cave.getAdjacentString(src2)
                    + "|"
                    + status.length()
                    + "\n"
                    + status;
          } else {
            res += "99|-|" + this.eatenArrows.length() + "\n" + this.eatenArrows;
          }
        } else {
          // *thunk*  The arrow can't find a way from 3 to 19 and flys randomly into room #!
          res += "SHOOTing|-|0\n";
        }
        break;
      case 3:
        res += "101|-|" + this.help.length() + "\n" + this.help;
        break;
      case 4:
        if (remove_player(pid)) {
          res += "0|-|0\n";
        } else {
          res += "ERR";
        }
        break;
      default:
        System.out.println("ERR");
    }
    System.out.println("[playTurn]: " + res);
    return res;
  }
}
