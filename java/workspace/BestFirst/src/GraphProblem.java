import java.util.*;

abstract class GraphProblem extends Problem {
    Vector nodes;
    GraphNode initNode, goalNode;

    GraphProblem(GraphNode theInit, GraphNode theGoal){
	super("GraphProblem", theInit, theGoal);
	nodes = new Vector();
	makeStateSpace();
	for(int i = 0 ; i < nodes.size() ; i++){
	    GraphNode m = (GraphNode)nodes.elementAt(i);
	    if(theInit.equal(m)){
		this.initNode = m;
		break;
	    }
	}
	for(int i = 0 ; i < nodes.size() ; i++){
	    GraphNode m = (GraphNode)nodes.elementAt(i);
	    if(theGoal.equal(m)){
		this.goalNode = m;
		break;
	    }
	}
	if(initNode == null || goalNode == null){
	    System.out.println("Start/Goal: INVALID!");
	}
    }

    public GraphNode getInitNode(){
	return initNode;
    }

    public GraphNode getGoalNode(){
	return goalNode;
    }

    private void biDirectionalLink(Node a, Node b){
	a.addChild((Node)b);
	b.addChild((Node)a);
    }

    abstract void makeStateSpace();
}
