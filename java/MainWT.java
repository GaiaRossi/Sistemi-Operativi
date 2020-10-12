import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class MainWT {
    public static void main(String args[]) {
        int n = 0, i, threadAvviati;
        // controlli sull'argomento
        try {
            n = Integer.parseInt(args[0]);
        } catch (Exception e) {
            System.err.println("Inserire il numero (intero) di thread da creare \nUso: java MainWT <num_Thread>");
            System.exit(1);
        }
        threadAvviati = n;
        WorkerThread[] threads = new WorkerThread[n];

        // creazione e avvio dei thread
        for (i = 0; i < n; i++) {
            threads[i] = new WorkerThread();
            Thread t = new Thread(threads[i]);
            t.start();
        }

        try {
            Thread.sleep(100);
        } catch (InterruptedException e1) {
            e1.printStackTrace();
        }

        // richiesta thread da terminare
        String tid = null;
        long id = 0;
        BufferedReader tastiera = new BufferedReader(new InputStreamReader(System.in));

        while(threadAvviati > 0){   
            try {
                System.out.println("Inserisci l'id del thread che vuoi terminare:");
                tid = tastiera.readLine();
            } catch (IOException e) {
                System.err.println("Errore lettura con tastiera");
                System.exit(2);
            }

            try {
                id = Long.parseLong(tid);
            } catch (Exception e) {
                System.out.println("Inserire un numero valido");
                continue;
            }

            if(id < threads[0].getId()|| id > threads[n - 1].getId()){
                System.out.println("Devi inserire un ID esistente");
                continue;
            }

            for(i = 0; i < n; i++){
                if(id == threads[i].getId() && !threads[i].isStopped()){
                    System.out.println("Thread con id " + id + " e' stato correttamente interrotto");
                    threads[i].interrupt();
                    threadAvviati--;
                    break;
                }
                else if(id == threads[i].getId() && threads[i].isStopped()){
                    System.out.println("Il thread con id " + id + " e' gia' stato interrotto");
                }
            }
        }
        System.out.println("Tutti i thread terminati, chiusura in corso..");
    }
}