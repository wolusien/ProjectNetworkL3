
import java.io.PrintWriter;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.util.Scanner;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author netbook
 */
public class message implements Runnable {
    Entity ent;
	
    
    public message(Entity ent){
		this.ent=ent;
               
		
	}
	public  DatagramSocket  portlibre(){
            DatagramSocket dso=null;
            int port=1024;
            while(true){
            try{
             dso=new DatagramSocket(port);
             ent.setUdp_port(port);
             break;
            }
          catch(Exception e){
			port++;
                        if(port==9999)break;
		}
            }
            return dso;
        }
            
        

	public void run() {
		try{
    DatagramSocket dso=portlibre();
    byte[]data=new byte[100];
    DatagramPacket paquet=new DatagramPacket(data,data.length);
    while(true){
    dso.receive(paquet);
    String st=new   
    String(paquet.getData(),0,paquet.getLength());
    System.out.println("J'ai reçu :"+st);
           
                    
		}
                }
		catch(Exception e){
			e.printStackTrace();
		}
		
	}
    
}
