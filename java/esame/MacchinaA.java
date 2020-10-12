import java.io.BufferedWriter;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.OutputStreamWriter;
import java.io.PipedOutputStream;
import java.util.concurrent.atomic.AtomicBoolean;

public class MacchinaA extends Thread {

    private AtomicBoolean running = new AtomicBoolean(false);
    private PipedOutputStream output;

    public MacchinaA(PipedOutputStream output) {
        this.output = output;
    }

    public void stopThread() {
        try{
            output.close();
        } catch(Exception e){
            e.printStackTrace();
        }
        running.set(false);
        this.interrupt();
    }

    public void run() {
        running.set(true);
        while (running.get()) {
            try {
                Thread.sleep(200);
                //inviaMessaggioStream();
                //inviaMessaggioBuffered();
                inviaMessaggioObject();
            } catch (Exception e) {
                //e.printStackTrace();
            }
        }
    }

    private void inviaMessaggioStream() {
        String messaggio = "Fatto";
        try {
            output.write(messaggio.getBytes());
            output.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    private void inviaMessaggioBuffered() throws Exception{
        BufferedWriter outputWriter = new BufferedWriter(new OutputStreamWriter(output));
        outputWriter.write("Fatto\n");
        outputWriter.flush();
    }

    private void inviaMessaggioObject() throws Exception{
        Massaggio messaggio = new Massaggio();
        ObjectOutputStream objectOutputStream = new ObjectOutputStream(output);
        objectOutputStream.writeObject(messaggio);
        objectOutputStream.flush();
    }
}