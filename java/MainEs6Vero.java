import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;

public class MainEs6Vero {

    public static void main(String args[]){
        PipedOutputStream pos = new PipedOutputStream();
        PipedInputStream pis = null;
        BufferedReader tastiera = new BufferedReader(new InputStreamReader(System.in));
        
        int numero_processi = 0;
        try {
            pis = new PipedInputStream(pos);
        } catch (Exception e) {
            e.printStackTrace();
        }

        do{
            System.out.println("Inserisci il numero dei processi da generare");
            try{
                numero_processi = Integer.parseInt(tastiera.readLine());
            }
            catch(Exception e){
                e.printStackTrace();
            }
        }while(numero_processi <= 0);

        ThreadLoad tLoad = new ThreadLoad(numero_processi);
        Monitor monitor = new Monitor();
        Sorter sorter = new Sorter(pos, tLoad);
        Manager manager = new Manager(pis, monitor, sorter, tLoad);

        Thread tl = new Thread(tLoad);
        Thread tmonitor = new Thread(monitor);
        Thread tsorter = new Thread(sorter);
        Thread tmanager = new Thread(manager);

        tl.start();
        tmonitor.start();
        tsorter.start();
        tmanager.start();

        tl.interrupt();

        try{
            tl.join();
            tmonitor.join();
            tsorter.join();
            tmanager.join();
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }
}