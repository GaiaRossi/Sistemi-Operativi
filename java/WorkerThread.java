import java.util.concurrent.atomic.AtomicBoolean;

public class WorkerThread implements Runnable {

    private final AtomicBoolean running = new AtomicBoolean(false);
    private long id;

    public void run() {
        running.set(true);
        id = Thread.currentThread().getId();
        System.out.println("Thread con id " + id);
        while (running.get()) {
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }

    public long getId(){
        return id;
    }

    //per il primo metodo commenta la seconda riga della funzione
    public void interrupt(){
        running.set(false);
        Thread.currentThread().interrupt();
    }

    public boolean isStopped(){
        return !running.get();
    }
}