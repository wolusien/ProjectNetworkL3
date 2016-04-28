import java.net.*;
import java.util.Random;
//import java.util.ArrayList;


public class Multidiffusion implements Runnable{
	int port;
	String ip;
	Entity ent;
	boolean quit=false;

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

	public void quit(){
		quit=true;
	}

	public int portlibre(){
		Random rand= new Random();
		MulticastSocket mso=null;
		int essaie=0;
		this.port=rand.nextInt(9999 - 4000 + 1) +4000;
		while(true){
			try{
				mso=new MulticastSocket(port);
				mso.close();
				break;
			}
			catch(Exception e){
				essaie++;
				this.port=rand.nextInt(9999 - 4000 + 1) +4000;
				if(essaie==200)return -1;
			}
		}
		return port;
	}

	//trouve une ip libre
	public String iplibre(){
		Random r= new Random();
		int r1=r.nextInt(257);
		int r2=r.nextInt(257);
		int r3=r.nextInt(257);

		try{
			String s="226."+Integer.toString(r1)+"."+Integer.toString(r2)+"."+Integer.toString(r3);
			MulticastSocket mso=new MulticastSocket(port);
			mso.joinGroup(InetAddress.getByName(s));
			return tools.remplissageIp(s);
		}catch(Exception e){
			return iplibre();

		}
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
			while(!quit){
				mso.receive(paquet);
				String st=new String(paquet.getData(),0,paquet.getLength());
				if(tools.verif_mess_down(st.trim().split(" "))){
					if(!ent.getdupl()){
						this.ent.quit();
					}
					else{
						if(this.ip.equals(ent.getCastIP()) && this.port==ent.getCastPort()){
							ent.actif=false;
							quit();
						}
						if(this.ip.equals(ent.getdupl_cast_ip()) && this.port==ent.getdupl_cast_port()){
							ent.duplactif=false;
							quit();
						}
						if(!ent.actif && !ent.duplactif){
							ent.quit();
						}
							
					}
				}
				//System.out.println("message recu : "+st);
			}
			mso.leaveGroup(InetAddress.getByName(ip));
			mso.close();
			System.out.println("multidiffusion fermer");

		} catch(Exception e){
			e.printStackTrace();
		}
	}

	//envoie multicast
	/* deplacer dans message
	public void down(){
		try{
			DatagramSocket dso=new DatagramSocket();
			byte[]data = new byte[255];
			String s= "DOWN";
			data=s.getBytes();
			InetSocketAddress ia=new InetSocketAddress(ent.getCastIP(),ent.getCastPort());
			DatagramPacket paquet=new DatagramPacket(data,data.length,ia);
			dso.send(paquet);
			dso.close();
		} catch(Exception e){
			e.printStackTrace();
		}
	}*/
}