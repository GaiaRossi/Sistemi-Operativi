import java.io.BufferedWriter;
import java.io.OutputStreamWriter;
import java.io.PipedOutputStream;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.Random;

public class MacchinaB extends Thread {

    private AtomicBoolean running = new AtomicBoolean(false);
    private ControllaProduzione controllaProduzione;
    private PipedOutputStream output;

    public MacchinaB(ControllaProduzione controllaProduzione, PipedOutputStream output) {
        this.controllaProduzione = controllaProduzione;
        this.output = output;
    }

    public void stopThread() {
        running.set(false);
        try {
            output.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        Thread.currentThread().interrupt();
    }

    public void run() {
        running.set(true);
        while (running.get()) {
            while (controllaProduzione.getSemilavorati() == 0) {
                try {
                    Thread.sleep(2000);
                } catch (Exception e) {
                    //e.printStackTrace();
                }
            }
            try {
                int sleep_time = randomTime();
                Thread.sleep(sleep_time);
                controllaProduzione.incrementaFiniti();
                avvertiMain();
            } catch (Exception e) {
                //e.printStackTrace();
            }
        }
    }

    private int randomTime(){
        Random value = new Random();
        return value.nextInt(50) + 100;
    }
    
    private void avvertiMain() throws Exception{
        BufferedWriter outputStream = new BufferedWriter(new OutputStreamWriter(output));
        outputStream.write("fatto\n");
        outputStream.flush();
    }

}