
import java.net.InetAddress;
import java.net.ServerSocket;

public class tools{
    public static  boolean verif_mess_server(String mess, Entity ent){
        String[] decomp=mess.split(" ");
	if (decomp.length!=5)return false;
        if(!decomp[0].equals("WELC"))return false;
	ent.setNextIp(decomp[1]);
        ent.setNextudp(Integer.parseInt(decomp[2]));
        ent.setCastIp(decomp[3]);
        ent.setCastPort(Integer.parseInt(decomp[4]));
        return true;
        
    }
    
     public static  boolean verif_mess_client(String mess, Entity ent){
        String[] decomp=mess.split(" ");
	if (decomp.length!=3)return false;
        if(!decomp[0].equals("NEWC"))return false;
	ent.setNextIp(decomp[1]);
        ent.setNextudp(Integer.parseInt(decomp[2]));
        return true;
        
    }
     
     public static String ip(){
try{
InetAddress ia=InetAddress.getLocalHost();
return ia.toString();
} catch(Exception e){
e.printStackTrace();
}
return"null";
}
     /*
 public static void main (String[]argv)  {
     System.out.println(Server.serverLibre());
        System.out.println(Server.serverLibre());
             System.out.println(Server.serverLibre());
             System.out.println(Server.serverLibre());
             try{
            ServerSocket  Ssock= new ServerSocket(1027);
             //ent.setUdp_port(port);
          
            }
          catch(Exception e){
			  System.out.println("pris");
		}
 } */
}
