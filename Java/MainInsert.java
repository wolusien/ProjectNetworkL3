import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.util.Scanner;


public class MainInsert {

	public static void main(String[] args) {
		if(args.length==3){
			Entity ent= new Entity();
			ent.insertion(args[0], Integer.parseInt(args[1]),args[2]);
		}
		else{
			System.out.println("ou sont les arguments?");
			System.out.println("ip port appl");
		}
		/*try{
			DatagramSocket dso=new DatagramSocket();
			byte[]data = new byte[512];
			String s= "sdfezfz";
			data=s.getBytes();
			InetSocketAddress ia=new InetSocketAddress("192.168.233.001",1024);
			DatagramPacket paquet=new DatagramPacket(data,data.length,ia);
			dso.send(paquet);
			dso.close();
		}catch(Exception e){
			e.printStackTrace();
		}*/
	}

}
