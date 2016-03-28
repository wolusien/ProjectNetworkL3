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

	public Entity(String id, int prevudp_rport, int tcp_port, String ip_ad, int nextudp_sport, String ipv4_ad, int cast_port){
		this.id=id;
		this.prevudp_rport=prevudp_rport;
		this.tcp_port=tcp_port;
		this.ip_ad=ip_ad;
		this.nextudp_sport=nextudp_sport;
		this.ipv4_ad=ipv4_ad;
		this.cast_port=cast_port;
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

	public void Init(){
		Server sev= new Server(this, this.prevudp_rport);
		Client cli= new Client(this, this.nextudp_sport, this.ip_ad);
	}

	public void run() {
		try{
			//ServerSocket Sersock= new ServerSocket(this.prevudp_rport);
			//Socket sock= new Socket(this.ip_ad, this.nextudp_sport);
			ServerSocket Ssock= new ServerSocket(this.tcp_port);
			while(true){
				Socket socket= Ssock.accept();
				PrintWriter pw= new PrintWriter(socket.getOutputStream());
				pw.print("WELC "+this.ip_ad+" "+this.nextudp_sport+" "+this.ipv4_ad+" "+this.cast_port+"\n");
				pw.flush();
				BufferedReader br =new BufferedReader(new InputStreamReader(socket.getInputStream()));
				String[] recu= br.readLine().split(" ");
				if(recu.length==3 && recu[0].equals("NEWC")){
					this.ip_ad=recu[1];
					this.nextudp_sport=Integer.parseInt(recu[2]);
					pw.print("ACKC\n");
					pw.flush();
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
