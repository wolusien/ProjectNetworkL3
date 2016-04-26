import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;


public class Envoiemessage {

	public static void main(String[] args){
		
		try{
			DatagramSocket dso=new DatagramSocket();
			String s="GBYE "+tools.genereIdm()+" 192.168.233.001 1024 192.168.233.001 1025";
			byte[]data = new byte[512];
			data=s.getBytes();
			InetSocketAddress ia=new InetSocketAddress("192.168.233.001",1024);
			DatagramPacket paquet=new DatagramPacket(data,data.length,ia);
			dso.send(paquet);
			
			dso.close();
		}catch(Exception e){
			e.printStackTrace();
		}
	}
}
