import java.io.PipedOutputStream;
import java.util.concurrent.atomic.AtomicBoolean;

public class Sorter implements Runnable {
    
    private AtomicBoolean running = new AtomicBoolean(false);
    private PipedOutputStream pos;
    private double[] maxProcess = new double[2];
    private ThreadLoad tl;
    private String messaggio;

    public Sorter(PipedOutputStream pos, ThreadLoad tl){
        this.pos = pos;
        this.tl = tl;
    }

    public void interrupt(){
        System.out.println("Sorter interrotto");
        running.set(false);
        Thread.currentThread().interrupt();
    }

    public void run(){
        running.set(true);
        while(running.get()){
            maxProcess = tl.getHigherCPUUsage();
            messaggio = "";
            messaggio = "Il thread a piu' alto cpu usage ha pid: " + maxProcess[0] + " con uso cpu: " + maxProcess[1] + "\n";
            try {
                pos.write(messaggio.getBytes());
                pos.flush();
                Thread.sleep(200);
            } catch (Exception e) {
                e.printStackTrace();
                this.interrupt();
            } 
        }

    }
    
    public boolean isInterrupted(){
        return !running.get();
    }
    
}