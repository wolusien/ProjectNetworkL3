
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.util.ArrayList;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author netbook
 */
public class message implements Runnable {

	Entity ent;
	ArrayList<String> listidm;

	public message(Entity ent){
		this.ent=ent;
		listidm=new ArrayList<String>();

	}
	public  DatagramSocket  portlibre(){
		DatagramSocket dso=null;
		int port=1024;
		while(true){
			try{
				dso=new DatagramSocket(port);
				ent.setUdp_port(port);
				ent.setNextudp(port);
				break;
			}
			catch(Exception e){
				port++;
				if(port==9999)break;
			}
		}
		return dso;
	}

	public boolean contientid(String id){
		for(int i=0; i<listidm.size(); i++){
			if(listidm.get(i).equals(id))
				return true;
		}
		return false;
	}

	public boolean verif_message(String mess){
		String[] decomp=mess.split(" ");
		byte[] data= new byte[512];
		data=mess.getBytes();

		//pas besoin de verifier down ici car c est dans multidiff
		/*if(tools.verif_mess_down(decomp)){
			return true;
		}
		else{*/
		
		//message deja recu
		if(decomp.length<2 || contientid(decomp[1])){
			System.out.println("message deja recu");
			return false;
		}
		//utilise une application
		if(tools.verif_mess_app(decomp)){
			listidm.add(decomp[1]);
			sendMessage(data);
			return true;
		}
		
		//sort de l anneau
		if(tools.verif_mess_eybg(decomp)){
			listidm.add(decomp[1]);
			ent.quit();
			return true;
		}
		//indique que l entitee souhaite sortir de l anneau
		if(tools.verif_mess_gbye(decomp)){
			/*
			 * 1 : idm
			 * 2 : ip
			 * 3 : port
			 * 4 : ip-succ
			 * 5 : port-succ
			 */
			listidm.add(decomp[1]);
			System.out.println("je suis pas sur donc : "+decomp[2]+" = "+ent.getNextIp()+ " / "+decomp[3]+" = "+ent.getNextudp());
			if(decomp[2].equals(ent.getNextIp()) && decomp[3].equals(Integer.toString(ent.getNextudp()))){
				ent.setNextIp(decomp[4]);
				ent.setNextudp(Integer.parseInt(decomp[5]));
				String s=tools.mess_eybg();
				byte[] data2= new byte[512];
				data2=s.getBytes();
				sendMessage(data2);
			}
			sendMessage(data);
			return true;
		}
		//dit qui il est
		if(tools.verif_mess_memb(decomp)){
			listidm.add(decomp[1]);
			sendMessage(data);
			return true;
		}
		//verifie si l anneau est casse
		if(tools.verif_mess_test(decomp)){
			listidm.add(decomp[1]);
			sendMessage(data);
			return true;
		}
		//indique qui se trouve dans l anneau
		if(tools.verif_mess_who(decomp)){
			listidm.add(decomp[1]);
			sendMessage(data);
			String s=tools.mess_memb(ent);
			byte[] data2= new byte[512];
			data2 =s.getBytes();
			sendMessage(data2);
			return true;
		}
		//}
	return false;
	}

	public void run() {
		try{
			DatagramSocket dso=portlibre();
			byte[]data=new byte[512];
			DatagramPacket paquet=new DatagramPacket(data,data.length);
			while(true){
				dso.receive(paquet);
				String st=new String(paquet.getData(),0,paquet.getLength());
				if(verif_message(st.trim())){
					System.out.println(st);
				}else{
					//System.out.println("mauvais message "+st);
				}
			}

		}
		catch(Exception e){
			e.printStackTrace();
		}

	}

	public void sendMessage(byte[] data){
		try{
			System.out.println("envoye a :"+ent.getNextudp());
			DatagramSocket dso= new DatagramSocket();
			InetSocketAddress ia= new InetSocketAddress(ent.getNextIp(), ent.getNextudp());
			DatagramPacket packet= new DatagramPacket(data, data.length, ia);
			dso.send(packet);
			dso.close();
		}
		catch(Exception e){
			//e.printStackTrace();
			down();
		}
	}
	
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

	}
}