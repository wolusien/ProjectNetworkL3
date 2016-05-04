public class Main {

	public static void main(String[] args){
		Entity ent= new Entity();
		ent.Init();
		while(true){
			try{
				Thread.sleep(2000);
				ent.getCli().test();

			}
			catch(Exception e){

			}
		}

	}
}
