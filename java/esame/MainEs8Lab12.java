import java.io.PipedInputStream;
import java.io.PipedOutputStream;

public class MainEs8Lab12 {
    public static void main(String[] args) throws Exception{
        PipedOutputStream output = new PipedOutputStream();
        PipedInputStream input = new PipedInputStream(output);
        
        MacchinaA macchinaA = new MacchinaA(output);
        MacchinaB macchinaB = new MacchinaB(input);

        System.out.println("Avvio thread");
        macchinaA.start();
        macchinaB.start();

        System.out.println("Thread avviati, dormo");
        Thread.sleep(10000);

        System.out.println("Termino i thread");
        macchinaA.stopThread();
        macchinaB.stopThread();
        macchinaA.join();
        macchinaB.join();
    }
}