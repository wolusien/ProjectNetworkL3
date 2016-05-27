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
	private Server sev;
	private message cli;
	private Multidiffusion muldiff;
	private Client client;
	public boolean actif=true;
	
	//debut duplication
	private int dupl_udp_port;
	private String dupl_udp_ip;
	private int dupl_cast_port;
	private String dupl_cast_ip;
	private boolean dupl=false;
	private Multidiffusion duplmuldiff;
	public boolean duplactif=false;
	
	

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
	public message getCli(){
		return this.cli;
	}
	public boolean getdupl(){
		return this.dupl;
	}
	public void duplication(){
		dupl=true;
		duplactif=true;
	}
	
	public void setdupl_muldiff(Multidiffusion muldiff){
		this.duplmuldiff=muldiff;
	}
	
	public int getdupl_udp_port(){
		return dupl_udp_port;
	}
	
	public int getdupl_cast_port(){
		return dupl_cast_port;
	}
	
	public String getdupl_udp_ip(){
		return dupl_udp_ip;
	}
	
	public String getdupl_cast_ip(){
		return dupl_cast_ip;
	}
	
	public void setdupl_udp_port(int port){
		this.dupl_udp_port=port;
	}
	
	public void setdupl_udp_ip(String ip){
		this.dupl_udp_ip=ip;
	}
	
	public void setdupl_cast_port(int port){
		this.dupl_cast_port=port;
	}
	
	public void setdupl_cast_ip(String ip){
		this.dupl_cast_ip=ip;
	}
	
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
	public void setMuldiff(Multidiffusion mult){
		this.muldiff=mult;
	}
	
	public void lanceDupl(){
		
	}
	public void Init(String idapp){
		this.setNextudp(udp_port);
		this.setNextIp(tools.ip());
		sev= new Server(this);
		cli= new message(this,idapp);
		System.out.println(this.next_ip);
		muldiff=new Multidiffusion(this);
		//this.cast_ip="226.0.0.0";
		//muldiff.setIp(this.cast_ip);
		this.cast_port=muldiff.portlibre();
		this.cast_ip=muldiff.iplibre();
		muldiff.setIp(this.cast_ip);
		client= new Client(this);
		lance_entity(sev,cli);
		lance_multidiffusion(muldiff);
		lanceClient(client);
	}
	
	public void lanceClient(Client client){
		try{
			Thread t= new Thread(client);
			t.start();
		}
		catch(Exception e){
			e.printStackTrace();
		}
	}
	
	public void lance_dupl_multidiffusion(Multidiffusion muldiff){
		try{
			Thread t4=new Thread(muldiff);
			t4.start();
		}
		catch(Exception e){
			e.printStackTrace();
		}
	}

	public void lance_multidiffusion(Multidiffusion muldiff){
		try{
			Thread t3= new Thread(muldiff);
			t3.start();
		}
		catch(Exception e){
			e.printStackTrace();
		}
	}

	public void lance_entity(Server sev,message m){
		try{
			Thread t1=new Thread(sev);
			Thread t2=new Thread( m);
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
	
	//interrompt tout
	public void quit(){
		this.actif=false;
		this.duplactif=false;
		if(cli!=null)
			cli.quit();
		if(sev!=null)
			sev.quit();
		if(muldiff!=null)
			muldiff.quit();
		if(duplmuldiff!=null)
			duplmuldiff.quit();
		System.exit(0);
		
	}


	public void insertion(String ip,int port, String idapp) {
		try{
			Socket socket=new Socket(ip,port);
			sev= new Server(this);
			cli= new message(this, idapp);
			client= new Client(this);
			lance_entity( sev, cli);
			lanceClient(client);
			System.out.println("entity ok");
			BufferedReader br=new BufferedReader(new InputStreamReader(socket.getInputStream()));
			PrintWriter pw=new PrintWriter(new OutputStreamWriter(socket.getOutputStream()));
			pw.print("NEWC "+tools.ip()+" "+this.getUdp_port()+"\n");
			pw.flush();
			System.out.println("flush "+this.getUdp_port());
			String mess=br.readLine();
			System.out.println("mess recu : "+mess);
			if(!tools.verif_mess_server(mess.trim(), this)){
				System.out.println("ceci ne correspond pas a un message d'insertion");
				quit();
			}
			pw.close();
			br.close();
			socket.close();
		}
		catch(Exception e){
			System.out.println(e);
			e.printStackTrace();
		}

	}
	

	public void duplication(String ip,int port) {
		try{
			Socket socket=new Socket(ip,port);
			System.out.println("entity ok");
			BufferedReader br=new BufferedReader(new InputStreamReader(socket.getInputStream()));
			PrintWriter pw=new PrintWriter(new OutputStreamWriter(socket.getOutputStream()));
			pw.print("DUPL "+tools.ip()+" "+this.getUdp_port()+" "+this.getCastIP()+" "+this.getCastPort()+"\n");
			pw.flush();
			System.out.println("flush "+this.getUdp_port());
			String mess=br.readLine();
			System.out.println("mess recu : "+mess);
			if(!tools.verif_dupl(mess.trim(), this)){
				System.out.println("ceci ne correspond pas a un message d'insertion");
				quit();
			}
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
