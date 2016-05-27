import java.util.*;
import java.io.*;
import java.net.*;
import java.lang.*;

public class Server implements Runnable{
	Entity ent;
	boolean quit=false;

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

	public void quit(){
		quit=true;
	}

	public void run() {
		try{
			ServerSocket Ssock=serverLibre();
			System.out.println(ent.getTcp_port());
			while(!quit){
				Socket socket= Ssock.accept();
				System.out.println("socket accepte");
				if(!ent.getdupl()){
					PrintWriter pw= new PrintWriter(socket.getOutputStream());

					pw.print("WELC "+ent.getNextIp()+" "+ent.getNextudp()+" "+ent.getCastIP()+" "+ent.getCastPort()+"\n");
					pw.flush();
					System.out.println("flush");
					BufferedReader br =new BufferedReader(new InputStreamReader(socket.getInputStream()));
					String s= br.readLine();
					System.out.println(s);
					if(tools.verif_mess_client(s, ent)){
						System.out.println("insertion");
						pw.print("ACKC\n");
						pw.flush();
					}
					else if(tools.verif_mess_dupl(s, ent)){
						System.out.println("duplication");
						pw.print("ACKD "+ent.getUdp_port()+"\n");
						pw.flush();
					}
					else{
						System.out.println("aucune insertion effectué");
					}


					pw.close();
					br.close();
				}else{
					PrintWriter pw=new PrintWriter(socket.getOutputStream());
					pw.print("NOTC");
					pw.flush();
					pw.close();
				}
				socket.close();
			}
			Ssock.close();
			Thread.currentThread().interrupt();
		}
		catch(Exception e){

		}
	}

}
