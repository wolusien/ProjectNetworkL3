import java.net.*;
//import java.util.ArrayList;


public class Multidiffusion implements Runnable{
	int port;
	String ip;
	Entity ent;

	public Multidiffusion(int port, String ip){
		this.port=port;
		this.ip=ip;
		//listEnt= new ArrayList<Entity>();
	}
	
	public Multidiffusion(Entity ent){
		this.ent=ent;
	}
	
	public void setIp(String ip){
		this.ip=ip;
	}
	
	public void setPort(int port){
		this.port=port;
	}
	
	public int portlibre(){
		MulticastSocket mso=null;
		this.port=1024;
		while(true){
			try{
				mso=new MulticastSocket(port);
				mso.close();
				break;
			}
			catch(Exception e){
				port++;
				if(port==9999)break;
			}
		}
		return port;
	}

	public void run() {
		try{
			MulticastSocket mso=new MulticastSocket(port);
			System.out.println(ip);
			mso.joinGroup(InetAddress.getByName(ip));
			byte[]data=new byte[512];
			DatagramPacket paquet=new DatagramPacket(data,data.length);
			while(true){
				mso.receive(paquet);
				String st=new String(paquet.getData(),0,paquet.getLength());
				System.out.println("message recu : "+st);
			}
		} catch(Exception e){
			e.printStackTrace();
		}
	}

	//envoie multicast
	public void down(){
		try{
			DatagramSocket dso=new DatagramSocket();
			byte[]data = new byte[255];
			String s= "DOWN";
			data=s.getBytes();
			InetSocketAddress ia=new InetSocketAddress(ip,port);
			DatagramPacket paquet=new DatagramPacket(data,data.length,ia);
			dso.send(paquet);
			dso.close();

		} catch(Exception e){
			e.printStackTrace();
		}

	}
}
