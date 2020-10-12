import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PipedInputStream;
import java.util.concurrent.atomic.AtomicBoolean;

public class ScansionaInput implements Runnable {
    private BufferedReader input = null;
    private PipedInputStream pis = null;
    private String messaggio;
    private AtomicBoolean running = new AtomicBoolean(false);

    public ScansionaInput(PipedInputStream in) {
        pis = in;
    }

    public void stop() {
        running.set(false);
        try {
            input.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void run() {
        running.set(true);
        input = new BufferedReader(new InputStreamReader(pis));
        while (running.get()) {
            try {
                messaggio = input.readLine();
                if(messaggio.equals("abcde") || messaggio.equals("1234")){
                    System.out.println("Pericolo");
                }
                else{
                    System.out.println("ok");
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}