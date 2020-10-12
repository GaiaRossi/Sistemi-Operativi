import java.util.concurrent.atomic.AtomicBoolean;

public class Monitor implements Runnable{

    private AtomicBoolean running = new AtomicBoolean(false);

    public void interrupt(){
        System.out.println("monitor interrotto");
        running.set(false);
        Thread.currentThread().interrupt();
    }

    public void run(){
        running.set(true);
        while(running.get()){
            try {
                Thread.sleep(200);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public boolean isInterrupted(){
        return !running.get();
    }
    
}