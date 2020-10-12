import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PipedOutputStream;
import java.util.concurrent.atomic.AtomicBoolean;

public class InputUtente implements Runnable {

    private BufferedWriter output = null;
    private PipedOutputStream pos = null;
    private String messaggio;
    private AtomicBoolean running = new AtomicBoolean(false);
    private BufferedReader tastiera = new BufferedReader(new InputStreamReader(System.in));

    public InputUtente(PipedOutputStream out) {
        pos = out;
    }

    public void stop() {
        running.set(false);
        try {
            output.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void run() {
        output = new BufferedWriter(new OutputStreamWriter(pos));
        running.set(true);
        while (running.get()) {
            try {
                System.out.println("Inserisci una stringa");
                messaggio = tastiera.readLine();
                output.write(messaggio + "\n");
                output.flush();
            } catch (IOException e) {
                e.printStackTrace();
            }

        }
    }
}