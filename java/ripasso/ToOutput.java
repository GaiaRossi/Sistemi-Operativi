import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.PipedInputStream;

public class ToOutput implements Runnable{

    private final int DIMENSIONE_MESSAGGIO = 100;
    private byte[] messaggio = new byte[DIMENSIONE_MESSAGGIO];
    private PipedInputStream inputStream;
    private String messaggioString;

    public ToOutput(PipedInputStream inputStream){
        this.inputStream = inputStream;
    }

    private void leggiMessaggioByte() throws Exception{
        inputStream.read(messaggio);
    }

    private String convertiByteToString(){
        return new String(messaggio);
    }

    private void stampaMessaggioByte() throws Exception{
        System.out.println("Messaggio: " + convertiByteToString());
    }

    private void leggiMessaggioBuffered() throws Exception{
        BufferedReader inputBuffered = new BufferedReader(new InputStreamReader(inputStream));
        messaggioString = inputBuffered.readLine();
        inputBuffered.close();
    }

    private void stampaMessaggioBuffered() throws Exception{
        System.out.println("Messaggio: " + messaggioString);
    }

    private Message leggiMessaggioSerialized() throws Exception{
        Message message;
        ObjectInputStream objectInputStream = new ObjectInputStream(inputStream);
        message = (Message)objectInputStream.readObject();
        return message;
    }

    private void riceviEStampaMessaggioSerialized() throws Exception{
        Message message = leggiMessaggioSerialized();
        System.out.println("Messaggio: " + message.getMessaggio());
    }

    public void run(){
        try {
            //leggiMessaggioByte();
            //stampaMessaggioByte();
            //leggiMessaggioBuffered();
            //stampaMessaggioBuffered();
            riceviEStampaMessaggioSerialized();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
}