import java.util.*;

public class GraphNode extends Node {
    //    String name;
    //    Vector children;
    Hashtable childrenCosts;
    //    Node pointer;  // for storing search result
    //    int x,y;      // (x,y) coordinate
    int gValue;   // コスト
    int hValue;   // ヒューリスティック値
    int fValue;   // 評価値
    boolean hasGValue = false;
    boolean hasFValue = false;

    GraphNode(String theName){
	super(theName);
	childrenCosts = new Hashtable();
    }

    public int getGValue(){
	return gValue;
    }

    public void setGValue(int theGValue){
	hasGValue = true;
	this.gValue = theGValue;
    }

    public int getHValue(){
	return hValue;
    }

    public void setHValue(int theHValue){
	this.hValue = theHValue;
    }

    public int getFValue(){
	return fValue;
    }

    public void setFValue(int theFValue){
	hasFValue = true;
	this.fValue = theFValue;
    }

    public void addChild(Node theChild, int theCost){
	children.addElement(theChild);
	childrenCosts.put(theChild,new Integer(theCost));
    }

    public Vector expand(){
	return children;
    }

    public int getCost(Node theChild){
	return ((Integer)childrenCosts.get(theChild)).intValue();
    }
    
    public String toString(){
	String result = name + "(h:" + hValue + ")";
	if(hasGValue){
	    result = result + "(g:" +gValue + ")";
	}
	if(hasFValue){
	    result = result + "(f:" + fValue + ")";
	}
	return result;
    }
}
