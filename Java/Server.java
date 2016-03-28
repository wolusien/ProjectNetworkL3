import java.util.*;
import java.io.*;
import java.net.*;
import java.lang.*;

public class Server implements Runnable{
	Entity ent;
	int port;
	
	public Server(Entity ent, int port){
		this.ent=ent;
		this.port=port;
	}
	public void run() {
		try{
			ServerSocket Ssock= new ServerSocket(port);
			while(true){
				Socket sock= Ssock.accept();
				
			}
		}
		catch(Exception e){
			
		}
	}
	
}
