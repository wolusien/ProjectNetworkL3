import java.util.*;
import java.lang.*;
import java.net.*;
import java.io.*;

class Entity {
	//private byte[] id;
	private String id;
	private int udp_port;
	private int tcp_port;
	private String next_ip;
	private  int next_udp;
	private String cast_ip;
	private int cast_port;
	Thread t1; //server
	Thread t2; //message
	Thread t3; //multidiff

	/*	public Entity(String id, int prevudp_rport, int tcp_port, String ip_ad, int nextudp_sport, String ipv4_ad, int cast_port){
		this.id=id;
		this.udp_port=prevudp_rport;
		this.tcp_port=tcp_port;
		this.ip=ip_ad;
		this.next_udp=nextudp_sport;
		this.ipv4_ad=ipv4_ad;
		this.cast_port=cast_port;
	}
	 */
	public String getId(){
		return this.id;
	}

	public String getNextIp(){
		return this.next_ip;
	}
	public void setNextIp(String ip1){
		next_ip=ip1;
	}

	public int getNextudp(){
		return this.next_udp;
	}
	public void setNextudp(int udp){
		next_udp=udp;
	}
	public int getTcp_port(){
		return this.tcp_port;
	}
	public void setTcp_port(int port){
		this.tcp_port=port;
	}
	public int getUdp_port(){
		return this.udp_port;
	}
	public void setUdp_port(int port){
		this.udp_port=port;
	}


	public int getCastPort(){
		return this.cast_port;
	}
	public void setCastPort(int port){
		this.cast_port=port;
	}
	public String getCastIP(){
		return this.cast_ip;
	}
	public void setCastIp(String ip){
		this.cast_ip=ip;
	}


	public void Init(){
		this.setNextudp(udp_port);
		this.setNextIp(tools.ip());
		Server sev= new Server(this);
		message cli= new message(this);
		System.out.println(this.next_ip);
		Multidiffusion muldiff=new Multidiffusion(this);
		//this.cast_ip="226.0.0.0";
		//muldiff.setIp(this.cast_ip);
		this.cast_port=muldiff.portlibre();
		this.cast_ip=muldiff.iplibre();
		muldiff.setIp(this.cast_ip);

		lance_entity(sev,cli);
		lance_multidiffusion(muldiff);
	}

	public void lance_multidiffusion(Multidiffusion muldiff){
		try{
			t3= new Thread(muldiff);
			t3.start();
		}
		catch(Exception e){
			e.printStackTrace();
		}
	}

	public void lance_entity(Server sev,message m){
		try{
			t1=new Thread(sev);
			t2=new Thread( m);
			//t.    setDaemon(true);
			t1.start();
			t2.start();
			
			System.out.println("next udp : "+this.next_udp+" udp "+this.getUdp_port());
			
		}
		catch(Exception e){
			System.out.println(e);
			e.printStackTrace();
		}


	}
	
	//interrompt les threads
	public void quit(){
		t1.interrupt();
		t2.interrupt();
		t3.interrupt();
		
	}


	public void insertion(String ip,int port) {
		try{
			Socket socket=new Socket(ip,port);
			Server sev= new Server(this);
			message cli= new message(this);
			lance_entity( sev, cli);
			System.out.println("entity ok");
			BufferedReader br=new BufferedReader(new InputStreamReader(socket.getInputStream()));
			PrintWriter pw=new PrintWriter(new OutputStreamWriter(socket.getOutputStream()));
			pw.print("NEWC "+tools.ip()+" "+this.getTcp_port()+"\n");
			pw.flush();
			System.out.println("flush ");
			String mess=br.readLine();
			System.out.println("mess recu");
			if(!tools.verif_mess_server(mess, this)) System.out.println("ceci ne correspond pas a un message d'insertion");
			pw.close();
			br.close();
			socket.close();
		}
		catch(Exception e){
			System.out.println(e);
			e.printStackTrace();
		}

	}
}
