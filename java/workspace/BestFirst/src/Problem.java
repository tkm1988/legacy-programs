import java.util.*;

abstract class Problem{
    String problemType;		// Type of the poblem
    Node initNode;		// Initial state
    Node goalNode;		// Goal state

    public Problem(String type, Node theInit, Node theGoal){
	problemType = type;
	initNode = theInit;
	goalNode = theGoal;
    }

    public Node getInitNode(){
	return initNode;
    }
    public void setInitNode(Node theInit){
	initNode = theInit;
    }

    public Node getGoalNode(){
	return goalNode;
    }

    public void setGoalNode(Node theGoal){
	goalNode = theGoal;
    }

    public boolean isGoal(Node node){
	if(node.equal(goalNode)){
	    return true;
	} else {
	    return false;
	}
    }

}
