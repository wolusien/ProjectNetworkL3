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
		this.port=4000;
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
	
	//trouve une ip libre
	public String iplibre(){
		for(int i=0 ; i<256; i++){
			for(int j=0 ; j<256 ; j++){
				for(int k=0 ; k<256 ;k++){
					try{
						String s="226."+Integer.toString(i)+"."+Integer.toString(j)+"."+Integer.toString(k);
						MulticastSocket mso=new MulticastSocket(port);
						mso.joinGroup(InetAddress.getByName(s));
						return tools.remplissageIp(s);
					}catch(Exception e){
						
					}
				}
			}
		}
		return null;
	}

	public void run() {
		System.out.println("multicast demarré");
		try{
			MulticastSocket mso=new MulticastSocket(port);
			mso.joinGroup(InetAddress.getByName(ip));
			byte[]data=new byte[512];
			System.out.println(ip);
			DatagramPacket paquet=new DatagramPacket(data,data.length);
			System.out.println("multicast port et address "+port+" "+ip);
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
