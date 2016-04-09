import java.util.*;
import java.lang.*;
import java.net.*;
import java.io.*;

class Entity implements Runnable{
	private String id;
	private int prevudp_rport;
	private int tcp_port;
	private String ip_ad;
	private int nextudp_sport;
	private String ipv4_ad;
	private int cast_port;
	
	//contructeur pour nouvel anneau
	public Entity(String id, int prevudp_rport, int tcp_port, String ip_ad, int nextudp_sport, String ipv4_ad, int cast_port){
		this.id=id;
		this.prevudp_rport=prevudp_rport;
		this.tcp_port=tcp_port;
		this.ip_ad=ip_ad;
		this.nextudp_sport=nextudp_sport;
		this.ipv4_ad=ipv4_ad;
		this.cast_port=cast_port;
	}
	
	//contructeur pour insertion
	public Entity(String id, int prevudp_rport, int tcp_port, String ip_ad){
		this.id=id;
		this.prevudp_rport=prevudp_rport;
		this.tcp_port=tcp_port;
		this.ip_ad=ip_ad;
	}

	public String getId(){
		return this.id;
	}

	public int getPrevudp_rport(){
		return this.prevudp_rport;
	}

	public int getTcp_port(){
		return this.tcp_port;
	}

	public String getIp_ad(){
		return this.ip_ad;
	}

	public int getNextudp_sport(){
		return this.nextudp_sport;
	}

	public String getIpv4_ad(){
		return this.ipv4_ad;
	}

	public int getCast_port(){
		return this.cast_port;
	}

	public void setPrevudp_rport(int p){
		this.prevudp_rport=p;
	}

	public void setNextudp_sport(int p){
		this.nextudp_sport=p;
	}
	
	//change les params d insertion
	public boolean initInsert(String addr,int porttcp){
		try{
			Socket sock= new Socket(addr, porttcp);
			BufferedReader br= new BufferedReader(new InputStreamReader(sock.getInputStream()));
			PrintWriter pw= new PrintWriter(sock.getOutputStream());
			
			String[] recu=br.readLine().trim().split(" ");
			if(recu.length==5 && recu[0].equals("WELC")){
				String ip=this.ip_ad;
				this.ip_ad=recu[1];
				this.nextudp_sport= Integer.parseInt(recu[2]);
				this.ipv4_ad=recu[3];
				this.cast_port=Integer.parseInt(recu[4]);
				
				pw.print("NEWC "+ip+" "+this.prevudp_rport+"\n");
				pw.flush();
				
				String confirm= br.readLine().trim();
				if(!confirm.equals("ACKC")) return false;
				
			}
			else{
				pw.close();
				br.close();
				sock.close();
				return false;
			}
			pw.close();
			br.close();
			sock.close();
			return true;
		}
		catch(Exception e){
			e.printStackTrace();
			return false;
		}
	}

	//brouillon
	private void InitDemarre(){
		Server sev= new Server(this, this.prevudp_rport);
		Client cli= new Client(this, this.nextudp_sport, this.ip_ad);
	}
	
	/*public boolean verifMessage(String mess){
		String[] mss=mess.split(" ");
		
		return true;
	}*/
	
	
	/*
	 * doit demarrer Server et client
	 * verifie si quelqu un veut s inserer
	 */
	public void run() {
		try{
			ServerSocket Ssock= new ServerSocket(this.tcp_port);
			while(true){
				Socket socket= Ssock.accept();
				PrintWriter pw= new PrintWriter(socket.getOutputStream());
				pw.print("WELC "+this.ip_ad+" "+this.nextudp_sport+" "+this.ipv4_ad+" "+this.cast_port+"\n");
				pw.flush();
				BufferedReader br =new BufferedReader(new InputStreamReader(socket.getInputStream()));
				String[] recu= br.readLine().trim().split(" ");
				if(recu.length==3 && recu[0].equals("NEWC")){
					this.ip_ad=recu[1];
					this.nextudp_sport=Integer.parseInt(recu[2]);
					pw.print("ACKC\n");
					pw.flush();
					
					System.out.println("insertion réussi");
				}
				else{
					pw.print("Erreur message recu\n");
					pw.flush();
				}
				socket.close();

			}
		}
		catch(Exception e){
			e.printStackTrace();
		}
	}
}
