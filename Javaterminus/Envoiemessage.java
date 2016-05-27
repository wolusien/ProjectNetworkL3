import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;


public class Envoiemessage {

	public static void main(String[] args){
		//String s1="GBYE "+tools.genereIdm()+" 192.168.233.001 1024 192.168.233.001 1025";
		String s2="WHOS "+tools.genereIdm();
		String nomfic="C:/Users/Eric/Downloads/eclipse-java-luna-SR2-win32-x86_64/eclipse/notice.html";
		//String s3=new String(ent.getCli().readFile("C:/Users/Eric/Downloads/eclipse-java-luna-SR2-win32-x86_64/eclipse/notice.html"));
		String s4= "APPL "+tools.genereIdm()+" TRANS### REQ "+Integer.toString(nomfic.length())+" "+nomfic;
		message(s4);
	}
	public static void message(String s){	
		try{
			DatagramSocket dso=new DatagramSocket();
			//String s="GBYE "+tools.genereIdm()+" 192.168.233.001 1024 192.168.233.001 1025";
			//String s="WHOS "+tools.genereIdm();
			//String nomfic="C:/Users/Eric/Downloads/eclipse-java-luna-SR2-win32-x86_64/eclipse/notice.html";
			//String s=new String(ent.getCli().readFile("C:/Users/Eric/Downloads/eclipse-java-luna-SR2-win32-x86_64/eclipse/notice.html"));
			//System.out.println(s);
			//String s= "APPL "+tools.genereIdm()+" TRANS### REQ "+Integer.toString(nomfic.length())+" "+nomfic;
			//System.out.println(s);
			byte[]data = new byte[512];
			data=s.getBytes();
			InetSocketAddress ia=new InetSocketAddress("192.168.0.38",2048);
			DatagramPacket paquet=new DatagramPacket(data,data.length,ia);
			dso.send(paquet);
			
			dso.close();
		}catch(Exception e){
			e.printStackTrace();
		}
	}
}
