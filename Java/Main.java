public class Main {

	public static void main(String[] args){
		if(args.length==1){
			Entity ent= new Entity();
			ent.Init(args[0]);
		}
		else{
			System.out.println("Mais mais il faut un argument pour APPL");
		}
	}
}
