
import java.io.BufferedInputStream;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Scanner;
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
	public String idapp="";
	public String attend="";
	public int restant=0;
	public int port;
	int ite=0;
	public 	String fic;
	public String  contenu="";
	public boolean joue=true;
	public String dev="";
	public message(Entity ent, String idapp){
		this.ent=ent;
		listidm=new ArrayList<String>();
		this.idapp=tools.remplissageId(idapp);
		System.out.println(idapp);
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
				this.port=port;
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
			if(decomp[2].equals("DIFF####") 
					&& decomp[2].equals(idapp) 
					&& Integer.parseInt(decomp[3])==decomp[4].length()){

				sendMessage(data);
				return true;
			}			
			//verifie si le fichier est la, si oui envoie le fichier
			if(tools.verif_mess_app_transREQ(decomp) 
					&& decomp[2].equals(idapp)){
				if(verif_file(decomp[5])){

					byte[] fic=readFile(decomp[5]);
					envoiefichier(fic, decomp[4], decomp[5], decomp[1]);
					return true; // de cette manière il ne peut pas renvoyer REQ
				}
				else if(decomp[1].equals(attend)) {
					attend="";
					System.out.println("Le fichier n a pas ete trouve");
				}
				else
					sendMessage(data);
				return true;
			}
			if(tools.verif_mess_app_trans(decomp)
					&& decomp[2].equals(idapp)
					&& decomp[4].equals(attend)){
				if(decomp[3].equals("ROK")){
					restant=Integer.parseInt(decomp[7]);
					fic=decomp[7];
					ite=0;

				}
				else if(decomp[3].equals("SEN")){
					traite(mess);
					if(ite==restant)remplitab(contenu);

				}
				else
					sendMessage(data);
				return true;
			}
			if(tools.verif_mess_app_lettre_dev(decomp) && decomp[2].equals(idapp) && joue){
				if(decomp[4].equals(tools.ip()) && decomp[5].equals(Integer.toString(port))){
					String s=tools.mess_app_gagnant_lettre(idapp, ent);
					sendMessage(s.getBytes());
					return true;
				}
				else{
					dev=decomp[6];
					return true;
				}
			}
			if(tools.verif_mess_app_rep(decomp) && decomp[2].equals(idapp)){
				sendMessage(data);
				return true;
			}
			if(tools.verif_mess_app_gag(decomp)){
				dev="";
				joue=true;
				sendMessage(data);
				return true;
			}
			
			sendMessage(data);
			// on peut retourner faux car on ne peut pas lire l application
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
				sendMessage(data2,0);
				ent.setNextIp(decomp[4]);
				ent.setNextudp(Integer.parseInt(decomp[5]));
			}
			if(ent.duplactif && decomp[2].equals(ent.getdupl_udp_ip()) && decomp[3].equals(Integer.toString(ent.getdupl_udp_port()))){
				String s=tools.mess_eybg();
				byte[] data2= new byte[512];
				data2=s.getBytes();
				sendMessage(data2,1);
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
			Thread.currentThread().interrupt();

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

	// 0 - envoie a non duppl
	// 1 - envoie a duppl
	public void sendMessage(byte[] data, int i){
		try{
			DatagramSocket dso= new DatagramSocket();
			if(ent.actif && i==0){
				System.out.println("envoye a :"+ent.getNextudp());
				InetSocketAddress ia= new InetSocketAddress(ent.getNextIp(), ent.getNextudp());
				DatagramPacket packet= new DatagramPacket(data, data.length, ia);
				dso.send(packet);
			}
			if(ent.getdupl() && ent.duplactif && i==1){
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

	public void envoiefichier(byte[] fic, String sizenom, String nomfic, String idtrans){
		//String idtrans= tools.genereIdm();
		String s= "APPL "+tools.genereIdm()+" TRANS### ROK "+idtrans+" "+sizenom+" "+nomfic+" ";
		String s2= "APPL "+tools.genereIdm()+" TRANS### SEN "+idtrans+" ";
		int taillemax=512-(s2.getBytes().length+8);
		int nummess;
		if(fic.length%taillemax==0)
			nummess=fic.length/taillemax;
		else
			nummess=fic.length/taillemax+1;
		int tailledernier=fic.length%taillemax;
		s=s+nummess;
		byte[] go=new byte[512];
		go=s.getBytes();
		sendMessage(go);
		int pointer=0;
		byte[] sen;
		for(int i=0; i<nummess;i++){
			s2= "APPL "+tools.genereIdm()+" TRANS### SEN "+idtrans+" "+tools.intToOctet(i, 3)+" ";
			byte[] envoie;
			if(i!=nummess-1){
				envoie = new byte[512];
				sen=(s2+tools.intToOctet(taillemax, 3)+" ").getBytes();
			}
			else{
				sen=(s2+tools.intToOctet(tailledernier,3)+" ").getBytes();
				envoie=new byte[sen.length+tailledernier];
			}
			for(int j=0; j<sen.length;j++){
				envoie[j]=sen[j];
			}
			int k=0;
			while(sen.length+k<512){
				if(pointer<fic.length)
					envoie[sen.length+k]=fic[pointer];
				pointer++;
				k++;
			}
			//System.out.println(new String(envoie));
			sendMessage(envoie);
		}
	}

	public  void remplitab(String tab){
		try{
		    BufferedWriter writer=new BufferedWriter(new FileWriter(new File("transfert")));
			writer.write(tab);
			writer.close();
		}
		catch(Exception e){

		}

	}
	public void  traite(String mess){

		String[] dec=mess.split(" ", 8);
		contenu=contenu+dec[dec.length-1];
		System.out.println("dec : "+dec[6]);
		System.out.println("ite : "+ite);
		if(Integer.parseInt(dec[5])!=ite){
			System.out.println("fichier envoyer dans le desordre") ;    	 
		}
		ite++;

	}
}


