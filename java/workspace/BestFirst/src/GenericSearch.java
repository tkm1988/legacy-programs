import java.util.*;

abstract class GenericSearch{
    String searchType;
    GraphNode initNode, goalNode;
    GraphProblem p;
    Vector open,closed;
    boolean success;
    int step;

    public GenericSearch(String searchType, GraphProblem theProblem){
	this.searchType = searchType;
	p = theProblem;
	initNode = p.getInitNode();
	goalNode = p.getGoalNode();
	open = new Vector();
	closed = new Vector();
	open.addElement(initNode);
	success = false;
	step = 0;
    }

    // revise method manipulates OPEN list by 
    // (1) adding non-OPEN & non-CLOSED nodes from EXPANDED list
    // (2) adding OPEN or CLOSED nodes (again) if they have smaller heuristic values

    abstract Vector revise(Vector open, Vector closed, Vector expanded, GraphNode node);

    public boolean run(){
	for(;;){
	    System.out.println("STEP:"+(++step));
	    System.out.println("OPEN:"+open.toString());
	    System.out.println("CLOSED:"+closed.toString());
	    // if OPEN is empty then failure
	    if(open.size() == 0){
		success = false;
		return success;
	    } else {
		// pop OPEN list
		GraphNode node = (GraphNode)open.elementAt(0);
		open.removeElementAt(0);
		// if node is the goal then success
		if(p.isGoal(node)){
		    success = true;
		    return success;
		} else {
		    // expand NODE
		    Vector children = node.expand();
		    // add NODE to CLOSED
		    closed.addElement(node);
		    // revise OPEN list
		    open = this.revise(open,closed,children,node);
		}
	    }
	}
    }

    /***
     * Vector を Node の fValue の昇順（小さい順）に列べ換える．
     */
    public Vector sortUpperByFValue(Vector theOpen){
	Vector newOpen = new Vector();
	GraphNode min,tmp = null;
	while(theOpen.size() > 0){
	    min = (GraphNode)theOpen.elementAt(0);
	    for(int i = 1 ; i < theOpen.size() ; i++){
		tmp = (GraphNode)theOpen.elementAt(i);
		if(min.getFValue() > tmp.getFValue()){
		    min = tmp;
		}
	    }
	    newOpen.addElement(min);
	    theOpen.removeElement(min);
	}
	return newOpen;
    }


    /***
     * Vector を Node の gValue の昇順（小さい順）に列べ換える．
     */
    public Vector sortUpperByGValue(Vector theOpen){
	Vector newOpen = new Vector();
	GraphNode min,tmp = null;
	while(theOpen.size() > 0){
	    min = (GraphNode)theOpen.elementAt(0);
	    for(int i = 1 ; i < theOpen.size() ; i++){
		tmp = (GraphNode)theOpen.elementAt(i);
		if(min.getGValue() > tmp.getGValue()){
		    min = tmp;
		}
	    }
	    newOpen.addElement(min);
	    theOpen.removeElement(min);
	}
	return newOpen;
    }

    /***
     * Vector を Node の hValue の昇順（小さい順）に列べ換える．
     */
    public Vector sortUpperByHValue(Vector theOpen){
	Vector newOpen = new Vector();
	GraphNode min,tmp = null;
	while(theOpen.size() > 0){
	    min = (GraphNode)theOpen.elementAt(0);
	    for(int i = 1 ; i < theOpen.size() ; i++){
		tmp = (GraphNode)theOpen.elementAt(i);
		if(min.getHValue() > tmp.getHValue()){
		    min = tmp;
		}
	    }
	    newOpen.addElement(min);
	    theOpen.removeElement(min);
	}
	return newOpen;
    }

    public void printSolution(GraphNode startNode, GraphNode goalNode){
	System.out.println("*** Solution: Total Cost = " + goalNode.getGValue());
	printSolutionHistory(startNode, goalNode);
    }

    public void printSolutionHistory(Node startNode, Node goalNode){
	if(goalNode == startNode){
	    System.out.println(goalNode.toString());
	} else {
	    System.out.println(goalNode.toString()+" <- ");
	    printSolutionHistory(startNode, goalNode.getPointer());
	}
    }
}
