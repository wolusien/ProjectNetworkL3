import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;


public class Client implements Runnable{
	Entity ent;
	int port;
	String addr;
	
	public Client(Entity ent, int port, String addr){
		this.ent=ent;
		this.port=port;
		this.addr=addr;
	}
	

	public void run() {
		try{
			Socket sock= new Socket(addr, port);
			PrintWriter pw= new PrintWriter(sock.getOutputStream());
			Scanner sc= new Scanner(System.in);
			while(!sock.isClosed()){
				
			}
			sc.close();
			pw.close();
		}
		catch(Exception e){
			e.printStackTrace();
		}
		
	}
}
