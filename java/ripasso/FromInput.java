import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.ObjectOutputStream;
import java.io.OutputStreamWriter;
import java.io.PipedOutputStream;

public class FromInput implements Runnable{
    private BufferedReader tastiera;
    private String messaggio;
    private PipedOutputStream outputStream;

    public FromInput(BufferedReader tastiera, PipedOutputStream outputStream){
        this.tastiera = tastiera;
        this.outputStream = outputStream;
    }

    private void chiediMessaggio() throws Exception{
        System.out.println("Inserisci il messsaggio da inviare");
        messaggio = tastiera.readLine();
    }

    private void inviaMessaggioByte() throws Exception{
        outputStream.write(messaggio.getBytes("UTF-8"));
        outputStream.flush();
    }

    private void inviaMessaggioBuffered() throws Exception{
        BufferedWriter outputBuffered = new BufferedWriter(new OutputStreamWriter(outputStream));
        outputBuffered.write(messaggio);
        outputBuffered.flush();
        outputBuffered.close();
    }

    private void inviaMessaggioSerialized() throws Exception{
        Message message = new Message(messaggio);
        ObjectOutputStream objectOutputStream = new ObjectOutputStream(outputStream);
        objectOutputStream.writeObject(message);
        objectOutputStream.flush();
        objectOutputStream.close();
    }

    public void run(){
        try{
            chiediMessaggio();
            //inviaMessaggioByte();
            //inviaMessaggioBuffered();
            inviaMessaggioSerialized();
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }
}