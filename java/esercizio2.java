import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;

public class esercizio2 {
    public static void main(String args[]) throws Exception{
        PipedOutputStream pos = new PipedOutputStream();
        PipedInputStream pis = new PipedInputStream(pos);

        BufferedReader tastiera = new BufferedReader(new InputStreamReader(System.in));

        FromInput fi = new FromInput(pos, tastiera);
        ToOutput to = new ToOutput(pis);

        System.out.println("Esecuzione dei thread");

        fi.start();
        to.start();
        
        fi.join();
        to.join();

    }
}

class FromInput extends Thread {

    String messaggio;
    PipedOutputStream o;
    BufferedReader tastiera;

    public FromInput(PipedOutputStream o, BufferedReader tastiera) {
        super();
        this.o = o;
        this.tastiera = tastiera;
    }

    public void sendMessage() throws Exception {
        messaggio = tastiera.readLine();
        writeMessage();
    }

    void writeMessage() throws Exception {
        o.write(messaggio.getBytes());
    }

    public void run() {
        System.out.println("Iniziato il thread " + Thread.currentThread().getId());
        try {
            sendMessage();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

class ToOutput extends Thread{

    PipedInputStream i;
    byte[] messaggio = new byte[20];

    public ToOutput(PipedInputStream i){
        super();
        this.i = i;
    }

    public void readMessage() throws Exception{
        i.read(messaggio);
        printMessage();
    }

    void printMessage(){
        System.out.println("Il messaggio letto e' " + new String(messaggio));
    }

    public void run(){
        System.out.println("Iniziato il thread " + Thread.currentThread().getId());
        try {
            readMessage();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}