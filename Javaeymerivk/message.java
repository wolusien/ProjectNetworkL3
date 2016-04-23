
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
		if(tools.verif_mess_down(decomp)){
			return true;
		}
		else{
			if(decomp.length<2 || contientid(decomp[1])){
				System.out.println("message deja recu");
				return false;
			}
			if(tools.verif_mess_app(decomp)){
				listidm.add(decomp[1]);
				sendMessage(data);
				return true;
			}
			if(tools.verif_mess_eybg(decomp)){
				listidm.add(decomp[1]);
				sendMessage(data);
				return true;
			}
			if(tools.verif_mess_gbye(decomp)){
				listidm.add(decomp[1]);
				sendMessage(data);
				return true;
			}
			if(tools.verif_mess_memb(decomp)){
				listidm.add(decomp[1]);
				sendMessage(data);
				return true;
			}
			if(tools.verif_mess_test(decomp)){
				listidm.add(decomp[1]);
				sendMessage(data);
				return true;
			}
			if(tools.verif_mess_who(decomp)){
				listidm.add(decomp[1]);
				sendMessage(data);
				return true;
			}
		}
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
					System.out.println("J'ai reçu :"+st);
				}else{
					System.out.println("mauvais message");
				}
			}

		}
		catch(Exception e){
			e.printStackTrace();
		}

	}

	public void sendMessage(byte[] data){
		try{
			System.out.println(ent.getNextIp()+" "+ent.getNextudp());
			DatagramSocket dso= new DatagramSocket();
			InetSocketAddress ia= new InetSocketAddress(ent.getNextIp(), ent.getNextudp());
			DatagramPacket packet= new DatagramPacket(data, data.length, ia);
			dso.send(packet);
			dso.close();
		}
		catch(Exception e){
			System.out.println(ent.getNextIp()+" "+ent.getNextudp());
			e.printStackTrace();
		}
	}
}
