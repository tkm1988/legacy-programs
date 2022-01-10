import java.util.*;

public class BestFirstSearch extends GenericSearch{

    BestFirstSearch(GraphProblem theProblem){
    	super("BestFirst", theProblem);
		GraphNode start = theProblem.getInitNode();
		start.setGValue(0);
    }

    public Vector revise(Vector open, Vector closed, Vector children, GraphNode node){
		for(int i = 0 ; i < children.size() ; i++){
		    GraphNode m = (GraphNode)children.elementAt(i);
		    int gmn = node.getGValue() + node.getCost(m);
	
		    if(!open.contains(m) && !closed.contains(m)){
				m.setGValue(gmn);
				m.setPointer(node);
				open.addElement(m);
		    }
		}
		/*オープンリストに含まれるノードを距離コストが昇順になるようソートする*/
		open = super.sortUpperByGValue(open);
		return open;
    }
}
