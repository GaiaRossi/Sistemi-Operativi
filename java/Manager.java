import java.io.PipedInputStream;
import java.util.concurrent.atomic.AtomicBoolean;

public class Manager implements Runnable{
    
    private PipedInputStream pis;
    private int numero_messaggi;
    private AtomicBoolean running = new AtomicBoolean(false);
    private Monitor monitor;
    private Sorter sorter;
    private ThreadLoad tl;

    public Manager(PipedInputStream pis, Monitor monitor, Sorter sorter, ThreadLoad tl){
        this.pis = pis;
        numero_messaggi = 0;
        this.monitor = monitor;
        this.sorter = sorter;
        this.tl = tl;
    }

    public void interrupt(){
        running.set(false);
        Thread.currentThread().interrupt();
    }

    public void run(){
        byte[] messaggio_letto = new byte[1024];
        running.set(true);
        while(running.get()){
            try {
                pis.read(messaggio_letto);
                System.out.println(new String(messaggio_letto));
                numero_messaggi++;
                if(numero_messaggi==10){
                    if(!monitor.isInterrupted()){
                        monitor.interrupt();
                    }
                    if(!sorter.isInterrupted()){
                        sorter.interrupt();
                    }
                    if(!tl.isInterrupted()){
                        tl.interrupt();
                    }
                    System.out.println("Sto per interrompere il manager");
                    this.interrupt();
                }
            } catch (Exception e) {
                System.err.println("Errore terminazione threads");
                System.exit(1);
            }
        }

    }
}