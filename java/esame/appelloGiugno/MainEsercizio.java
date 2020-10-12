import java.io.PipedInputStream;
import java.io.PipedOutputStream;

public class MainEsercizio {
    public static void main(String[] args) throws Exception{
        PipedInputStream pis = new PipedInputStream();
        PipedOutputStream pos = new PipedOutputStream(pis);

        ScansionaInput si = new ScansionaInput(pis);
        InputUtente iu = new InputUtente(pos);

        Thread tsi = new Thread(si);
        Thread tiu = new Thread(iu);

        tsi.start();
        tiu.start();

        Thread.sleep(3000);

        si.stop();
        iu.stop();

        tsi.join();
        tiu.join();
    }
}