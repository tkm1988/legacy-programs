import java.util.*;

public class DepthFirstSearch extends GenericSearch{

    DepthFirstSearch(Problem p){
	super("DepthFirst", p);
    }

    public Vector revise(Vector remaining, Vector expanded){
	for(int i = 0 ; i < expanded.size() ; i++){
	    remaining.insertElementAt(expanded.elementAt(i),i);
	}
	return remaining;
    }
}
