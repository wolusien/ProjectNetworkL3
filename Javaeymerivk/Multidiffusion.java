import java.util.ArrayList;


public class Multidiffusion implements Runnable{
	int port;
	ArrayList<Entity> listEnt;
	
	public Multidiffusion(int port){
		this.port=port;
		listEnt= new ArrayList<Entity>();
	}
	
	public void addEntity(Entity ent){
		listEnt.add(ent);
	}

	public void run() {
		
	}
}
