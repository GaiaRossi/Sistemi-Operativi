import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;

public class MainEs9Lab12 {
    public static void main(String[] args) {
        ControllaProduzione controllaProduzione = new ControllaProduzione();
        MacchinaA macchinaA1 = new MacchinaA(controllaProduzione);
        MacchinaA macchinaA2 = new MacchinaA(controllaProduzione);
        PipedOutputStream output = new PipedOutputStream();
        PipedInputStream input = null;
        try {
            input = new PipedInputStream(output);
        } catch (IOException e1) {
            e1.printStackTrace();
        }
        BufferedReader inputStream = new BufferedReader(new InputStreamReader(input));
        MacchinaB macchinaB = new MacchinaB(controllaProduzione, output);

        System.out.println("Avvio thread");
        macchinaA1.start();
        macchinaA2.start();

        macchinaB.start();

        System.out.println("Ascolto produzione");
        int i = 0;
        while(i < 15) {
            try {
                inputStream.readLine();
                i++;
            } catch (IOException e) {
                //e.printStackTrace();
            }
        }

        //System.out.println("Produzione terminata");

        macchinaA1.stopThread();
        macchinaA2.stopThread();
        macchinaB.stopThread();

        try {
            inputStream.close();

            macchinaA1.join();
            macchinaA2.join();
            macchinaB.join();
        } catch (Exception e) {
            e.printStackTrace();
        } 

        System.out.println("In controllo produzione ci sono " + controllaProduzione.getSemilavorati() + " semilavorati");
        System.out.println(controllaProduzione.getFiniti() + " finiti");

        /*
        try {
            Thread.sleep(1000);
        } catch (Exception e) {
            e.printStackTrace();
        }

        System.out.println("In controllo produzione ci sono " + controllaProduzione.getSemilavorati() + " semilavorati");
        System.out.println(controllaProduzione.getFiniti() + " finiti");
        */

    }
}