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
				if(sc.hasNextLine()){
					String s= sc.nextLine();
					verif(s);
				}
			}
			sc.close();
			Thread.currentThread().interrupt();
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
			String s= tools.mess_gbye(ent, tools.ip(), ent.getCli().port);
			ent.getCli().sendMessage(s.getBytes());
		}
		if(mess.startsWith("APPL")){
			String[] m=mess.split(" ");
			// APPL idm LETTRE## id ip port [lettre] reponse
			if(m.length==1 && ent.getCli().idapp.equals("LETTRE##")){
				String s=tools.mess_app(ent.getCli().idapp, m[0], ent);
				ent.getCli().sendMessage(s.getBytes());
			}
			if(m.length>=2){
				if(ent.getCli().idapp.equals("LETTRE##") && ent.getCli().joue){
					if(ent.getCli().dev!=""){
						if(m[1].startsWith(ent.getCli().dev)){
							ent.getCli().dev="";
							String s= tools.mess_app_rep_lettre(ent, ent.getCli().idapp, ent.getCli().dev, m[1]);
							ent.getCli().sendMessage(s.getBytes());
							String s1= tools.mess_app(ent.getCli().idapp, "", ent);
							ent.getCli().sendMessage(s1.getBytes());
						}
						else{
							ent.getCli().dev="";
							ent.getCli().joue=false;
							System.out.println("perdu");
							String s1= tools.mess_app(ent.getCli().idapp, "", ent);
							ent.getCli().sendMessage(s1.getBytes());
						}
					}
					else{
						System.out.println("rien a deviner");
					}
				}
				else{
					String s=tools.mess_app(ent.getCli().idapp, mess.substring(5),ent);
					if(ent.getCli().idapp.equals("TRANS###"))
						ent.getCli().attend=s.split(" ")[1];
					ent.getCli().sendMessage(s.getBytes());
				}
			}
		}
	}
}
