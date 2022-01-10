import java.util.*;

public class MazeNode extends GraphNode {
    int x,y;      // (x,y) coordinate


    MazeNode(String theName, int theX, int theY){
	super(theName);
	x = theX;
	y = theY;
    }

    public int getX(){
	return x;
    }

    public int getY(){
	return y;
    }

    public void setX(int theX){
	this.x = theX;
    }

    public void setY(int theY){
	this.y = theY;
    }
}
