import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;


public class MainInsert {

	public static void main(String[] args) {
		Entity ent= new Entity();
		ent.insertion("172.28.2.52", 1024);
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
