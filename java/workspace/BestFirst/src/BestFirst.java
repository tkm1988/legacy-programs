import java.util.*;

public class BestFirst {
    public static void main(String args[]){
	boolean success = false;
	if(args.length != 2){
	    System.out.println("USAGE: java BestFirst [StartCity] [GoalCity]");
	} else {
	    String start = args[0];
	    String goal  = args[1];
	    MazeNode initNode = new MazeNode(start,0,0);
	    MazeNode goalNode = new MazeNode(goal,0,0);
	    MazeProblem p = new MazeProblem(initNode, goalNode);
	    BestFirstSearch b = new BestFirstSearch(p);
	    success = b.run();
	    if(success){
		b.printSolution(p.getInitNode(), p.getGoalNode());
	    } else {
		System.out.println("Path from: " + start + "to: " + goal + " NOT-FOUND");
	    }
	}
    }
}
