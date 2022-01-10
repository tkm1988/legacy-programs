import java.awt.event.*;
import java.awt.*;

import javax.swing.*;

import java.util.*;

public class Lifegame extends JFrame implements ActionListener {
	private JPanel bgCell, bgBottom;
	private JButton[][] Cell;
	private JButton bStart, bClear, bStop;
	private JLabel Time;
	private JTextField tTime;

	private ImageIcon Live, Death;
	private boolean runFlag = false;
	private long interval = 1000, stime;	//[ms]
	private	int row = 50, column = 50;
	private int[][] state;
	
	public Lifegame(){	
		// ウィンドウの大きさ設定
		this.setSize(550, 550);

		//ウィンドウの初期化
		getContentPane().setLayout(new BorderLayout());

		//アイコンの設定
		Live  = new ImageIcon(Toolkit.getDefaultToolkit().getImage("BlackPiece.gif") );  // 生きているセル
		Death  = new ImageIcon(Toolkit.getDefaultToolkit().getImage("BlannkSquare.gif") );  //死んでいるセル

		//セルの配置
		bgCell = new JPanel(new GridLayout(row, column));
		Cell = new JButton[row][column];
		state = new int[row][column];	//セルの状態を表す配列，0だと死で1だと生を表す
		
		for(int i = 0; i < row; i++){
			for(int j = 0; j < column; j++){
				Cell[i][j] = new JButton();
				Cell[i][j].addActionListener(this);
				Cell[i][j].setIcon(Death);
				state[i][j] = 0;
				bgCell.add(Cell[i][j]);
			}
		}

		//ウインドウ中央にセルを設置
		getContentPane().add(bgCell, SwingConstants.CENTER);
		
		//操作部分の作成
		bgBottom = new JPanel(new FlowLayout());
		Time = new JLabel("Time[ms] : ");
		tTime = new JTextField("1000");	//インターバルを設定するためのテキストエリア（初期値は100[ms]）

		//開始ボタン
		bStart = new JButton("START");
		bStart.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if(e.getSource().equals(bStart) && runFlag == false){
					runFlag = true;
					interval = (long)Integer.valueOf(tTime.getText());
					stime = System.currentTimeMillis();
				}
			}
		});
		
		//状態のクリアボタン
		bClear = new JButton("CLEAR");
		bClear.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if(e.getSource().equals(bClear)){
					for(int i = 0; i < row; i++){
						for(int j = 0; j < column; j++){
							Cell[i][j].setIcon(Death);
							state[i][j] = 0;
						}
					}
				}
			}
		});
		
		//停止ボタン
		bStop = new JButton("STOP");
		bStop.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if(e.getSource().equals(bStop) && runFlag == true){
					runFlag = false;
				}
			}
		});
		bgBottom.add(bStart);
		bgBottom.add(bClear);
		bgBottom.add(bStop);
		bgBottom.add(Time);
		bgBottom.add(tTime);

		//ウインドウ下部に操作部分を配置
		getContentPane().add(bgBottom, BorderLayout.PAGE_END);
		
		//ウィンドウを閉じたときにアプリケーションを終了するための処理
		this.addWindowListener(new WindowAdapter(){
			public void windowClosing(WindowEvent ev){
			    System.exit(0);
			}
		});
		
		//ウィンドウの可視化
		this.setVisible(true);
	}

	public void actionPerformed(ActionEvent e) {
		//セルをクリックする事で各セルの生死を変更
		for(int i = 0; i < row; i++){
			for(int j = 0; j < column; j++){
				if(e.getSource().equals(Cell[i][j])){
					if(Cell[i][j].getIcon().equals(Live)){
						Cell[i][j].setIcon(Death);
						state[i][j] = 0;
					}else{
						Cell[i][j].setIcon(Live);
						state[i][j] = 1;
					}
				}
			}
	    }
    }
	
	//セルの状態のコピーを作る(ディープコピー)
	public int[][] CopyState(int[][] state){
		int[][] newstate = new int[row][column];
		
		for(int i = 0; i < row; i++){
			for(int j = 0; j < column; j++){
				newstate[i][j] = state[i][j];
			}
		}
		return newstate;
	}
	
	//各セルのムーア近傍（8-近傍）について調べる
	//ムーア近傍にあたるセルが生きていれば1づつカウントしていく
	public int CheckMooreNeighborhood(int[][] state, int r, int c){
		int value = 0;

		if(r == 0){
			value = value + state[row - 1][c] + state[r + 1][c];
			if(c == 0){
				value = value + state[row - 1][column - 1]
				              + state[row - 1][c + 1]
				              + state[r][column - 1]
				              + state[r][c + 1]
				              + state[r + 1][column - 1]
				              + state[r + 1][c + 1];
			}else if(c == (column - 1)){
				value = value + state[row - 1][c - 1]
							  + state[row - 1][0]
							  + state[r][c - 1]
							  + state[r][0]
							  + state[r + 1][c - 1]
							  + state[r + 1][0];
			}else{
				value = value + state[row - 1][c - 1]
				              + state[row - 1][c + 1]
				              + state[r][c - 1]
				              + state[r][c + 1]
				              + state[r + 1][c - 1]
				              + state[r + 1][c + 1];
			}
		}else if(r == (row - 1)){
			value = value + state[r - 1][c] + state[0][c];
			if(c == 0){
				value = value + state[r - 1][column - 1]
				              + state[r - 1][c + 1]
				              + state[r][column - 1]
				              + state[r][c + 1]
				              + state[0][column - 1]
				              + state[0][c + 1];
			}else if(c == (column - 1)){
				value = value + state[r - 1][c - 1]
							  + state[r - 1][0]
							  + state[r][c - 1]
							  + state[r][0]
							  + state[0][c - 1]
							  + state[0][0];
			}else{
				value = value + state[r - 1][c - 1]
				              + state[r - 1][c + 1]
				              + state[r][c - 1]
				              + state[r][c + 1]
				              + state[0][c - 1]
				              + state[0][c + 1];
			}
		}else if(c == 0){
			value = value + state[r - 1][c]
			              + state[r + 1][c]
						  + state[r - 1][column - 1]
				          + state[r - 1][c + 1]
				          + state[r][column - 1]
				          + state[r][c + 1]
				          + state[r + 1][column - 1]
			              + state[r + 1][c + 1];
		}else if (c == (column - 1)){
			value = value + state[r - 1][c]
   			              + state[r + 1][c]
   						  + state[r - 1][c - 1]
   				          + state[r - 1][0]
   				          + state[r][c - 1]
   				          + state[r][0]
   				          + state[r + 1][c - 1]
   			              + state[r + 1][0];
		}else{
			value = value + state[r - 1][c]
   			              + state[r + 1][c]
  						  + state[r - 1][c - 1]
   				          + state[r - 1][c + 1]
   				          + state[r][c - 1]
   				          + state[r][c + 1]
   				          + state[r + 1][c - 1]
   			              + state[r + 1][c + 1];
		}

		return value;
	}
	
	public void run(){
		while(true){
			if(runFlag == true){
				//インターバルをおいて結果を表示
				if(interval <= (System.currentTimeMillis() - stime)){
					int[][] cstate = CopyState(state);	//状態の複製を生成
					for(int i = 0; i < row; i++){
						for(int j = 0; j < column; j++){
							int live_cell_num = 0;
							
							//ムーア近傍を調べる
							live_cell_num = CheckMooreNeighborhood(cstate, i, j);
							
							//状態の変更
							if(state[i][j] == 0 && live_cell_num == 3){	//誕生
								state[i][j] = 1;
								Cell[i][j].setIcon(Live);
							}else if(state[i][j] == 1 && (live_cell_num == 2 || live_cell_num == 3)){	//維持
								state[i][j] = 1;
								Cell[i][j].setIcon(Live);
							}else{	//死
								state[i][j] = 0;
								Cell[i][j].setIcon(Death);
							}
						}
				    }
					stime = System.currentTimeMillis();//開始時刻の更新
				}
			}
		}
	}

	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO 自動生成されたメソッド・スタブ
		Lifegame lg = new Lifegame();
		lg.run();
	}

}
