import java.io.*;
import org.apache.commons.math.random.MersenneTwister;

class mt_test{
    public static void main(String [] args){
	MersenneTwister mt = new MersenneTwister();

	try{
	    for(int i = 0; i < 50; ++i){
		System.out.println(mt.nextFloat());
	    }
	}catch(Exception e){
	    System.out.println(e);
	}
    }
}