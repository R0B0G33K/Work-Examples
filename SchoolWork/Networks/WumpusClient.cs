using System;
using System.Text;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Collections.Generic;

namespace Networks
{
    class Message
    {
        String command;
        String target;
        int numBytes;
        String messages;

        public Message() { }

        public Message (String data)
        {
            String[] lines = data.Split("\n");
            String[] parts = lines[0].Split("|");

            this.command = parts[0];
            this.target = parts[1];
            this.numBytes = Int32.Parse(parts[2]);
            this.messages = "";

            if(this.numBytes > 0)
            {
                for(int i = 1; i < lines.Length; i++)
                {
                    this.messages += lines[i] + "\n";
                }
            }
        }

        public void join(String name)
        {
            this.command = "JOIN";
            this.target = name;
            this.numBytes = 0;
        }

        public void shoot(String target)
        {
            this.command = "SHOOT";
            this.target = target;
            this.numBytes = 0;
        }

        public void move(String target)
        {
            this.command = "MOVE";
            this.target = target;
            this.numBytes = 0;
        }

        public void help()
        {
            this.command = "HELP";
            this.target = "-";
            this.numBytes = 0;
        }

        public void quit()
        {
            this.command = "QUIT";
            this.target = "-";
            this.numBytes = 0;
        }

        public String commandLine()
        {
            String commandLine = "";
            commandLine += this.command + "|";
            commandLine += this.target + "|";
            commandLine += this.numBytes + "\n";
            return commandLine;
        }

        public String getTarget()
        {
            return this.target;
        }

        public String getCommand()
        {
            return this.command;
        }

        public override string ToString()
        {
            return this.messages;
        }

    }

    class WumpusClient
    {
        private static Socket ConnectSocket(string server, int port)
        {
            Socket s = null;
            IPHostEntry hostEntry = null;

            // Get host related information.
            hostEntry = Dns.GetHostEntry(server);

            // Loop through the AddressList to obtain the supported AddressFamily. This is to avoid
            // an exception that occurs when the host IP Address is not compatible with the address family
            // (typical in the IPv6 case).
            foreach (IPAddress address in hostEntry.AddressList)
            {
                IPEndPoint ipe = new IPEndPoint(address, port);
                Socket tempSocket =
                    new Socket(ipe.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

                tempSocket.Connect(ipe);

                if (tempSocket.Connected)
                {
                    s = tempSocket;
                    break;
                }
                else
                {
                    continue;
                }
            }
            return s;
        }

        static void Main(string[] args)
        {
            using (Socket client = ConnectSocket("localhost", 9876))
            {
                if (client == null)
                {
                    Console.WriteLine("Connection failed");
                    return;
                }

                Message firstmessage = new Message();
                Console.WriteLine("Welome! Enter your name: ");
                String name = Console.ReadLine();
                firstmessage.join(name);
                String msg = firstmessage.commandLine();
                Byte[] toSendOut = Encoding.UTF8.GetBytes(msg);
                client.Send(toSendOut, toSendOut.Length, 0);
                bool variableQuit = false;

                while (!variableQuit)
                {
                    Byte[] bytesRecieved = new Byte[1024];
                    String answer = "";

                    int bytes = client.Receive(bytesRecieved, bytesRecieved.Length, 0);
                    answer = answer + Encoding.UTF8.GetString(bytesRecieved, 0, bytes);

                    Message newMessage = new Message(answer);
                    Console.WriteLine(newMessage);

                    Message newRequest = new Message();

                    if (newMessage.getCommand() == "300" || newMessage.ToString() == "You win!")
                    {
                        newRequest.quit();
                        String commandMessage = newRequest.commandLine();
                        Byte[] sendingCommand = Encoding.UTF8.GetBytes(commandMessage);
                        client.Send(sendingCommand, sendingCommand.Length, 0);
                        break;
                    }

                    while (true)
                    {
                        Console.WriteLine("Move or Shoot?");
                        String input = Console.ReadLine();
                        if (input.ToLower() == "shoot" || input.ToLower() == "s")
                        {
                            Console.WriteLine("What room do you shoot?");
                            String input2 = Console.ReadLine();

                            newRequest.shoot(input2);
                            String commandMessage = newRequest.commandLine();
                            Byte[] sendingCommand = Encoding.UTF8.GetBytes(commandMessage);
                            client.Send(sendingCommand, sendingCommand.Length, 0);
                            break;

                        }
                        else if (input.ToLower() == "move" || input.ToLower() == "m")
                        {
                            Console.WriteLine("Move to which room?");
                            String input2 = Console.ReadLine();

                            newRequest.move(input2);
                            String commandMessage = newRequest.commandLine();
                            Byte[] sendingCommand = Encoding.UTF8.GetBytes(commandMessage);
                            client.Send(sendingCommand, sendingCommand.Length, 0);
                            break;
                        }
                        else if (input.ToLower() == "help")
                        {
                            newRequest.help();
                            String commandMessage = newRequest.commandLine();
                            Byte[] sendingCommand = Encoding.UTF8.GetBytes(commandMessage);
                            client.Send(sendingCommand, sendingCommand.Length, 0);
                            break;
                        }
                        else if (input.ToLower() == "quit")
                        {
                            newRequest.quit();
                            String commandMessage = newRequest.commandLine();
                            Byte[] sendingCommand = Encoding.UTF8.GetBytes(commandMessage);
                            client.Send(sendingCommand, sendingCommand.Length, 0);
                            variableQuit = true;
                            break;
                        }
                        else
                        {
                            Console.WriteLine("Please enter a valid command.");
                        }
                    }
                }
            }
        }
    }
}
