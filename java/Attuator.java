import java.io.PipedInputStream;
import java.util.concurrent.atomic.AtomicBoolean;

public class Attuator implements Runnable{
    
    private final AtomicBoolean running = new AtomicBoolean(false);
    PipedInputStream in;
    byte[] readValue = new byte[10];
    double temp, minVal;

    public Attuator(PipedInputStream in, double minVal){
        this.in = in;
        this.minVal = minVal;
    }

    public void interrupt(){
        running.set(false);
        Thread.currentThread().interrupt();
    }

    public void run(){
        running.set(true);
        while(running.get()){
            try{
                in.read(readValue);
            }
            catch(Exception e){
                this.interrupt();
            }
            temp = bTd(readValue);
            System.out.println("Letto: " + new String(readValue));
            if(temp < minVal){
                System.out.println("Accendi riscaldamento, valore temperatura corrente: " + temp);
            }
            else{
                System.out.println("Valore temperatura corrente: " + temp);
            }
        }
    }

    public double bTd(byte[] val){
        return Double.parseDouble(new String(val));
    }
}