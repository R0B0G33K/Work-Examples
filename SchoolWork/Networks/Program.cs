using System;

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
  class Program
  {
    static void Main(string[] args)
    {
      DateTime now = DateTime.Now;
      Message HelloWorld = new Message("Hello World", now, "text");
      Console.WriteLine(HelloWorld);
      Console.ReadKey(); 
    }
  }
}
