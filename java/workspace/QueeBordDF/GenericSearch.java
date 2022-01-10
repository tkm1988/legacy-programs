import java.util.*;

abstract class GenericSearch{
    String searchType;
    Node initNode, goalNode;
    Problem p;
    Vector open;
    Vector goalVector;
    boolean success;
    int step;

    public GenericSearch(String searchType, Problem theProblem){
	this.searchType = searchType;
	p = theProblem;
	initNode = p.getInitNode();
	goalNode = p.getGoalNode();
	open = new Vector();
	goalVector = new Vector();
	open.addElement(initNode);
	success = false;
	step = 0;
    }

    abstract Vector revise(Vector a, Vector b);

    public boolean run(){
	for(;;){
	    System.out.println("STEP:"+(++step));
	    System.out.println("OPEN:"+open.toString());
	    // if OPEN is empty then failure
	    if(open.size() == 0){
		success = false;
		return success;
	    } else {
		// pop OPEN list
		Node node = (Node)open.elementAt(0);
		open.removeElementAt(0);
		// if node is the goal then success
		if(p.isGoal(node)){

		    // System.out.println("Goal: " + p.getGoalNode().toString());
		    // System.out.println("Node: " + node.toString());

		    success = true;
		    p.setGoalNode(node);	// small trick
		    return success;
		} else {
		    // expand node
		    Vector children = node.expand();
		    // set solution backpointers
		    for(int i = 0 ; i < children.size() ; i++){
			Node m = (Node)children.elementAt(i);
			m.setPointer(node);
		    }
		    // revise OPEN list
		    open = this.revise(open,children);
		}
	    }
	}
    }

    //********************************************************************
    // 課題部分
    // このメソッドrun2()をゴール状態を続けて探索するようにrun()を参考にプログラムせよ
    // この関数はすべての探索が終了すると、探索したすべてのゴール状態をクラスVectorの
    // インスタンスgoalVectorに格納して、goalVectorを返す関数とする
    //********************************************************************
    public Vector run2(){
    	for(;;){
    	    System.out.println("STEP:"+(++step));
    	    System.out.println("OPEN:"+open.toString());
    	    // openが空になったら探索を終了してgoalVectorを返して終了する
    	    if(open.size() == 0){
    		success = false;
    		return goalVector;
    	    } else {
    	    	//****************************
    	    	//この部分をrun()を参考にプログラムせよ
    	    	
    	    	
    	    	//****************************
    	    }
    	}
    	}
    
    public void printSolution(){
	printSolution(p.getInitNode(), p.getGoalNode());
    }

    void printSolution(Node initNode, Node goalNode){
	if(goalNode.equal(initNode)){
	    System.out.println(goalNode.toString());
	} else {
	    System.out.println(goalNode.toString()+" <- ");
	    printSolution(initNode, goalNode.getPointer());
	}
    }

    public  int[] getResult()
    {
    		return p.getGoalNode().Contents();
    }
    
    public void printResult(){
	printResult(p.getGoalNode());
    }

    void printResult(Node goalNode){
	String[] results = goalNode.display();
	for(int i = 0 ; i < results.length ; i++){
	    System.out.println(results[i]);
	}
    }
    
    public Node goalNode()
    {
    	 return goalNode;
    }
	
}
