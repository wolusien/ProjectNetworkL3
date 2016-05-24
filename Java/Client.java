import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;


public class Client implements Runnable{
	Entity ent;
	boolean quit=false;
	
	public Client(Entity ent){
		this.ent=ent;
	}
	
	public void quit(){
		quit=true;
	}
	

	public void run() {
		try{
			Scanner sc= new Scanner(System.in);
			while(!quit){
				String s= sc.nextLine();
				verif(s);
			}
			sc.close();
		}
		catch(Exception e){
			e.printStackTrace();
		}
		
	}
	
	public void verif(String mess){
		if(mess.equals("WHOS")){
			String s=tools.mess_who();
			ent.getCli().sendMessage(s.getBytes());
		}
		if(mess.equals("TEST")){
			String s=tools.mess_test(ent);
			ent.getCli().test();
		}
		if(mess.equals("GBYE")){
			String s= tools.mess_gbye(ent, ent.getNextIp(), ent.getNextudp());
			ent.getCli().sendMessage(s.getBytes());
		}
		if(mess.startsWith("APPL")){
			String[] m=mess.split(" ");
			if(m.length>=2){
				String s=tools.mess_app(ent.getCli().idapp, mess.substring(5));
				if(ent.getCli().idapp.equals("TRANS###"))
					ent.getCli().attend=s.split(" ")[1];
				ent.getCli().sendMessage(s.getBytes());
			}
		}
	}
}
