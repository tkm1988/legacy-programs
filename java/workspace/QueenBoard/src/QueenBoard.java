import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class QueenBoard extends JFrame implements ActionListener{
	/** 
	•@param args 
	*/ 
	int boardSize=8; 
	JLabel title;	// Eight Queen ... 
	JButton[][] buttonGrid;	// 盤 
	int[][] state;	//盤面の状況 コマがある（１）、利き筋である(-1), 何もない(0) 
	ImageIcon queenOn;	// コマの絵(黒) 
	ImageIcon queenChk;	// コマの絵(赤) 
	ImageIcon queenOff;	// 空白の絵 
	boolean trapChk=true;    // 利きゴマの表示をするか 
	JButton iButton; // 盤面のクリア用ボタン 
	JRadioButton dButton; // 利き筋の表示・非表示切り替えボタン 
	Label aLabel1; // クィーンの数表示用ラベル 
	int queenNum=0; // 有効なクィーンの数

	//Constructor
	public QueenBoard(){
		this.setSize(400, 400);
		this.addWindowListener(new WindowAdapter(){
			public void WindowClosing(WindowEvent ev){
				System.exit(0);
			}
		});
		//ウィンドウの初期化 
		getContentPane().setLayout(new BorderLayout()); 
		  
		// 色違いの queen アイコンを読み込み 
		queenOn  = new ImageIcon(Toolkit.getDefaultToolkit().getImage( "queen-on.gif" ));  // 黒 
		queenOff = new ImageIcon(Toolkit.getDefaultToolkit().getImage( "queen-off.gif" )); // 空白 
		queenChk = new ImageIcon(Toolkit.getDefaultToolkit().getImage( "queen-chk.gif" )); // 赤 

		state = new int[boardSize][boardSize]; 

		//盤面グリット部分の初期化　ボタンを用いる 
		buttonGrid = new JButton[boardSize][boardSize];   // チェス盤のマス目 
		JPanel backpanel = new JPanel(new GridLayout(boardSize, boardSize));   // チェス盤 
		for(int i=0; i<boardSize; i++) {
			for(int j=0; j<boardSize; j++) { 
				buttonGrid[i][j] = new JButton( ) ;  // マス目を生成 
				buttonGrid[i][j].addActionListener(this); 
				buttonGrid[i][j].setIcon(queenOff); 
				state[i][j] = 0; 
				backpanel.add(buttonGrid[i][j]);    // マス目をチェス盤に敷き詰め 
			} 
		}

		// チェス盤を Center に貼り付け 
		getContentPane().add(backpanel, SwingConstants.CENTER);  
		// タイトル N-Queen 
		title = new JLabel(boardSize + "-Queen   ", SwingConstants.CENTER); 
		//   North に貼り付け 
		getContentPane().add(title, BorderLayout.NORTH); 

		// 盤面の初期化ボタン 
		iButton = new JButton("Init"); 
		iButton.addActionListener(new ActionListener() { 
			public void actionPerformed(ActionEvent e){ 
				if (e.getSource().equals(iButton)){
					gridInit();
				}
			}
		}); 
		JRadioButton dButton = new JRadioButton("利き筋を表示", true); 
		trapChk = true;	// 初期値は、表示と指定 
		dButton.addItemListener(new ItemListener() { 
			public void itemStateChanged(ItemEvent e) { 
					if(e.getStateChange()==ItemEvent.DESELECTED) trapChk = false; 
					else	trapChk = true; 
			} 
		}); 
		aLabel1 = new Label();
		queenNum=0; 
		aLabel1.setText(Integer.toString(boardSize));
		JPanel southpan = new JPanel(new GridLayout(0,1)); 
		southpan.add(iButton);   
		southpan.add(dButton); 
		southpan.add(aLabel1); 
		getContentPane().add(southpan, BorderLayout.SOUTH);	// South に貼り付け 
		
		this.setVisible(true);
	}

	public void gridInit(){ 
		for(int i=0; i<boardSize; i++){
			for(int j=0; j<boardSize; j++){ 
				state[i][j]= 0; 
				buttonGrid[i][j].setBackground(Color.WHITE); 
				buttonGrid[i][j].setIcon(queenOff); 
			} 
		}
		queenNum=0; 
		aLabel1.setText(Integer.toString(boardSize)); 
	}  
	
	private boolean legal(int x, int y) { 
		return ((x >= 0 && x < boardSize) && (y >= 0 && y < boardSize)); 
	} 

	public void checkState(int ip, int it){ 
		// 横の利き筋を設定 
		for(int ix=0; ix<boardSize; ix++) state[ix][it]= -1; 
		//縦の利き筋を設定 
		for(int iy=0; iy<boardSize; iy++) state[ip][iy]= -1; 
		//右上斜め方向の利き筋 
		for (int i=1; legal(ip+i,it+i); i++) state[ip+i][it+i]= -1; 
		//右下斜め方向の利き筋 
		for (int i=1; legal(ip+i,it-i); i++) state[ip+i][it-i]= -1; 
		//左上斜め方向の利き筋 
		for (int i=1; legal(ip-i,it+i); i++) state[ip-i][it+i]= -1; 
		//左下斜め方向の利き筋 
		for (int i=1; legal(ip-i,it-i); i++) state[ip-i][it-i]= -1; 
	}
	
	//利き筋となったグリッドに色を塗る関数 
	public void drawCell() { 
		for(int i=0; i < boardSize; i++){
			for(int j=0; j < boardSize; j++) { 
				if((state[i][j] == -1) || (state[i][j] == 1)){ 
					buttonGrid[i][j].setBackground(Color.ORANGE);
				} 
			}
		}
	}

	//盤上のグリッドとなっているボタンが押されたときに呼び出される関数 
	public void actionPerformed(ActionEvent e) { 
		//ActionEvent e に，イベントが発生した場所が格納されているので 
		//それを元に，どのボタンが押されたかを判別し， 
		//対応したRenderPanelのメソッドを呼び出す． 
		for(int ip=0; ip < boardSize; ip++){ 
		    for(int it=0; it < boardSize; it++){ 
		        if (e.getSource().equals(buttonGrid[ip][it])){
			    	if((state[ip][it] == -1) || (state[ip][it] == 1)){ 
			    		buttonGrid[ip][it].setIcon(queenChk); 
			    		aLabel1.setText(Integer.toString(boardSize-queenNum)+" Game Over");
			    		break; 
			    	}else{ 
			    		buttonGrid[ip][it].setIcon(queenOn); 
			    		state[ip][it] = 1; 
			    		checkState(ip, it); 
			    		if(trapChk)	drawCell(); 
			    		queenNum++;     
			    		aLabel1.setText(Integer.toString(boardSize-queenNum));
			    		break; 
			    	} 
		        }
		   } 
		}		
	} 
	
	public static void main(String [] args){
		//TODO Auto-generated method stub
		System.out.println("Lets start QueenBoard\n");
		QueenBoard aQueenBoard = new QueenBoard();
	}
}

