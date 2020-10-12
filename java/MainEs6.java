import java.io.*;

public class MainEs6 {
    public static void main(String args[]){
        double tempSoglia = 0;
        BufferedReader tastiera = new BufferedReader(new InputStreamReader(System.in));

        do{
            System.out.println("Inserisci la temperatura minima che vuoi raggiungere");
            System.out.println("(Deve essere un valore compreso tra 18 e 21)");
            try{
                tempSoglia = Double.parseDouble(tastiera.readLine());
            }
            catch(Exception e){
                System.err.println("Errore nell'inserimento della temperatura");
                System.exit(1);
            }
        }while(tempSoglia < 18 || tempSoglia > 21);

        //thread
        PipedOutputStream outs = new PipedOutputStream();
        PipedInputStream ins = null;
        try{
            ins = new PipedInputStream(outs);
        }
        catch(Exception e){
            e.printStackTrace();
            System.err.println("Errore apertura pipedinputstream");
            System.exit(1);
        }

        //avviare i thread
        Thread s = new Thread(new Sensor(outs));
        Thread a = new Thread(new Attuator(ins, tempSoglia));
        
        s.start();
        a.start();

        try {
            Thread.sleep(2000);
        } catch (Exception e) {
            System.err.println("Errore nella sleep");
            System.exit(2);
        }

        s.interrupt();
        a.interrupt();
        try{
            s.join();
            a.join();
        }
        catch(Exception e){
            e.printStackTrace();
            System.err.println("Errore nella join");
            System.exit(3);
        }
    }
}