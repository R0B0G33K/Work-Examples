package wumpus;

import java.net.*;
import java.io.*;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Dictionary;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Random;

class ClientHandler extends Thread
{
    final Socket conn;
    final String ip;
    int id;
    Game game;

    public ClientHandler(Socket conn, String ip, Game game)
    {
        this.conn = conn;
        this.ip = ip;
        this.game = game;
        this.id = this.game.getNextId();
    }

    @Override
    public void run()
    {
        String req;
        String res;

        while (true) {
            try {
              BufferedReader br = new BufferedReader(new InputStreamReader(conn.getInputStream()));
              OutputStream os = new PrintStream(conn.getOutputStream());

              req = br.readLine();
              System.out.println(ip + " - " + req);

              String[] parts = req.split("\\|");

              res = this.game.playTurn(this.id, parts[0], parts[1]);
              //System.out.println("Send:" + res + "#");
              os.write(res.getBytes("UTF-8"));
	          os.flush();

              if (parts[0].equals("QUIT")) { return; }

            } catch (IOException e) {
                e.printStackTrace();
                return;
            } 
        }

    }
}
