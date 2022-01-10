import java.util.*;

public class BreadthFirstSearch extends GenericSearch{

    public BreadthFirstSearch(GraphProblem theProblem){
	super("BreadthFirst", theProblem);
	GraphNode start = theProblem.getInitNode();
	start.setGValue(0);
    }

    public Vector revise(Vector open, Vector closed, Vector expanded, GraphNode node){
	for(int i = 0 ; i < expanded.size() ; i++){
	    GraphNode m = (GraphNode)expanded.elementAt(i);
	    int gmn = node.getGValue() + node.getCost(m);

	    if(!open.contains(m) && !closed.contains(m)){
		m.setGValue(gmn);
		m.setPointer(node);
		open.addElement(m);
	    }
	}
	return open;
    }
}
