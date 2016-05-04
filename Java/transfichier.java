
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author netbook
 */
public class transfichier {
	static int taille=0;
	/*##############################################################################################################################################
                     CODEUTILISER PAR L'ENVOYEUR DE FICHIER
     ################################################################################################################################################*/

	/*renvoie une arraylist contenant le message a renvoyer por le transfert de fichier f*/
	//[APPL␣idm␣TRANS###␣ROK␣id-trans␣size-nom␣nom-fichier␣nummess]
	public static   ArrayList mess_transROC(String f,String size){
		String idm=tools.genereIdm();//genere alea
		String id_trans=tools.genereIdm();
		ArrayList l=new ArrayList();

		int nummess= Lire( f, l,id_trans);
		String s= "APPL "+idm+"TRANS### ROC "+id_trans+" "+tools.intToOctet(Integer.parseInt(size), 3)+" "+f+" "+taille;
		l.add(s);
		// ecrire(l);
		return l;
	}
	//[APPL␣idm␣TRANS###␣REQ␣size-nom␣nom-fichier]
	/*verifie si c'est un message reQ et si le repertoire personnel contient le fichier demander*/
	public static  boolean verif_mess_app_transREQ(String[]decomp){
		if (decomp.length!=6)return false;
		if(!(decomp.equals("APPL")||decomp[2].equals("TRANS###")||decomp[3].equals("REQ")))return false;

		File f=recherche_fichier( decomp[5]);
		return f!=null;
	}
	/*fonction auxiliaire*/
	public static File recherche_fichier(String s){
		File f=new File(s);
		if(f.exists())return f;
		return null;
	}
	/*remplit l'arrayliste des message send*/
	public static String make_mess_trans(ArrayList l, String s,String id_trans){

		while(s.length()>=10){
			String s1= s.substring(0, 9);
			String s2= s.substring(9, s.length());
			String idm=tools.genereIdm();;

			String mess= "APPL "+idm+" TRANS### SEN"+" "+id_trans +" "+taille+" "+s1.length()+" "+s1;;
			taille++;
			l.add(mess);
			s=s2;
			// l.add(s1);


		}

		return s;
	}
	/*public static void ecrire(ArrayList l){
            for(int i=0;i<l.size();i++){
                System.out.println(l.get(i));
            }
        }*/
	/*remplir une arraylist contenantles messageet renvoie le nombre de message neccessaire pour neccessaire poure le transfert*/
	public static int Lire(String fic,  ArrayList l,String id_trans){
		String s=""; 

		try {
			//[APPL␣idm␣TRANS###␣SEN␣id-trans␣no-mess␣size-content␣content]
			File f=recherche_fichier( fic);
			if(f==null)return -1;
			InputStream ips = new FileInputStream(f);
			InputStreamReader ipsr = new InputStreamReader(ips);
			BufferedReader br = new BufferedReader(ipsr);
			String ligne;
			String line=" ";
			String reste=" ";


			while ((line=br.readLine()) !=null) {
				// System.out.println(lu);

				line=line+reste;
				//System.out.println(dec);


				reste=make_mess_trans( l, line,id_trans);
				//l.add(mess);
				//System.out.println("Value of s after reading a line : \n"+s);
			}
			br.close();

		}
		catch(Exception e){
			e.printStackTrace();
			System.out.println("ERREUR");       
		}

		return taille;
	}
	/*##############################################################################################################################################
                     CODE UTILISER PAR LE DEMANDEUR DE FICHIER
     ################################################################################################################################################*/         
	//[APPL␣idm␣TRANS###␣REQ␣size-nom␣nom-fichier]
	public static  String mess_app(String nom_fichier){
		String idm=tools.genereIdm();//genere alea

		return "APPL "+idm+" TRANS### REQ "+nom_fichier.length()+" "+nom_fichier;
	}


	//[APPL␣idm␣TRANS###␣ROK␣id-trans␣size-nom␣nom-fichier␣nummess] return-1

	public static  boolean verif_mess_app_transROC(String[]decomp){
		if (decomp.length!=7)return false;
		if(!(decomp.equals("APPL")||decomp[2].equals("TRANS###")||decomp[3].equals("ROKS")))return false;


		return true;
	}
	//[APPL␣idm␣TRANS###␣SEN␣id-trans␣no-mess␣size-content␣content]
	public static  boolean verif_mess_app_transSEN(String[]decomp,String id_trans){
		if (decomp.length!=8)return false;
		if(!(decomp.equals("APPL")||decomp[2].equals("TRANS###")||decomp[3].equals("ROKS")||decomp[5].equals(id_trans)))return false;


		return true;
	}





}
