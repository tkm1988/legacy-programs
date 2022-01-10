import java.util.*;

abstract class Node{
    String name;
    int level;
    Vector children;
    Node pointer;  // for storing search result

    Node(String theName){
	name = theName;
	children = new Vector();
    }

    Node(String theName, int theLevel){
	name = theName;
	level = theLevel;
	children = new Vector();
    }

    public String getName(){
	return name;
    }


    public int getLevel(){
	return level;
    }

    public void setLevel(int theLevel){
	level = theLevel;
    }

    public Node getPointer(){
	return this.pointer;
    }

    public void setPointer(Node theNode){
	pointer = theNode;
    }

    public boolean equal(Node n){
	if((this.getName()).equals(n.getName())){
	    return true;
	} else {
	    return false;
	}
    }

    public void addChild(Node theChild){
	children.addElement(theChild);
    }

    public Vector getChildren(){
	return children;
    }

    abstract Vector expand();

    abstract String[] display();
    
    abstract int[] Contents(); 

    public String toString(){
	String result = "<" + name + ">";
	return result;
    }

}
