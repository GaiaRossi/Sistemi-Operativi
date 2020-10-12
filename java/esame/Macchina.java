import java.util.Random;
import java.util.concurrent.atomic.AtomicBoolean;

public class Macchina implements Runnable{
    
    private Produzione produzione;
    AtomicBoolean running = new AtomicBoolean(false);

    public Macchina(Produzione produzione){
        this.produzione = produzione;
    }
    
    public void interrupt(){
        running.set(false);
        Thread.currentThread().interrupt();
    }

    public boolean isRunning(){
        return running.get();
    }

    public void run(){
        running.set(true);
        int i = 0;
        while(running.get() && i < 35){
            try {
                Random random = new Random();
                int tempo_produzione = random.nextInt(101) + 500;
                Thread.sleep(tempo_produzione);
                produzione.incremento();
                i++;
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        if(i == 35){
            this.interrupt();
        }
    }
}