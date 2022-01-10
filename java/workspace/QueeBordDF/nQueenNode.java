import java.util.*;

public class nQueenNode extends Node{
    int size;		// number of Queens
    int[] Qposition;    // Queen position table
			// level wil be used to indicate number of Qs already placed

    nQueenNode(int theSize){
	super(theSize + "-Queen");
	size = theSize;
	level = 0;		// default level
	Qposition = new int[size];
	for(int i = 0 ; i < size ; i++){
	    Qposition[i]=0;
	}
    }
    
    nQueenNode(int theSize, int theLevel){
	super(theSize + "-Queen", theLevel);
	size = theSize;
	Qposition = new int[size];
	for(int i = 0 ; i < size ; i++){
	    Qposition[i]=0;
	}
    }

    public nQueenNode copyNode(){
	nQueenNode newNode = new nQueenNode(size, level);
	for(int i = 0 ; i < level ; i++){
	    newNode.setQposition(i, this.Qposition[i]);
	}
	return newNode;
    }

    public void setQposition(int i, int j){
	int s = size-1;
	if(i < 0 || size <= i){
	    System.out.println("I:" + i + " must be between 0 and " + s);
	} else {
	    if(j < 0 || size <= j){
		System.out.println("J: " + j + " must be between 0 and " + s);
	    } else {
		Qposition[i] = j;
	    }
	}
    }

    Vector expand(){
	Vector children = new Vector();
	int theLevel = this.getLevel();
	for(int j = 0 ; j < size ; j++){
	    nQueenNode newNode = copyNode();
	    newNode.setQposition(theLevel,j);
	    newNode.setLevel(theLevel+1);
	    if(newNode.is_legal_placement()){
		children.addElement(newNode);
	    }
	}
	return children;
    }

    public boolean equal(Node n){
	if(this.getLevel() == n.getLevel()){
	    return true;
	} else {
	    return false;
	}
    }

    boolean is_legal_placement(){
	for(int i = 0 ; i < level ; i++){
	    int j = Qposition[i];
	    for(int k = 0 ; k < level ; k++){
		if(k != i){
		    // row check is not needed
		    // column check
		    int l = Qposition[k];
		    if(j == l) return false;
		    // diagonal
		    if((i+j) == (k+l)) return false;
		    if((i-j) == (k-l)) return false;
		}
	    }
	}
	return true;
    }

    public String toString(){
	String result = "<Q:" + level + ">";
	return result;
    }

    public String[] display(){
	String[] result = new String[size];
	StringBuffer line;

	for(int i = 0 ; i < size ; i++){
	    line = new StringBuffer("");
	    for(int j = 0 ; j < size ; j++){
		if(Qposition[i] == j){
		    line.append("Q");
		} else {
		    line.append(" ");
		}
		result[i] = line.toString();
	    }
	}
	return result;
    }

    public int[] Contents()
    {
    		return Qposition;
    }
    public int getQposition(int ip)
    {
    		return Qposition[ip];
    }
}
