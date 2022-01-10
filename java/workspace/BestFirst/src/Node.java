import java.util.*;

public class Node{
    String name;
    Vector children;
    Node pointer;  // for storing search result

    Node(String theName){
	name = theName;
	children = new Vector();
    }

    public String getName(){
	return name;
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

    public Vector expand(){
	return children;
    }

    public String toString(){
	String result = "<" + name + ">";
	return result;
    }

}
