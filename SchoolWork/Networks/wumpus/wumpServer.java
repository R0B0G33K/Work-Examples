package wumpus;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class wumpServer {

    private static int port = 9876;
    private static ServerSocket serverSocket;
    private static ClientHandler clientHandler;
    private static Thread thread;
    private static Socket conn;

    public static void main(String[] args) throws IOException {
        if (args.length == 1) {
            int desiredPort = Integer.parseInt(args[0]);
            if (desiredPort > 1023 && desiredPort < 65536) { port = desiredPort; }
            else { System.out.println("Invalid port, trying the default port instead."); }
        }

        serverSocket = new ServerSocket(port);
        System.out.println("Listening for connections on port " + port);

        Game game = new Game();

        while(true) {
            conn = serverSocket.accept();
            String ip = conn.getInetAddress().getHostAddress();
            System.out.println( ip + " client accepted");
            clientHandler = new ClientHandler(conn, ip, game);
            thread = new Thread( clientHandler );
            thread.start();
        }
    }

}
