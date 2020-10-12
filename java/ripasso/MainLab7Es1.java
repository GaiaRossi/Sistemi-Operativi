import java.io.BufferedReader;
import java.io.InputStreamReader;

public class MainLab7Es1 {
    public static void main(String[] args) throws Exception{
        BufferedReader tastiera = new BufferedReader(new InputStreamReader(System.in));
        Accumulatore accumulatore = new Accumulatore(0.0);
        int numero_thread, i;

        System.out.println("Inserisci il numero di thread da generare");
        numero_thread = Integer.parseInt(tastiera.readLine());

        System.out.println("Valore iniziale dell'accumulatore: " + accumulatore.getValoreAccumulatore());

        System.out.println("Generazione thread");
        CounterThread sommatore;
        Thread counterThread[] = new Thread[numero_thread];
        for(i = 0; i < numero_thread; i++){
            sommatore = new CounterThread(accumulatore);
            counterThread[i] = new Thread(sommatore);
            counterThread[i].start();
        }

        System.out.println("Thread avviati");
        System.out.println("Attesa terminazione");

        for(i = 0; i < numero_thread; i++){
            counterThread[i].join();
        }

        System.out.println("Thread terminati");
        System.out.println("Valore attuale dell'accumulatore: " + accumulatore.getValoreAccumulatore());
    }
}