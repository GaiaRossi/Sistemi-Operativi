import java.lang.Math;

public class esercizio1Runnable{

    static Accumulatore a = new Accumulatore(0);
    static int numeroThread = 3;

    public static void main(String[] args) throws Exception {
        //creazione di un thread
        Thread[] esercizio = new Thread[numeroThread];

        //creazione dei thread e avvio degli stessi
        for(int i = 0; i < esercizio.length; i++){
            esercizio[i] = new Thread(new CounterThread());
            esercizio[i].start();
        }

        //attesa di tutti i thread
        for(int i = 0; i < esercizio.length; i++){
            esercizio[i].join();
        }

        System.out.println("Il valore dell'accumulatore e' : " + a.getValue());
    }
}

class Accumulatore{
    double a;
    public Accumulatore(double init_val){
        a = init_val;
    }
    public void addValue(double val){
        a += val;
    }
    public double getValue(){
        return a;
    }
}

class CounterThread implements Runnable{

    public void run(){
        System.out.println("Partito thread numero " + Thread.currentThread().getId());
        esercizio1Runnable.a.addValue(Math.random() * 19 + 1);
    }
}