import java.awt.* ;
import java.awt.event.*;
import javax.swing.*;

import java.util.*;


//チェス盤において、８個のクィーンを、お互いの利き筋には、
//どのクィーンもないように配置する問題を、エイト・クィーン問題という。

/**
 * @author suzuki
 *
 */
public class QueenBoard extends JFrame  implements ActionListener  {

	/**
	 * @param args
	 */
	int boardSize=8;
	JLabel     title;           // Eight Queen ...
	JButton[][] buttonGrid;   // 盤
	int[][] 	   state;		//盤面の状況 コマがある（１）、利き筋である(-1), 何もない(0)
	ImageIcon  queenOn;         // コマの絵(黒)
	//ImageIcon  queenTry;        // コマの絵(灰)
	ImageIcon  queenChk;        // コマの絵(赤)
	ImageIcon  queenOff;        // 空白の絵
	boolean    trapChk=true;    // 利きゴマの表示をするか
	JButton		iButton;
	JButton		sButton;
	JButton		viewButton;
	JButton		nextButton;
	JButton		qButton;	
	JRadioButton		dButton;
	TextField	aTextField1;
	Label		aLabel1;
	int			queenNum=0;
	Vector	goalVector;
	Random	aRD;
	
	QueenBoard() {
		// Quizで用いるための乱数設定用
		Calendar calendar = new GregorianCalendar();
		int sec=calendar.get(Calendar.SECOND);
		aRD=new Random(sec);
		
		// ウィンドウの大きさ設定
		this.setSize(500,500);
		
		//ウィンドウを閉じたときにアプリケーションを終了するための処理
		this.addWindowListener(new WindowAdapter(){
			public void windowClosing(WindowEvent ev){
				System.exit(0);
			}
		});
		
		//ウィンドウの初期化
		getContentPane().setLayout(new BorderLayout());

		// 色違いの queen アイコンを読み込み
		queenOn  = new ImageIcon( Toolkit.getDefaultToolkit().getImage( "queen-on.gif" ) );  // 黒
		queenOff = new ImageIcon( Toolkit.getDefaultToolkit().getImage( "queen-off.gif" ) ); // 空白
		//queenTry = new ImageIcon( Toolkit.getDefaultToolkit().getImage( "queen-try.gif" ) ); // 灰色
		queenChk = new ImageIcon( Toolkit.getDefaultToolkit().getImage( "queen-chk.gif" ) ); // 赤

		//盤面の状況を表すstateの初期化
		state = new int[boardSize][boardSize];		
		//盤面グリット部分の初期化　ボタンを用いる
		buttonGrid = new JButton[boardSize][boardSize];   // チェス盤のマス目
		JPanel backpanel = new JPanel(new GridLayout(boardSize, boardSize));      // チェス盤
		for(int i=0; i<boardSize; i++)
			for(int j=0; j<boardSize; j++) {
				buttonGrid[i][j] = new JButton( ) ;  // マス目を生成
				buttonGrid[i][j].addActionListener(this);
				buttonGrid[i][j].setIcon(queenOff);
				state[i][j]= 0;
				backpanel.add( buttonGrid[i][j] );                                      // マス目をチェス盤に敷き詰め			
			}
		getContentPane().add(backpanel, SwingConstants.CENTER);                   // チェス盤を Center に貼り付け
		title =new JLabel(boardSize + "-Queen   ", SwingConstants.CENTER);    // タイトル N-Queen
		getContentPane().add(title, BorderLayout.NORTH);                      //   North に貼り付け

		//アクセサリ
	    /**
	     * ボタン配置用パネル
	     */
	    JPanel panel = new JPanel();
        panel.setPreferredSize(new Dimension(500, 100));
        panel.setLayout(null);


	    
		// 盤面の初期化ボタン
		iButton = new JButton("Init");
		iButton.setBounds(0, 0, 80, 30);
		iButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (e.getSource().equals(iButton))
					gridInit();
			}
		});
		
		// 探索開始ボタン
		sButton = new JButton("Search");
		sButton.setBounds(80, 0, 80, 30);
		sButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (e.getSource().equals(sButton))
					SearchByDF(boardSize); // ここの部分をSearchByDF2にする
			}
		});
		
		// 探索結果表示ボタン
		viewButton = new JButton("View");
		viewButton.setBounds(160, 0, 80, 30);
		viewButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (e.getSource().equals(viewButton)){
					int aNum=Integer.parseInt(aTextField1.getText());
				    if((aNum<0)||(aNum>goalVector.size())){
				    		aNum=0;
				    		aTextField1.setText(Integer.toString(aNum));
				    }
			    		nQueenNode aGoal=((nQueenNode)(goalVector.elementAt(aNum)));
			    		viewQposition(aGoal.Contents());
			    
				}
			}
		});
		
		// 表示状態の番号提示用
		aTextField1= new TextField();
		aTextField1.setBounds(240,0, 80, 30);
		
		// 次の状態を表示する
		nextButton = new JButton("Next");
		nextButton.setBounds(320, 0, 80, 30);
		nextButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (e.getSource().equals(nextButton)){
					int aNum=Integer.parseInt(aTextField1.getText());
					aNum++;
				    if((aNum<0)||(aNum>=goalVector.size())){
				    		aNum=0;

				    }
		    			aTextField1.setText(Integer.toString(aNum));
			    		nQueenNode aGoal=((nQueenNode)(goalVector.elementAt(aNum)));
			    		viewQposition(aGoal.Contents());
			    
				}
			}
		});	

		// クイズとして出題する
		qButton = new JButton("Quiz");
		qButton.setBounds(400, 0, 80, 30);
		qButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (e.getSource().equals(qButton)){
					aRD.nextInt(goalVector.size());
					int aNum=aRD.nextInt(goalVector.size());

				    if((aNum<0)||(aNum>=goalVector.size())){
				    		aNum=0;

				    }
    					aTextField1.setText(Integer.toString(aNum));
			    		nQueenNode aGoal=((nQueenNode)(goalVector.elementAt(aNum)));
			    		RandViewQposition(aGoal.Contents());
			    		// drawCell();
			    		aLabel1.setText("残り　"+Integer.toString(boardSize-queenNum));
				}
			}
		});	

		// 利き筋を表示、非表示切り替えボタン
		dButton = new JRadioButton("利き筋を表示", true);
		dButton.setBounds(0, 30, 120, 30);
		trapChk = true;                                                       // 初期値は、表示と指定
		dButton.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent e) {
				if(e.getStateChange()==ItemEvent.DESELECTED) trapChk = false;
				else                                         trapChk = true;
			}
		});

		// 各種アプリの状態表示用
		aLabel1 = new Label();
		aLabel1.setBounds(0,70, 300, 70);
		
		// 各種ボタン類をpanelに貼り付けてフレームに表示
		panel.add(iButton);   
		panel.add(sButton);   
		panel.add(dButton);
		panel.add(aLabel1);
		panel.add(viewButton);
		panel.add(aTextField1);
		panel.add(nextButton);
		panel.add(qButton);		
		this.getContentPane().add(panel, BorderLayout.SOUTH);
        		
		//ウィンドウの可視化
		this.setVisible(true);

	}

	// 連続探索用メソッド　DepthFirstSearch dはrun2()を呼び出す
	// d.run2()から見つかった答えの配置はベクトルgoalVectorとして返ってくる
	public void SearchByDF2(int size)
	{
		aLabel1.setText("Searching");
		boolean success = false;
	    nQueenNode goalNode = new nQueenNode(size,size);
	    nQueenNode initNode = new nQueenNode(size,0);
	    nQueenProblem p = new nQueenProblem(initNode, goalNode);
	    DepthFirstSearch d = new DepthFirstSearch(p);
	    goalVector = d.run2();
		nQueenNode aGoal=((nQueenNode)(goalVector.elementAt(0)));
		viewQposition(aGoal.Contents());
	    System.out.println("Goal: " + goalVector.size() + "-Queen FOUND");
	    aLabel1.setText("Search Goal: " + goalVector.size() + "-Queen FOUND");
	    aTextField1.setText(Integer.toString(0));
	    
	}
	
	// 単発探索用メソッド　DepthFirstSearch dはrun()を呼び出す
	public void SearchByDF(int size)
	{
		boolean success = false;
	    nQueenNode goalNode = new nQueenNode(size,size);
	    nQueenNode initNode = new nQueenNode(size,0);
	    nQueenProblem p = new nQueenProblem(initNode, goalNode);
	    DepthFirstSearch d = new DepthFirstSearch(p);
	    success = d.run();
	    if(success){
		System.out.println("Goal: " + size + "-Queen FOUND");
		d.printResult();
		viewQposition(d.getResult());
	    } else {
		System.out.println("Goal: " + size + "-Queen NOT-FOUND");
	    }
	    
	}
	
    public void printState(){
	for(int i=0; i<boardSize; i++){
	    for(int j=0; j<boardSize; j++){
		if(state[i][j]==1){
		    System.out.print("1 ");
		} else {
		    if(state[i][j]==-1){
			System.out.print("* ");
		    } else {
			System.out.print("0 ");
		    }
		}
	    }
	    System.out.println(" ");
	}
	
    }
	
// nQueenNodeの持つQpositionを受け取りstate配列に反映させる
	public void QposToState(int[] aQposition)
	{
		gridInit();
		for(int ip=0; ip<aQposition.length; ip++)
		{
			System.out.println(aQposition[ip]);
			state[ip][ aQposition[ip] ]=1;
		}
		// これはプログラム動作確認用
		for(int i=0; i<boardSize; i++)
		{
			for(int j=0; j<boardSize; j++) {
				System.out.print(Integer.toString(state[i][j])+" ");

			}
			System.out.println(" ");
		}
		
	}
	
	// aQpositionで受け取った配置をフレーム上に描画する
	public void viewQposition(int[] aQposition)
	{
		QposToState(aQposition);
		viewBoard();
	}
    //---------------------------------------------------
	// クイズ用　aQpositionで受け取った配置の一部だけランダムにフレーム上に描画する
    //---------------------------------------------------
	public void RandViewQposition(int[] aQposition)
	{
		QposToState(aQposition);
		RandViewBoard();
	}	
	
    //---------------------------------------------------
	// 状態の初期化
    //---------------------------------------------------
	public void gridInit(){
		for(int i=0; i<boardSize; i++)
			for(int j=0; j<boardSize; j++) {
				state[i][j]= 0;
				buttonGrid[i][j].setBackground(Color.WHITE);
				buttonGrid[i][j].setIcon(queenOff);
				}
		queenNum=0;
		aLabel1.setText(Integer.toString(boardSize));
	}

	
    //---------------------------------------------------
	// legal(int x, int y)
	// x,y 座標がボード内にあるかどうか判定
	//---------------------------------------------------
	private boolean legal(int x, int y) {
		return (x >= 0 && x < boardSize && y >= 0 && y < boardSize);
	}
	
	
    //---------------------------------------------------
	// checkState(int ip, int it)
    //---------------------------------------------------	
	public void checkState(int ip, int it){
			// 横の利き筋を設定
			for(int ix=0; ix<boardSize; ix++)
				if(state[ix][it]!=1)
				state[ix][it]= -1;
			//縦の利き筋を設定
			for(int iy=0; iy<boardSize; iy++)
				if(state[ip][iy]!=1)
				state[ip][iy]= -1;
			//右上斜め方向の利き筋
			for (int i=1; legal(ip+i,it+i); i++)
				if(state[ip+i][it+i]!=1)
				state[ip+i][it+i]= -1;
			//右下斜め方向の利き筋
			for (int i=1; legal(ip+i,it-i); i++)
				if(state[ip+i][it-i]!=1)
				state[ip+i][it-i]= -1;
			//左上斜め方向の利き筋
			for (int i=1; legal(ip-i,it+i); i++)
				if(state[ip-i][it+i]!=1)
				state[ip-i][it+i]= -1;
			//左下斜め方向の利き筋
			for (int i=1; legal(ip-i,it-i); i++)
				if(state[ip-i][it-i]!=1)
				state[ip-i][it-i]= -1;
		}

    //---------------------------------------------------	
	//利き筋となったグリッドに色を塗る関数
    //---------------------------------------------------	
	public void drawCell()
		{
			for(int i=0; i<boardSize; i++){
				for(int j=0; j<boardSize; j++) {
					if(trapChk){
						if((state[i][j]== -1)||(state[i][j]== 1)){
							buttonGrid[i][j].setBackground(Color.ORANGE);
							//buttonGrid[i][j].setText(Integer.toString(state[i][j]));
						}
					}
				}
			}
			System.out.println("drawCell queenNum="+Integer.toString(queenNum));
			// これはプログラム動作確認用
			printState();
		}
		
		// 
	    //---------------------------------------------------	
		// 乱数を使ってグリッド上にアイコンを配置
		// Quiz用の描画関数
	    //---------------------------------------------------	
		public void RandViewBoard()
		{
		    Set<Integer> s = new HashSet<Integer>();

		    /* board state clear for the capture positions */
		    for(int i=0; i<boardSize; i++){
			for(int j=0; j<boardSize; j++){
			    if(state[i][j]!=1){
				state[i][j] = 0;
				    }
			}
		    }

		    System.out.println("RandViewBoard");

			queenNum=0;
			int aViewRow=aRD.nextInt(boardSize-1)+1;
			System.out.println("Number of Q's displayed: " + Integer.toString(aViewRow));
			for(int ip=0; ip<aViewRow; ip++){
			    int ix=0;
			    for(ix=aRD.nextInt(boardSize); s.contains(ix); ix=aRD.nextInt(boardSize)){};
			    s.add(ix);
			}
			for(int i=0; i<boardSize; i++){
			    if(!s.contains(i)){
				for(int j=0; j<boardSize; j++){
				    state[i][j] = 0;
				}
			    }
			}
			for(int ix : s){
			    for(int it=0; it<boardSize; it++){
				if(state[ix][it]== 1){
				    System.out.println("X: " + Integer.toString(ix) + "; Y: " + Integer.toString(it));
				    buttonGrid[ix][it].setIcon(queenOn);
				    queenNum++;
				    checkState(ix, it); 
				}
			    }
			}
			drawCell();
    		}	
		
	    //---------------------------------------------------	
		// stateに従ってグリッド上にアイコンを描画
	    //---------------------------------------------------			
		public void viewBoard()
		{
    			for(int ip=0; ip<boardSize; ip++){
    				for(int it=0; it<boardSize; it++){
    					if(state[ip][it]== 1){
    						buttonGrid[ip][it].setIcon(queenOn);
						queenNum++;
    						checkState(ip, it); 
    					}
    				}
    			}
			drawCell();
    		}		
		
	    //---------------------------------------------------		
		//盤上のグリッドとなっているボタンが押されたときに呼び出される関数
	    //---------------------------------------------------
		public void actionPerformed(ActionEvent e) {
	        //ActionEvent e に，イベントが発生した場所が格納されているので
	        //それを元に，どのボタンが押されたかを判別し，
	        //対応したRenderPanelのメソッドを呼び出す．
	    		for(int ip=0; ip<boardSize; ip++){
	    			for(int it=0; it<boardSize; it++){
	    				if (e.getSource().equals(buttonGrid[ip][it])){
	    					if((state[ip][it]== -1)||(state[ip][it]== 1)){
	    						//置かれたクィーンの位置が制約に反している場合の処理
	    						buttonGrid[ip][it].setIcon(queenChk);
	    						aLabel1.setText("残り　"+Integer.toString(boardSize-queenNum)+" Game Over");
	    						break;
	    					}
	    					else {
	    						//置かれたクィーンの位置が有効な場合の処理
	    						
	    						buttonGrid[ip][it].setIcon(queenOn);
	    						state[ip][it]=1;
	    						checkState(ip, it); 		
	    						queenNum++;
	    						drawCell();
	    						aLabel1.setText("残り　"+Integer.toString(boardSize-queenNum));
	    						break;
	    					}
	    				}
	    			}
	    		}
	    }
		
    //---------------------------------------------------
	// main関数
	//---------------------------------------------------
		public static void main(String[] args) {
			// TODO Auto-generated method stub
			System.out.println("Lets start QueenBoard\n");
			QueenBoard aQueenBoard= new QueenBoard();
		}

}
