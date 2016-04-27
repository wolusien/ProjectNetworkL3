
import java.net.InetAddress;
import java.net.ServerSocket;
import java.util.Date;

public class tools{
	public static  boolean verif_mess_server(String mess, Entity ent){
		String[] decomp=mess.split(" ");
		if (decomp.length!=5)return false;
		if(!decomp[0].equals("WELC"))return false;
		ent.setNextIp(decomp[1]);
		ent.setNextudp(Integer.parseInt(decomp[2]));
		ent.setCastIp(decomp[3]);
		ent.setCastPort(Integer.parseInt(decomp[4]));

		Multidiffusion multidiff= new Multidiffusion(ent);
		multidiff.setIp(ent.getCastIP());
		multidiff.setPort(ent.getCastPort());
		ent.lance_multidiffusion(multidiff);
		return true;

	}
	
	//genere un idm
	public static String genereIdm(){
		Date d= new Date();
		try{
			Thread.sleep(1);
		}
		catch(Exception e){
			e.printStackTrace();
		}
		String s= Long.toString(d.getTime());
		return s.substring(s.length()-8);
	}
	
	//message dans l'anneau
	/*###############################################"###########################################################"

	 */
	public static  String mess_app(String id_app,String mess){
		String idm=genereIdm();//genere alea
		return "APPL "+remplissageId(idm)+" "+remplissageId(id_app)+" "+mess;
	}
	public static  String mess_who(){
		String idm=genereIdm();//genere alea
		return "WHOS "+idm;
	}
	public static  String mess_memb(Entity e){
		String idm=genereIdm();//genere alea
		return "MEMB "+idm+" "+remplissageId(e.getId())+" "+remplissageIp2(e.getNextIp())+" "+remplissageIp2(e.getNextIp());
	}
	public static  String mess_gbye(Entity e,String ip,int port){
		String idm=genereIdm();//genere alea
		return "GBYE "+idm+" "+remplissageIp2(ip)+" "+intToOctet(port,4)+" "+remplissageIp2(e.getNextIp())+" "+intToOctet(e.getNextudp(),4);
	}
	public static  String mess_eybg(){
		String idm=genereIdm();//genere alea
		return "EYBG "+idm;
	}
	public static  String mess_test(Entity e){
		String idm=genereIdm();//genere alea
		return "TEST "+idm+" "+remplissageIp2(e.getCastIP())+" "+intToOctet(e.getCastPort(),4);
	}
	public static  String mess_down(){
		return "DOWN";
	}
	/*
fin
	 */
	/*
verif les message circulant sur l'anneau
####################################################################
	 */
	public static  boolean verif_mess_app( String []decomp){

		if(decomp[0].equals("APPL")){
			if(decomp.length>=4){
				return true;

			}
			return false;

		}
		return false;
	}
	public static boolean  verif_mess_who(String []decomp){
		if(decomp[0].equals("WHOS")){
			if(decomp.length==2){
				return true;
			}
			return false;

		}
		return false;

	}
	public static  boolean  verif_mess_memb(String []decomp){
		if(decomp[0].equals("MEMB")){
			if(decomp.length==5){
				return true;

			}
			return false;

		}
		return false;

	}
	public static  boolean verif_mess_gbye(String []decomp){
		if(decomp[0].equals("GBYE")){
			if(decomp.length==6){
				return true;

			}
			return false;

		}
		return false;

	}
	public static  boolean verif_mess_eybg(String []decomp){
		if(decomp[0].equals("EYBG")){
			if(decomp.length==2){
				return true;

			}
			return false;

		}
		return false;

	}
	public static  boolean verif_mess_test(String []decomp){
		if(decomp[0].equals("TEST")){
			if(decomp.length==4){
				return true;

			}
			return false;

		}
		return false;

	}
	public static  boolean verif_mess_down(String[]decomp){
		return decomp[0].equals("DOWN");

	}

	/*
fin

	 */
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
			return remplissageIp(ia.getHostAddress());
		} catch(Exception e){
			e.printStackTrace();
		}
		return"null";
	}
	public static String intToOctet(int conv,int t){
            String c=Integer.toString(conv);
            if(c.length()==t)return c;
            else{
                System.out.println((t-c.length()));
                int add=t-c.length();
                for(int i=0;i<add;i++){
                    c="0"+c;
            }
            }
            return c;
        }
         public static String remplissageId(String id){
              
            if(id.length()==8)return id;
            else{
                System.out.println((8-id.length()));
                int add=8-id.length();
                for(int i=0;i<add;i++){
                    id=Character.MIN_VALUE+id;
            }
            }
            return id;
         }
        public static String remplissageIp2(String ip){
            String[] tab=ip.split("\\.");
            if(!(tab.length==4)){
			System.out.println("ce n'est pas un ip");
			return null;
		}
            return ""+ intToOctet(Integer.parseInt(tab[0]),3)+"."+intToOctet(Integer.parseInt(tab[1]),3)+"."+intToOctet(Integer.parseInt(tab[2]),3)+"."+intToOctet(Integer.parseInt(tab[3]),3);
            
        }
	//rempli avec des 0
	public static String remplissageIp(String ip){
		String[] tab=ip.split("\\.");
		String ipfinal="";
		if(!(tab.length==4)){
			System.out.println("ce n'est pas un ip");
			return null;
		}
		for(int i=0; i<tab.length; i++){
			while(tab[i].length()<3){
				tab[i]="0"+tab[i];
			}
			if(i==tab.length-1){
				ipfinal=ipfinal+tab[i];
			}
			else{
				ipfinal=ipfinal+tab[i]+".";
			}
		}
		
		return ipfinal;
	}
	
	
 public static void main (String[]argv)  { 
    String r=  remplissageId("reseau");

     byte[]tab=r.getBytes();
     System.out.println(r);
    
      System.out.println(r.getBytes().length);
       
 } 
}
