
import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.nio.file.Files;
import java.util.ArrayList;
import java.io.OutputStream;

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
	private boolean quit=false;
	Entity ent;
	ArrayList<String> listidm;
	private String test;

	public message(Entity ent){
		this.ent=ent;
		listidm=new ArrayList<String>();

	}

	public void quit(){
		quit=true;
	}
	public  DatagramSocket  portlibre(){
		DatagramSocket dso=null;
		int port=2048;
		while(true){
			try{
				dso=new DatagramSocket(port);
				ent.setUdp_port(port);
				ent.setNextudp(port);
				System.out.println("port udp : "+port);
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
			return false;
		}
		//utilise une application
		if(tools.verif_mess_app(decomp)){
			listidm.add(decomp[1]);
			if(decomp[2].startsWith("DIFF") && Integer.parseInt(decomp[3])==decomp[4].length()){
				sendMessage(data);
				return true;
			}
			sendMessage(data);
			return false;
		}

		//sort de l anneau
		if(tools.verif_mess_eybg(decomp)){
			listidm.add(decomp[1]);
			quit();
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
			//System.out.println("je suis pas sur donc : "+decomp[2]+" = "+ent.getNextIp()+ " / "+decomp[3]+" = "+ent.getNextudp());
			if(decomp[2].equals(ent.getNextIp()) && decomp[3].equals(Integer.toString(ent.getNextudp()))){
				String s=tools.mess_eybg();
				byte[] data2= new byte[512];
				data2=s.getBytes();
				sendMessage(data2);
				ent.setNextIp(decomp[4]);
				ent.setNextudp(Integer.parseInt(decomp[5]));
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
			if(test!=null && test.equals(decomp[1])){
				System.out.println("on est ici");
				test=null;
			}
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
			while(!quit){
				dso.receive(paquet);
				String st=new String(paquet.getData(),0,paquet.getLength());
				if(verif_message(st.trim())){
					System.out.println(st);
				}else{
					//System.out.println("mauvais message "+st);
				}
			}
			dso.close();
			System.out.println("message fermer");

		}
		catch(Exception e){
			e.printStackTrace();
		}

	}

	public void sendMessage(byte[] data){
		try{
			DatagramSocket dso= new DatagramSocket();
			if(ent.actif){
				System.out.println("envoye a :"+ent.getNextudp());
				InetSocketAddress ia= new InetSocketAddress(ent.getNextIp(), ent.getNextudp());
				DatagramPacket packet= new DatagramPacket(data, data.length, ia);
				dso.send(packet);
			}
			if(ent.getdupl() && ent.duplactif){
				System.out.println("envoye a dupl :"+ent.getdupl_udp_port());
				InetSocketAddress ia= new InetSocketAddress(ent.getdupl_udp_ip(), ent.getdupl_udp_port());
				DatagramPacket packet= new DatagramPacket(data, data.length, ia);
				dso.send(packet);
			}
			dso.close();
		}
		catch(Exception e){
			//e.printStackTrace();
			down();
		}
	}

	public boolean test(){
		try{
			if(ent.getdupl()){
				System.out.println("impossible de tester car est une duplication");
				return true;
			}
			if(test!=null){
				System.out.println("impossible de tester car un test est deja en cours");
				return true;
			}
			else{
				DatagramSocket dso= new DatagramSocket();
				byte[] data= new byte[255];
				String s=tools.mess_test(ent);
				test= s.trim().split(" ")[1];
				data=s.getBytes();
				InetSocketAddress ia= new InetSocketAddress(ent.getNextIp(),ent.getNextudp());
				DatagramPacket paquet= new DatagramPacket(data, data.length, ia);
				dso.send(paquet);
				dso.close();
				int count=0;
				while(true){
					Thread.sleep(100);
					if(test==null) break;
					if(count==30){
						System.out.println("go down");
						down();
						return false;
					}
					count++;
				}
				return true;
			}
		}
		catch(Exception e){
			e.printStackTrace();
			return false;
		}
	}

	public void down(){
		try{
			DatagramSocket dso=new DatagramSocket();
			byte[]data = new byte[255];
			String s= "DOWN";
			data=s.getBytes();
			if(ent.actif){
				InetSocketAddress ia=new InetSocketAddress(ent.getCastIP(),ent.getCastPort());
				DatagramPacket paquet=new DatagramPacket(data,data.length,ia);
				dso.send(paquet);
			}
			if(ent.getdupl() && ent.duplactif){
				InetSocketAddress ia=new InetSocketAddress(ent.getdupl_cast_ip(), ent.getdupl_cast_port());
				DatagramPacket packet= new DatagramPacket(data, data.length, ia);
				dso.send(packet);
			}
			dso.close();

		} catch(Exception e){
			e.printStackTrace();
		}

	}

	
	// Ici pour APPL TRANS
	
	//lire un fichier
	public byte[] readFile(String path){
		if(!verif_file(path)) return null;
		File file = new File(path);
		try {
			return Files.readAllBytes(file.toPath());
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

	
	//verifie si un fichier existe
	public boolean verif_file(String path){
		File f=new File(path);
		if(f.exists())
			return true;
		return false;
	}
}
