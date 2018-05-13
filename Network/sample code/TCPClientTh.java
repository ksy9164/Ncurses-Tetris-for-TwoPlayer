import java.io.*; 
import java.net.*; 
class TCPClientTh { 

    public static void main(String argv[]) throws Exception 
    { 
        String sentence; 
        String modifiedSentence; 

        BufferedReader inFromUser = 
          new BufferedReader(new InputStreamReader(System.in)); 

        Socket clientSocket = new Socket("localhost", 6789); 

        DataOutputStream outToServer = 
          new DataOutputStream(clientSocket.getOutputStream()); 

	BufferedReader inFromServer = 
          new BufferedReader(new
          InputStreamReader(clientSocket.getInputStream())); 

        while(true)
	{
        sentence = inFromUser.readLine(); 
        if(sentence.length() == 0)
		break;

        outToServer.writeBytes(sentence + "\r\n"); 

        modifiedSentence = inFromServer.readLine(); 

        System.out.println("FROM SERVER: " + modifiedSentence); 
	}
        clientSocket.close(); 

                   
    } 
} 
