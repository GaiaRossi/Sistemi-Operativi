import java.io.PipedOutputStream;
import java.util.concurrent.atomic.AtomicBoolean;

public class Sensor implements Runnable{
    
    private final AtomicBoolean running = new AtomicBoolean(false);
    PipedOutputStream out;
    double temp;

    public Sensor(PipedOutputStream out){
        this.out = out;
    }

    public double generaTemp(){
        return Math.random()*(21 - 18) + 18;
    }

    public void interrupt(){
        running.set(false);
        Thread.currentThread().interrupt();
    }

    public void run(){
        running.set(true);
        while(running.get()){
            temp = generaTemp();
            try{
                out.write(dTb(temp));
                Thread.sleep(300);
            }
            catch(Exception e){
                this.interrupt();
            }
        }
    }

    public byte[] dTb(double val){
        return Double.toString(val).getBytes();
    }
}