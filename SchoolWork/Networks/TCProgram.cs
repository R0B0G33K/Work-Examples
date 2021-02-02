using System;
using System.Net.Sockets;

namespace Networks
{
    class Message
  {
    public string message;
    public DateTime date;
    public string contentType;
    
    public Message(string contentMessage, DateTime Date, string Type)
    {
      message = contentMessage;
      date = Date;
      contentType = Type;
    } 

    public override string ToString()
    {
        return "Message: " + message + "  Date & Time: " + date + "  Content Type: " + contentType;
    }
  }
  class TCProgram
    {
    static void Main(string[] args)
    {
      DateTime now = DateTime.Now;
      Message HelloWorld = new Message("Hello World", now, "text");
      try
      {
        Int32 port = 43500;
        String server = "thomas.butler.edu";

        TcpClient client = new TcpClient(server, port);
        
        Byte[] data = System.Text.Encoding.ASCII.GetBytes(HelloWorld.ToString());

        NetworkStream stream = client.GetStream();

        stream.Write(data, 0, data.Length);

        Console.WriteLine("Sent: {0}", HelloWorld.ToString());
        stream.Close();
        client.Close();
        Console.ReadKey();
      }
      catch
      {
        Console.WriteLine("Error: Could not send message");
        Console.ReadKey();
      }
    }
  }
}
