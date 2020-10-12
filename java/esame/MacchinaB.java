import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.PipedInputStream;
import java.util.concurrent.atomic.AtomicBoolean;

public class MacchinaB extends Thread {

    private PipedInputStream input;
    private AtomicBoolean running = new AtomicBoolean(false);
    private byte messaggioByte[] = new byte[10];
    private String messaggio;

    public MacchinaB(PipedInputStream input) {
        this.input = input;
    }

    public void stopThread() {
        try {
            input.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        running.set(false);
        this.interrupt();
    }

    public void run() {
        running.set(true);
        while (running.get()) {
            try {
                //riceviMessaggioStream();
                //riceviMessaggioBuffered();
                riceviMessaggioObject();
                Thread.sleep(200);
                System.out.println("Prodotto fatto");
            } catch (Exception e) {
                //e.printStackTrace();
            }
        }
    }

    private void riceviMessaggioStream() {
        try {
            input.read(messaggioByte);
        } catch (IOException e) {
            e.printStackTrace();
        }
        messaggio = new String(messaggioByte);
        System.out.println("Messaggio letto: " + messaggio);
    }

    private void riceviMessaggioBuffered() throws Exception{
        BufferedReader inputWriter = new BufferedReader(new InputStreamReader(input));
        messaggio = inputWriter.readLine();
        System.out.println("Messaggio letto: " + messaggio);
    }

    private void riceviMessaggioObject() throws Exception{
        Massaggio messaggio = new Massaggio();
        ObjectInputStream objectInputStream = new ObjectInputStream(input);
        messaggio = (Massaggio)objectInputStream.readObject();
        System.out.println("Messaggio ricevuto " + messaggio.getMessaggio());
    }
}