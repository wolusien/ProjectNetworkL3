import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.util.Scanner;

public class MainInsert {
    
	public static void main(String[] args) {
		/*Scanner sc= new Scanner(System.in);
		System.out.println("Quel est ip ? ");
		String ip=sc.nextLine();
		System.out.println("Quel est le port ? ");
		String port=sc.nextLine();
		int por=Integer.parseInt(port);
		System.out.println("Id APPL ? ");
		String appl=sc.nextLine();
		sc.close();*/
		Entity ent= new Entity();
		ent.insertion("192.168.70.111", 1024,"TRANS###");
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
