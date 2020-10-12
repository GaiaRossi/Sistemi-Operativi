import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;

public class MainLab7Es2 {
    public static void main(String[] args) throws Exception{
        PipedOutputStream outputStream = new PipedOutputStream();
        PipedInputStream inputStream = new PipedInputStream(outputStream);

        BufferedReader tastiera = new BufferedReader(new InputStreamReader(System.in));

        FromInput fromInput = new FromInput(tastiera, outputStream);
        ToOutput toOutput = new ToOutput(inputStream);

        Thread threadFromInput = new Thread(fromInput);
        Thread threadToOutput = new Thread(toOutput);

        System.out.println("Avvio thread");
        threadFromInput.start();
        threadToOutput.start();

        System.out.println("Attesa terminazione");
        
        threadToOutput.join();
        threadFromInput.join();
    }
}