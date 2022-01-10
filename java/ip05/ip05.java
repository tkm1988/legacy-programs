import org.jfree.chart.*;
import org.jfree.chart.plot.CategoryPlot;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.axis.*;
import org.jfree.data.DefaultKeyedValues;
import org.jfree.data.category.CategoryDataset;
import org.jfree.data.general.DatasetUtilities; 
import org.jfree.data.general.DefaultPieDataset;
import java.*;
import java.io.File;
import java.io.IOException;
import java.math.BigDecimal;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;

public class ip05{
    public static void Histgram(String InputFilePath) throws IOException{
	File iFile = new File(InputFilePath);
	BufferedImage img;
	try{img = ImageIO.read(iFile);}
	catch(IOException e){System.out.println(e);throw e;}
	int iWidth = img.getWidth();
	int iHeight = img.getHeight();
//	int MaxBrightnessValue = 0;
	int [] BrightnessArray = new int[256];
	DefaultKeyedValues values = new DefaultKeyedValues();

	for(int y = 0; y < iHeight; y++){
	    for(int x = 0; x < iWidth; x++){
		int iRGB = img.getRGB(x, y);
		//RGB : 0xAARRGGBB(Hex)
		int iRed = (iRGB >> 16) & 0x000000FF;
		int iGreen = (iRGB >> 8) & 0x000000FF;
		int iBlue = iRGB & 0x000000FF;
		int iBrightness = (int)((0.299 * iRed) + (0.587 * iGreen) + (0.114 * iBlue));
		BrightnessArray[iBrightness]++;
	    }
	}
	for(int i = 0; i < 256; i++){
	    values.addValue(new String(Integer.toString(i)), new BigDecimal(BrightnessArray[i]));
//	    MaxBrightnessValue = Math.max(BrightnessArray[i], MaxBrightnessValue);
	}
	CategoryDataset categoryData = DatasetUtilities.createCategoryDataset("Brightness Value", values);
	JFreeChart jFreeChart = ChartFactory.createLineChart("Histgram", "Pixels", "Repetition", categoryData, PlotOrientation.VERTICAL, true, false, false);
	CategoryPlot categoryPlot = jFreeChart.getCategoryPlot();
	ValueAxis valueAxis = categoryPlot.getRangeAxis();
	valueAxis.setRange(0, 10000);
	
	ChartUtilities.saveChartAsPNG(new File(InputFilePath + ".png"), jFreeChart, 400, 300);
    }
    public static void main(String [] args){
	try{
	    Histgram(args[0]);
	}catch(IOException e){
	    System.out.println(e);
	}
    }
}
