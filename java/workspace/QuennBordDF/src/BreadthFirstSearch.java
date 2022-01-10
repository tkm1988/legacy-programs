import java.util.*;

public class BreadthFirstSearch extends GenericSearch{

    public BreadthFirstSearch(Problem p){
	super("BreadthFirst", p);
    }

    public Vector revise(Vector remaining, Vector expanded){
	for(int i = 0 ; i < expanded.size() ; i++){
	    remaining.addElement(expanded.elementAt(i));
	}
	return remaining;
    }
}
