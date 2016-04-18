import java.util.*;
import java.io.*;
import java.net.*;
import java.lang.*;

public class Server implements Runnable{
	Entity ent;

	public ServerSocket  serverLibre(){
		ServerSocket Ssock=null;
		int port=1024;
		while(true){
			try{
				Ssock= new ServerSocket(port);
				ent.setTcp_port(port);
				break;
			}
			catch(Exception e){
				port++;
				if(port==1080)break;
			}
		}
		return Ssock;
	}

	public Server(Entity ent){
		this.ent=ent;

	}
	public void run() {
		try{
			ServerSocket Ssock=serverLibre();
			System.out.println(ent.getTcp_port());
			while(true){
				Socket socket= Ssock.accept();
				System.out.println("socket accepte");
				PrintWriter pw= new PrintWriter(socket.getOutputStream());
				pw.print("WELC "+" "+ent.getNextIp()+" "+ent.getUdp_port()+" "+ent.getCastIP()+" "+ent.getCastPort()+"\n");
				pw.flush();
				System.out.println("flush");
				BufferedReader br =new BufferedReader(new InputStreamReader(socket.getInputStream()));
				if(!tools.verif_mess_client(br.readLine(), ent))System.out.println("ceci ne correspond pas a un message d'insertion");
				else System.out.println("insertion");
				pw.close();
				br.close();
				socket.close();
			}
		}
		catch(Exception e){

		}
	}

}
