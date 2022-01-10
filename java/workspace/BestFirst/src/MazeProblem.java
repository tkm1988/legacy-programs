import java.util.*;
import java.lang.*;

public class MazeProblem extends GraphProblem {

    MazeProblem(MazeNode theInit, MazeNode theGoal){
	super(theInit, theGoal);
	int goalX = ((MazeNode)goalNode).getX();
        int goalY = ((MazeNode)goalNode).getY();
		for(int i = 0 ; i < nodes.size() ; i++){
		    MazeNode m = (MazeNode)nodes.elementAt(i);
		    int theX = m.x;
		    int theY = m.y;
		    int dx = goalX - theX;
		    int dy = goalY - theY;

		    //直線距離を求めフィールドmに代入
		    m.setHValue((int)Math.sqrt(dx*dx + dy*dy));

		    //マンハッタン距離の場合
		    //m.setHValue(Math.abs(dx)+Math.abs(dy));
		}
    }

    private void biDirectionalLink(MazeNode a, MazeNode b, int n, int m){
	a.addChild((MazeNode)b,n);
	b.addChild((MazeNode)a,m);
    }

    public void makeStateSpace(){
	MazeNode node[];
	node = new MazeNode[10];
	// 状態空間の生成
	node[0] = new MazeNode("Seoul",-1300,-200);
	node[1] = new MazeNode("SanFrancisco",10900,200);
	node[2] = new MazeNode("Tokyo",0,0);
	node[3] = new MazeNode("Sendai",150,300);
	node[4] = new MazeNode("Hakodate",150,700);
	node[5] = new MazeNode("Sapporo",250,850);
	node[6] = new MazeNode("Kanazawa",-250,100);
	node[7] = new MazeNode("Nagoya",-250,-80);
	node[8] = new MazeNode("Osaka",-380,-80);
	node[9] = new MazeNode("Fukuoka",-800,-200);

	biDirectionalLink(node[0],node[1],12600,12600);		// Seoul - SanFrancisco
	biDirectionalLink(node[0],node[2],2500,2000);		// Seoul - Tokyo
	biDirectionalLink(node[0],node[4],2400,2350);		// Seoul - Hakodate
	biDirectionalLink(node[0],node[9],500,500);		// Seoul - Fukuoka
	biDirectionalLink(node[1],node[2],12000,13000);		// Tokyo - SanFrancisco
	biDirectionalLink(node[1],node[7],13000,12300);		// SanFrancisco - Nagoya
	biDirectionalLink(node[2],node[3],450,600);		// Tokyo - Sendai
	biDirectionalLink(node[2],node[4],900,1200);		// Tokyo - Hakodate
	biDirectionalLink(node[2],node[5],1200,1200);		// Tokyo - Sapporo
	biDirectionalLink(node[2],node[6],450,350);		// Tokyo - Kanazawa
	biDirectionalLink(node[2],node[7],600,800);		// Tokyo - Nagoya
	biDirectionalLink(node[2],node[8],600,800);		// Tokyo - Osaka
	biDirectionalLink(node[2],node[9],1200,1500);		// Tokyo - Fukuoka
	biDirectionalLink(node[3],node[5],800,800);		// Sendai - Sapporo
	biDirectionalLink(node[4],node[5],250,250);		// Hakodate - Sapporo
	biDirectionalLink(node[5],node[7],1500,1500);		// Sapporo - Nagoya

	for(int i = 0 ; i < 10 ; i++) {
	    nodes.addElement(node[i]);
	}
    }

}
