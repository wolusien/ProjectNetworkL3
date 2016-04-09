import java.net.*;
import java.util.Scanner;

public class Main {
	
	/*
	 * taper 1 creation anneau
	 * taper 2 insertion
	 */
	public static void main(String[] args){
		System.out.println("1: creation\n"
				+"2: insertion");
		Scanner sc= new Scanner(System.in);
		String s=sc.nextLine();
		if(s.equals("1")){
			System.out.println(Creation("bonjour",4444,4445));
		}
		if(s.equals("2")){
			System.out.println(Insertion("toto", 4242, 4243));
		}
		sc.close();
	}
	
	public static boolean Creation(String id, int portudp, int porttcp){
		try {
			InetAddress ia= InetAddress.getLocalHost();
			String[] s= ia.toString().split("/");
			System.out.println(s[1]);
			
			Entity ent= new Entity(id, portudp, porttcp, s[1], portudp, s[1], porttcp);
			Thread t= new Thread(ent);
			t.start();
			return true;
		} 
		catch (UnknownHostException e) {
			e.printStackTrace();
			return false;
		}
	}
	
	public static boolean Insertion(String id, int porttcp,int portudp){
		try{
			InetAddress ia= InetAddress.getLocalHost();
			String[] s=ia.toString().split("/");
			System.out.println(s[1]);
			
			Entity ent=new Entity(id, portudp, porttcp, s[1]);
			return ent.initInsert(s[1], 4445);
		}catch(Exception e){
			e.printStackTrace();
			return false;
		}
		
	}
	
	
	

}
