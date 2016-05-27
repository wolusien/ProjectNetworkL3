
public class Maindupl {

	public static void main(String[] args) {
		if(args.length==3){
			Entity ent= new Entity();
			ent.Init(args[2]);
			ent.duplication(args[0], Integer.parseInt(args[1]));
		}
		else{
			System.out.println("non non non");
			System.out.println("ip port appl");
		}
	}

}
