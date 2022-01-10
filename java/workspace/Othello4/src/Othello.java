import java.awt.* ;
import java.awt.event.*;
import javax.swing.*;
import java.util.*;

public class Othello extends JFrame implements ActionListener {

    // pieces
    static int empty = 0;
    static int black = 1;
    static int white = 2;
    static int outer = 3;
    ImageIcon	blackOn;
    ImageIcon	whiteOn;
    ImageIcon	emptyStill;

    // board
    static int		boardSize = 8;
    JLabel		title, status;
    JButton[][]		buttonGrid;
    int[]		state;
    static int[]	allDirs = {-11,-10,-9,-1,1,9,10,11};  // 8方向
    JButton		sButton;
    JRadioButton	bHumanButton;
    JRadioButton	bRandomButton;
    JRadioButton	bMaxDButton;
    JRadioButton	bWDButton;
    JRadioButton	bMWDButton;
    JRadioButton	bMiniMaxButton;
    JRadioButton	bABButton;
    TextField		bTextField;

    JRadioButton	wHumanButton;
    JRadioButton	wRandomButton;
    JRadioButton	wMaxDButton;
    JRadioButton	wWDButton;
    JRadioButton	wMWDButton;
    JRadioButton	wMiniMaxButton;
    JRadioButton	wABButton;
    TextField		wTextField;

    Label		aLabel0, aLabel1;	// message output

    // 評価関数最大最小値
    static int winningScore = Integer.MAX_VALUE - 1;
    static int losingScore  = -winningScore;

    String bStrategyName = "";
    String wStrategyName = "";
    EvalFunction bEF;
    EvalFunction wEF;

    // 着手の制御
    int player = black;	// 現在の手番
    Strategy bStrategy, wStrategy;     // 戦略
    boolean inputEnabled = false;  // 盤面から着手入力を許可
    boolean inputDone = false;     // 盤面から着手終了
    boolean runflag = false;	   // 対局進行中
    int inputMove;

    Othello(){

	//盤面の状況を表すstateの初期化
	state = initialState();
	
	// default strategy 設定
	bStrategy = new HumanPlayer();
	wStrategy = new HumanPlayer();
		
	// ウィンドウの大きさ設定
	this.setSize(450,750);
		
	//ウィンドウを閉じたときにアプリケーションを終了するための処理
	this.addWindowListener(new WindowAdapter(){
		public void windowClosing(WindowEvent ev){
		    System.exit(0);
		}
	    });
		
	//ウィンドウの初期化
	getContentPane().setLayout(new BorderLayout());

	// 駒表示アイコンの読み込み
	blackOn  = new ImageIcon( Toolkit.getDefaultToolkit().getImage( "BlackPiece.gif" ) );  // 黒
	whiteOn = new ImageIcon( Toolkit.getDefaultToolkit().getImage( "WhitePiece.gif" ) ); // 白
	//emptyStill = new ImageIcon( Toolkit.getDefaultToolkit().getImage( "BlankSquare.gif" ) ); // 空白

	
	//盤面グリット部分の初期化　ボタンを用いる
	buttonGrid = new JButton[boardSize][boardSize];
	JPanel backpanel = new JPanel(new GridLayout(boardSize, boardSize));
	for(int i=0; i<boardSize; i++)
	    for(int j=0; j<boardSize; j++) {
		buttonGrid[i][j] = new JButton( ) ;  // マス目を生成
		buttonGrid[i][j].addActionListener(this);
		backpanel.add( buttonGrid[i][j] );   // マス目を盤に敷き詰め			
	    }
	
	// 盤を Center に貼り付け
	getContentPane().add(backpanel, SwingConstants.CENTER);
	
	// タイトル Othello, North に貼り付け
	// 状況表示, North に貼り付け
	JPanel topPane = new JPanel();
	topPane.setLayout(new BoxLayout(topPane, BoxLayout.PAGE_AXIS));
	
	title =new JLabel("Othello", SwingConstants.CENTER);
	topPane.add(title);
	topPane.add(Box.createRigidArea(new Dimension(0,5)));
	
	status =new JLabel("黒: " + count(black, state) + "  白: " + count(white, state), SwingConstants.RIGHT);
	topPane.add(status);
	getContentPane().add(topPane, BorderLayout.NORTH); 
	
	//盤面表示の初期化
	displayBoard();
	
	/**
	 * ボタン配置用パネル
	 */
	JPanel bottomPane = new JPanel();
	bottomPane.setLayout(new BoxLayout(bottomPane, BoxLayout.PAGE_AXIS));
	bottomPane.setPreferredSize(new Dimension(450, 250));
	JPanel panel = new JPanel();
	panel.setLayout(new BoxLayout(panel, BoxLayout.LINE_AXIS));
	
	// 開始ボタン
	sButton = new JButton("Play");
	sButton.setSize(80,30);
	sButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if (e.getSource().equals(sButton)) {
			// set bStrategy
			if (bStrategyName == "MiniMax") {
			    int bLevel = Integer.parseInt(bTextField.getText());
			    if (bLevel<=0) {
				bLevel=1;
				bTextField.setText(Integer.toString(bLevel));
			    };
			    bStrategy = new MiniMaxPlayer(bLevel, bEF);
			    bStrategyName = "";
			} else if (bStrategyName == "AlphaBeta") {
			    int bLevel = Integer.parseInt(bTextField.getText());
			    if (bLevel<=0) {
				bLevel=1;
				bTextField.setText(Integer.toString(bLevel));
			    };
			    bStrategy = new AlphaBetaPlayer(bLevel, bEF);
			    bStrategyName = "";
			}
			// set wStrategy
			if (wStrategyName == "MiniMax") {
			    int wLevel = Integer.parseInt(wTextField.getText());
			    if (wLevel<=0) {
				wLevel=1;
				wTextField.setText(Integer.toString(wLevel));
			    };
			    wStrategy = new MiniMaxPlayer(wLevel, wEF);
			    wStrategyName = "";
			} else if (wStrategyName == "AlphaBeta") {
			    int wLevel = Integer.parseInt(wTextField.getText());
			    if (wLevel<=0) {
				wLevel=1;
				wTextField.setText(Integer.toString(wLevel));
			    };
			    wStrategy = new AlphaBetaPlayer(wLevel, wEF);
			    wStrategyName = "";
			}
			// Start the GAME
			runflag = true;
			//			state = initialState();
			//			displayBoard();
		    }
		}
	    });
	
	// 黒戦略選択ボタン
	JLabel bRLabel = new JLabel("黒の戦略", SwingConstants.LEFT);
	bHumanButton = new JRadioButton("人間");
	bHumanButton.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    if (e.getSource().equals(bHumanButton)){
			bStrategy = new HumanPlayer();
			bStrategyName = "";
			// bMaxDButton.setSelected(false);
			// bWDButton.setSelected(false);
			bTextField.setText("");
		    }
		}
	    });
	bHumanButton.setSelected(true);
	
	bRandomButton = new JRadioButton("ランダム");
	bRandomButton.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    if (e.getSource().equals(bRandomButton)){
			bStrategy = new RandomPlayer();
			bStrategyName = "";
			// bMaxDButton.setSelected(false);
			// bWDButton.setSelected(false);
			bTextField.setText("");
		    }
		}
	    });
	
	bMiniMaxButton = new JRadioButton("MiniMax");
	bMiniMaxButton.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    if (e.getSource().equals(bMiniMaxButton)){
			bStrategyName = "MiniMax";
		    }
		}
	    });
	
	bABButton = new JRadioButton("AlphaBeta");
	bABButton.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    if (e.getSource().equals(bABButton)){
			bStrategyName = "AlphaBeta";
		    }
		}
	    });
	
	// 黒評価関数選択ボタン
	JLabel bEFLabel = new JLabel("黒の評価関数", SwingConstants.LEFT);

	bMaxDButton = new JRadioButton("欲張り");
	bMaxDButton.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    if (e.getSource().equals(bMaxDButton)){
			bEF = new CountDiff();
		    }
		}
	    });
	
	bWDButton = new JRadioButton("重み付け");
	bWDButton.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    if (e.getSource().equals(bWDButton)){
			bEF = new WeightedDiff();
		    }
		}
	    });
	
	bMWDButton = new JRadioButton("修正重み");
	bMWDButton.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    if (e.getSource().equals(bMWDButton)){
			bEF = new ModWeightedDiff();
		    }
		}
	    });
	
	ButtonGroup bButton = new ButtonGroup();
	bButton.add(bHumanButton);
	bButton.add(bRandomButton);
	bButton.add(bMiniMaxButton);
	bButton.add(bABButton);

	ButtonGroup bEButton = new ButtonGroup();
	bEButton.add(bMaxDButton);
	bEButton.add(bWDButton);
	bEButton.add(bMWDButton);

	bTextField = new TextField();
	bTextField.setSize(60, 30);

	// 白戦略選択ボタン
	JLabel wRLabel = new JLabel("白の戦略", SwingConstants.LEFT);
	wHumanButton = new JRadioButton("人間");
	wHumanButton.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    if (e.getSource().equals(wHumanButton)){
			wStrategy = new HumanPlayer();
			wStrategyName = "";
			// wMaxDButton.setSelected(false);
			// wWDButton.setSelected(false);
			wTextField.setText("");
		    }
		}
	    });
	wHumanButton.setSelected(true);
	
	wRandomButton = new JRadioButton("ランダム");
	wRandomButton.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    if (e.getSource().equals(wRandomButton)){
			wStrategy = new RandomPlayer();
			wStrategyName = "";
			// wMaxDButton.setSelected(false);
			// wWDButton.setSelected(false);
			wTextField.setText("");
		    }
		}
	    });
	
	wMiniMaxButton = new JRadioButton("MiniMax");
	wMiniMaxButton.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    if (e.getSource().equals(wMiniMaxButton)){
			wStrategyName = "MiniMax";
		    }
		}
	    });
	
	wABButton = new JRadioButton("AlphaBeta");
	wABButton.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    if (e.getSource().equals(wABButton)){
			wStrategyName = "AlphaBeta";
		    }
		}
	    });
	
	// 白評価関数選択ボタン
	JLabel wEFLabel = new JLabel("白の評価関数", SwingConstants.LEFT);

	wMaxDButton = new JRadioButton("欲張り");
	wMaxDButton.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    if (e.getSource().equals(wMaxDButton)){
			wEF = new CountDiff();
		    }
		}
	    });
	
	wWDButton = new JRadioButton("重み付け");
	wWDButton.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    if (e.getSource().equals(wWDButton)){
			wEF = new WeightedDiff();
		    }
		}
	    });
	
	wMWDButton = new JRadioButton("修正重み");
	wMWDButton.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    if (e.getSource().equals(wMWDButton)){
			wEF = new ModWeightedDiff();
		    }
		}
	    });

	ButtonGroup wButton = new ButtonGroup();
	wButton.add(wHumanButton);
	wButton.add(wRandomButton);
	wButton.add(wMiniMaxButton);
	wButton.add(wABButton);

	ButtonGroup wEButton = new ButtonGroup();
	wEButton.add(wMaxDButton);
	wEButton.add(wWDButton);
	wEButton.add(wMWDButton);

	wTextField = new TextField();
	wTextField.setSize(60, 30);
	
	// 各種アプリの状態表示用
	aLabel0 = new Label();
	aLabel0.setText("");
	aLabel1 = new Label();
	aLabel1.setText("");
	
	// 各種ボタン類をpanelに貼り付けてフレームに表示
	JPanel bRadioPanel = new JPanel(new GridLayout(0, 1));
	bRadioPanel.add(bRLabel);
	bRadioPanel.add(bHumanButton);
	bRadioPanel.add(bRandomButton);
	bRadioPanel.add(bMiniMaxButton);
	bRadioPanel.add(bABButton);

	JLabel bdummyLabel1 = new JLabel(" ");
	JLabel bdummyLabel2 = new JLabel(" ");
	JLabel bdLabel = new JLabel("深さ");

	JPanel bERadioPanel = new JPanel(new GridLayout(0,1));
	bERadioPanel.add(bdummyLabel1);
	bERadioPanel.add(Box.createRigidArea(new Dimension(0,10)));
	bERadioPanel.add(bEFLabel);
	bERadioPanel.add(bMaxDButton);
	bERadioPanel.add(bWDButton);
	bERadioPanel.add(bMWDButton);
	
	JPanel bTextPanel = new JPanel(new GridLayout(0,1));
	bTextPanel.add(bdummyLabel2);
	bTextPanel.add(Box.createRigidArea(new Dimension(0,20)));
	bTextPanel.add(bdLabel);
	bTextPanel.add(bTextField);

	JPanel wRadioPanel = new JPanel(new GridLayout(0, 1));
	wRadioPanel.add(wRLabel);
	wRadioPanel.add(wHumanButton);
	wRadioPanel.add(wRandomButton);
	wRadioPanel.add(wMiniMaxButton);
	wRadioPanel.add(wABButton);

	JLabel wdummyLabel1 = new JLabel(" ");
	JLabel wdummyLabel2 = new JLabel(" ");
	JLabel wdLabel = new JLabel("深さ");

	JPanel wERadioPanel = new JPanel(new GridLayout(0,1));
	wERadioPanel.add(wdummyLabel1);
	wERadioPanel.add(Box.createRigidArea(new Dimension(0,10)));
	wERadioPanel.add(wEFLabel);
	wERadioPanel.add(wMaxDButton);
	wERadioPanel.add(wWDButton);
	wERadioPanel.add(wMWDButton);
	
	JPanel wTextPanel = new JPanel(new GridLayout(0,1));
	wTextPanel.add(wdummyLabel2);
	wERadioPanel.add(Box.createRigidArea(new Dimension(0,20)));
	wTextPanel.add(wdLabel);
	wTextPanel.add(wTextField);

	panel.add(bRadioPanel);
	panel.add(bERadioPanel);
	panel.add(bTextPanel);
	panel.add(wRadioPanel);
	panel.add(wERadioPanel);
	panel.add(wTextPanel);

	bottomPane.add(aLabel0);
	bottomPane.add(panel);
	bottomPane.add(aLabel1);
	bottomPane.add(sButton);   
	this.getContentPane().add(bottomPane, BorderLayout.SOUTH);
        
	//ウィンドウの可視化
	this.setVisible(true);
	
    }
    
    // 盤面状態のcopy
    public int[] copyState (int[] theState) {
	int[] newState = new int[100];
	System.arraycopy(theState, 0, newState, 0, 100);
	//	for(int k = 0; k < 100; k++){
	//	    newState[k] = theState[k];
	//	}
	return newState;
    }

    public int count (int player, int[] theState) {
	int n = 0;
	for(int k = 0; k < 100; k++){
	    if(theState[k] == player) n++;
	}
	return n;
    }

    public int countDifference (int player, int[] theState) {
	int diff;
	diff = count(player, theState) - count(opponent(player), theState);
	return diff;
    }

    // 初期盤面
    public int[] initialState(){
	state = new int[100];
	for(int i = 0; i < 10; i++){
	    for(int j = 0; j < 10; j++){
		if(i==0 || i==9 || j==0 || j==9){
		    state[10*i+j]=outer;
		} else {
		    state[10*i+j]=empty;
		}
	    }
	}
	state[44]=white;
	state[45]=black;
	state[54]=black;
	state[55]=white;
	return state;
    }

    // 盤面の表示
    public void displayBoard(){
	int i,j;
	for(int k = 0; k < 100; k++){
	    if(state[k]!=outer){
		i = k / 10 - 1;
		j = k % 10 - 1;
		if(state[k]==black){
		    buttonGrid[i][j].setIcon(blackOn);
		} else {
		    if(state[k]==white){
			buttonGrid[i][j].setIcon(whiteOn);
		    } else {
			buttonGrid[i][j].setIcon(emptyStill);
		    }
		}
	    }
	    status.setText("黒: " + count(black, state) + "  白: " + count(white, state));
	}
    }

    //盤上のグリッドとなっているボタンが押されたときに呼び出される関数
    //---------------------------------------------------
    public void actionPerformed(ActionEvent e) {
	if(inputEnabled && !inputDone) {
	    //ActionEvent e に，イベントが発生した場所が格納されているので
	    //それを元に，どのボタンが押されたかを判別し，
	    //着手の処理をする．
	    for(int i=0; i<boardSize; i++){
		for(int j=0; j<boardSize; j++){
		    if (e.getSource().equals(buttonGrid[i][j])){
			int move = (i+1)*10+j+1;
			if(!legalMove(move, player, state)){
			    // 置けない場所に置こうとした時
			    aLabel1.setText("そこには置けません");
			    return;
			} else {
			    // 正しい場所に着手した時
			    inputEnabled = false;
			    inputMove = move;
			    aLabel1.setText("");
			    inputDone = true;
			}
		    }
		}
	    }
	}
    }

    // 敵のplayer
    static int opponent(int player){
	if (player == black){
	    return white;
	} else {
	    return black;
	}
    }

    // 着手として盤面の中が指定されているか？
    boolean validMove(int move){
	int k = move % 10;
	if(11 <= move && move <= 88 && k > 0 && k < 9){
	    return true;
	} else {
	    return false;
	}
    }

    // 正しい着手かどうか判断
    static boolean legalMove(int move, int player, int[] theState){
	if(theState[move] != empty){
	    return false;
	} else {
	    // 8方向のどちらかで相手の駒を引っくりかえせばOK
	    for(int k = 0 ; k < 8 ; k++){
		if(causeFlipQ(move, allDirs[k], player, theState)>=0)
		   return true;
	    }
	    return false;
	}
    }

    // initialize legalMoves before the call
    // int[] legalMoves = new int[100];
    // returns the number of legalMoves k by the "player" in "theState"
    static int allLegalMoves (int[] legalMoves, int player, int[] theState) {
	int k = 0;
	for (int p = 11; p <= 88; p++) {
	    int p1 = p % 10;
	    if (p1 > 0 && p1 < 9 && legalMove(p, player, theState)) {
		legalMoves[k] = p;
		k++;
	    }
	};
	return k;
    }

    // 着手を実行
    public int[] makeMove(int move, int player, int[] theState){
	theState[move] = player;
	for(int k = 0; k < 8; k++){
	    theState = makeFlips(move, allDirs[k], player, theState);
	}
	return theState;
    }

    // 挟まれた駒の反転
    public int[] makeFlips(int move, int dir, int player, int[] theState){
	int bracket = causeFlipQ(move, dir, player, theState);
	if(bracket >= 0){
	    for(int c = move+dir; c!=bracket; c = c+dir){
		theState[c] = player;
	    }
	}
	return theState;
    }

    // 駒の反転を起こすかどうか判断
    // 起こす場合には挟む自駒の位置を
    // 起さない場合には -1 を返す
    static int causeFlipQ(int move, int dir, int player, int[] theState){
	int c = move + dir;
	if(theState[c] != opponent(player))
	    return -1;
	return findBracket(c + dir, dir, player, theState);
    }

    static int findBracket(int c, int dir, int player, int[] theState){
	if(theState[c]==player){
	    return c;
	} else if(theState[c]==opponent(player)){
	    return findBracket(c+dir, dir, player, theState);
	} else {
	    return -1;
	}
    }
    
    // Next player: black or white or -1 (no more play)
    static int nextPlayer(int currentPlayer, int[] theState){
	int opp = opponent(currentPlayer);
	if(anyLegalMove(opp, theState)){
	    return opp;
	} else if(anyLegalMove(currentPlayer, theState)){
	    return currentPlayer;
	} else {
	    return -1;
	}
    }	 

    static boolean anyLegalMove(int player, int[] theState){
	for(int p = 11; p <= 88; p++){
	    int p1 = p % 10;
	    if(p1 > 0 && p1 < 9 && legalMove(p, player, theState)) {
		// System.out.println(p + " LEGAL for " + player);
		return true;
	    }
	}
	return false;
    }

    /* 対戦戦略 */

    abstract class Strategy {
	abstract int[] getMove(int player, int[] state);
    };

    // getMove methodで次の着手を指定
    // その中で displayBoard により着手後の盤面を表示
    class HumanPlayer extends Strategy {
	public int[] getMove(int player, int[] state) {
	    inputEnabled = true;
	    inputDone = false;
	    while (inputEnabled || !inputDone) { };
	    state = makeMove(inputMove, player, state);
	    displayBoard();
	    return state;
	}
    }

    class RandomPlayer extends Strategy {
	Random aRD;

	RandomPlayer() {
	    // 乱数設定用
	    Calendar calendar = new GregorianCalendar();
	    int sec=calendar.get(Calendar.SECOND);
	    aRD=new Random(sec);
	}

	public int[] getMove(int player, int[] state) {
	    int[] legalMoves = new int[100];
	    int k = allLegalMoves(legalMoves, player, state);

	    int aNum = aRD.nextInt(k);
	    int move = legalMoves[aNum];
	    state = makeMove(move, player, state);
	    displayBoard();
	    return state;
	}
    }

    /* 評価関数 */

    abstract class EvalFunction {
	abstract int evalFun (int player, int[] theState);
    };

    class CountDiff extends EvalFunction {
	public int evalFun (int player, int[] theState) {
	    return countDifference(player, theState);
	}
    }

    class WeightedDiff extends EvalFunction {
	int[] Weights = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			 0,120,-20, 20,  5,  5, 20,-20,120,  0,
			 0,-20,-40, -5, -5, -5, -5,-40,-20,  0,
			 0, 20, -5, 15,  3,  3, 15, -5, 20,  0,
			 0,  5, -5,  3,  3,  3,  3, -5,  5,  0,
			 0,  5, -5,  3,  3,  3,  3, -5,  5,  0,
			 0, 20, -5, 15,  3,  3, 15, -5, 20,  0,
			 0,-20,-40, -5, -5, -5, -5,-40,-20,  0,
			 0,120,-20, 20,  5,  5, 20,-20,120,  0,
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0};

	public int evalFun (int player, int[] theState) {
	    int opp = opponent(player);
	    int w = 0;
	    for (int p = 0; p < 100; p++) {
		if (theState[p] == player) {
		    w = w + Weights[p];
		} else if (theState[p] == opp) {
		    w = w - Weights[p];
		}
	    }
	    return w;
	}
    }

    public int getNeighbors(int p, int[] NN) {
	int k = 0;
	for (int i = 0; i < 8; i++) {
	    int q = p + allDirs[i];
	    int q1 = q % 10;
	    if (11 <= q && q <= 88 && q1 > 0 && q1 < 9) {
		NN[k] = q;
		k++;
	    }
	}
	return k;
    }

    class ModWeightedDiff extends WeightedDiff {
	int[] corners = {11, 18, 81, 88};

	public int evalFun (int player, int[] theState) {
	    int opp = opponent(player);
	    int w = super.evalFun(player, theState);
	    for (int p = 0; p < 4; p++) {
		if (theState[corners[p]] != empty) {
		    int[] NN = new int[8];
		    int n = getNeighbors(corners[p], NN);
		    for (int i = 0; i < n; i++) {
			if (theState[NN[i]] != empty) {
			    int ww = 5 - Weights[NN[i]];
			    if (theState[NN[i]] == player) {
				w = w + ww;
			    } else {
				w = w - ww;
			    }
			}
		    }
		}
	    }
	    return w;
	}

    }

    public int Maximizer (EvalFunction e, int player, int[] theState) {
	int[] legalMoves = new int[100];
	int n = allLegalMoves(legalMoves, player, theState);
	int maxMove = 0;
	int maxScore = Integer.MIN_VALUE;

	for (int i = 0; i < n; i++) {
	    int move = legalMoves[i];
	    int score = e.evalFun(player, makeMove(move, player, copyState(theState)));
	    if (score > maxScore) {
		maxMove = move;
		maxScore = score;
	    }
	}
	return maxMove;
    }
	
    class MaxDiffPlayer extends Strategy {
	public int[] getMove (int player, int[] theState) {
	    CountDiff e = new CountDiff();
	    int maxMove = Maximizer(e, player, theState);
	    theState = makeMove(maxMove, player, theState);
	    displayBoard();
	    return theState;
	}
    }
    
    class WeightDiffPlayer extends Strategy {
	public int[] getMove (int player, int[] theState) {
	    WeightedDiff e = new WeightedDiff();
	    int maxMove = Maximizer(e, player, theState);
	    theState = makeMove(maxMove, player, theState);
	    displayBoard();
	    return theState;
	}
    }

    /* Search strategies */

    // when the game is finished this is used for the value of evalFun
    public int finalScore (int player, int[] theState) {
	int n = countDifference (player, theState);
	if (n > 0) {
	    return winningScore;
	} else if (n < 0) {
	    return losingScore;
	} else {
	    return 0;
	}
    }

    /* Minimax Search */

    class MiniMaxPlayer extends Strategy {
	int level;
	EvalFunction e;

	// theLevel must be strictly greater than 0
	MiniMaxPlayer(int theLevel, EvalFunction theE) {
	    this.level = theLevel;
	    this.e = theE;
	}

	int MiniMax (int player, int[] theState, int theLevel, EvalFunction theE) {
	    // これ以上先読みをしないならば盤面の評価値を返す
	    if (theLevel == 0) {
		return theE.evalFun(player, theState);
	    } else {
		// 着手可能手を調べる
		int[] legalMoves = new int[100];
		int n = allLegalMoves(legalMoves, player, theState);
		if (n == 0) {
		    // 着手可能手がなければ敵の着手可能手を調べる
		    if (anyLegalMove(opponent(player), theState)) {
			// もしあれば敵の可能手による最小化を行う
			return -MiniMax(opponent(player), theState, theLevel-1, theE);
		    } else {
			// なければゲームはそこで終了なので勝敗を返す
			return finalScore(player, theState);
		    }
		} else {
		    // 自分の可能手がある場合
		    int bestMove = legalMoves[0];
		    int bestScore = losingScore;
		    // 自分の可能手に関して最大化を行う
		    for (int i = 0; i < n; i++) {
			// 可能手のそれぞれに対して自分がその手を指した後
			int[] newState = makeMove(legalMoves[i], player, copyState(theState));
			// 敵の可能手に関して最小化を行う
			int newScore = -MiniMax(opponent(player), newState, theLevel-1, theE);
			if (newScore > bestScore) {
			    bestMove = legalMoves[i];
			    bestScore = newScore;
			}
		    }
		    return bestScore;
		}
	    }
	}

	public int[] getMove (int player, int[] theState) {
	    int[] legalMoves = new int[100];
	    int n = allLegalMoves(legalMoves, player, theState);
	    int bestMove = legalMoves[0];
	    int bestScore = losingScore;
	    for (int i = 0; i < n; i++) {
		int[] newState = makeMove(legalMoves[i], player, copyState(theState));
		int newScore = -MiniMax(opponent(player), newState, this.level-1, this.e);
		if (newScore > bestScore) {
		    bestMove = legalMoves[i];
		    bestScore = newScore;
		}
	    }
	    //	    if (!legalMove(bestMove, player, theState))
	    //		System.out.println("Move:" + bestMove + " N:" + n + " BS:" + bestScore);
	    theState = makeMove(bestMove, player, theState);
	    // System.out.println("Player: "+player+" Move: "+bestMove);
	    displayBoard();
	    return theState;
	}
    }


    /* AlphaBeta Search */

    class AlphaBetaPlayer extends Strategy {
	int level;
	EvalFunction e;

	// theLevel must be strictly greater than 0
	AlphaBetaPlayer(int theLevel, EvalFunction theE) {
	    this.level = theLevel;
	    this.e = theE;
	}

	int AlphaBeta (int player, int[] theState, int alpha, int beta, int theLevel, EvalFunction theE) {
	    if (theLevel == 0) {
		return theE.evalFun(player, theState);
	    } else {
		int[] legalMoves = new int[100];
		int n = allLegalMoves(legalMoves, player, theState);
		if (n == 0) {
		    if (anyLegalMove(opponent(player), theState)) {
			return -AlphaBeta(opponent(player), theState, alpha, beta, theLevel-1, theE); // ここを完成させる
		    } else {
			return finalScore(player, theState);
		    }
		} else {
		    int bestMove = legalMoves[0];
		    int bestScore = losingScore;
		    for (int i = 0; bestScore < beta && i < n; i++) {
			int[] newState = makeMove(legalMoves[i], player, copyState(theState));
			int newScore = -AlphaBeta(opponent(player), newState, alpha, beta, theLevel-1, theE);	// ここを完成させる
			if (newScore > bestScore) {
			    bestMove = legalMoves[i];
			    bestScore = newScore;
			}
			if (newScore > alpha) {
			    alpha = newScore;// ここを完成させる
			}
		    }
		    return bestScore;
		}
	    }
	}

	public int[] getMove (int player, int[] theState) {
	    int[] legalMoves = new int[100];
	    int n = allLegalMoves(legalMoves, player, theState);
	    int bestMove = legalMoves[0];
	    int bestScore = losingScore;
	    int alpha = losingScore;
	    int beta = winningScore;
	    for (int i = 0; bestScore < beta && i < n; i++) {
		int[] newState = makeMove(legalMoves[i], player, copyState(theState));
		int newScore = -AlphaBeta(opponent(player), newState, -beta, -alpha, this.level-1, this.e);
		if (newScore > bestScore) {
		    bestMove = legalMoves[i];
		    bestScore = newScore;
		}
		if (newScore > alpha) {
		    alpha = newScore;
		}
	    }
	    theState = makeMove(bestMove, player, theState);
	    System.out.println("Player: "+player+" Move: "+bestMove);
	    displayBoard();
	    return theState;
	}
    }

    /*
    ** ゲームの実行 **
    */

    public void run(){
	while (true) {
	    aLabel0.setText("戦略を選択してPlayを押してください");
	    //	    aLabel1.setText("");
	    while (!runflag) {};
	    state = initialState();
	    displayBoard();
	    for(player = black; player > 0; player=nextPlayer(player, state)){
		//		System.out.println("Player : " + player);
		//		System.out.println("B:" + count(black, state) + " W:" + count(white, state));
		if (player == black) {
		    aLabel0.setText("黒の手番です");
		    state = bStrategy.getMove(player, state);
		} else {
		    aLabel0.setText("白の手番です");
		    state = wStrategy.getMove(player, state);
		}
	    }
	    // Game over (player < 0) 
	    //	    int score = count(black, state)-count(white, state);
	    int score = countDifference(black, state);
	    if (score > 0) {
		aLabel1.setText("ゲーム終了: 結果 黒の勝ち[" + score + "]");
	    } else if (score < 0) {
		aLabel1.setText("ゲーム終了: 結果 白の勝ち[" + -score + "]");
	    } else {
		aLabel1.setText("ゲーム終了: 引き分け");
	    }
	    runflag = false;
	}
    }

    //---------------------------------------------------
    // main関数
    //---------------------------------------------------
    public static void main(String[] args) {
	System.out.println("Let's play Othello Game\n");
	Othello othelloGame = new Othello();
	othelloGame.run();
    }

}

