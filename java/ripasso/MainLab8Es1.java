import java.io.BufferedReader;
import java.io.InputStreamReader;

public class MainLab8Es1 {
    public static void main(String[] args) throws Exception{
        if(args.length != 1){
            System.err.println("Uso: java MainLab8Es1 <numero_workerThread>");
            System.exit(1);
        }

        BufferedReader tastiera = new BufferedReader(new InputStreamReader(System.in));
        int numero_thread = Integer.parseInt(args[0]);
        int i, id_thread_to_stop, thread_attivi = 0;


        System.out.println("Creazione di " + numero_thread + " thread");
        Thread thread;
        WorkerThread workerThreads[] = new WorkerThread[numero_thread];
        
        for(i = 0; i < numero_thread; i++){
            workerThreads[i] = new WorkerThread();
            thread = new Thread(workerThreads[i]);
            thread.start();
            thread_attivi++;
        }

        while(thread_attivi > 0){
            System.out.println("Inserisci l'id del thread da terminare");
            id_thread_to_stop = Integer.parseInt(tastiera.readLine());
            boolean trovato = false;
            for(i = 0; i < numero_thread && !trovato; i++){
                if(workerThreads[i].getId() == (long)id_thread_to_stop){
                    trovato = true;
                    workerThreads[i].stop();
                    thread_attivi--;
                }
            }
            if(trovato == false){
                System.out.println("L'id che hai inserito non e' valido");
            }
        }

        System.out.println("Tutti i thread sono stati terminati");

    }
}