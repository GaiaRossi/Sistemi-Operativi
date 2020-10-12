import java.util.concurrent.atomic.AtomicBoolean;

public class WorkerThread implements Runnable{
    
    private AtomicBoolean running = new AtomicBoolean(false);
    private long id;

    public void stop(){
        running.set(false);
        this.interrupt();
    }

    private void interrupt(){
        Thread.currentThread().interrupt();
    }

    public boolean isRunning(){
        return running.get();
    }

    public long getId(){
        return id;
    }

    public void run(){
        salvaID();
        stampaID();
        checkIfAskedToStop();
    }

    private void salvaID(){
        id = Thread.currentThread().getId();
    }

    private void stampaID(){
        System.out.println("Creato thread con ID: " + id);
    }

    private void checkIfAskedToStop(){
        while(running.get());
        this.stop();
    }
}