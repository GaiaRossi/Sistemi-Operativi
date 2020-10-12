public class esercizio1Extend extends Thread{

    static AccumulatoreE a = new AccumulatoreE(0);
    static int numeroThread = 3;

    public esercizio1Extend(){
        super();
    }

    public void run(){
        System.out.println("Partito il thread con id " + Thread.currentThread().getId());
        a.addValue(Math.random()*19 + 1);
    }

    public static void main(String args[]) throws Exception{

        esercizio1Extend[] es = new esercizio1Extend[numeroThread];

        //creazione dei thread e partenza degli stessi
        for(int i = 0; i < es.length; i++){
            es[i] = new esercizio1Extend();
            es[i].start();
        }

        //attesa di tutti i thread
        for(int i = 0; i < es.length; i++){
            es[i].join();
        }

        System.out.println("Il valore dell'accumulatore e' " + a.getValue());
    }
}

class AccumulatoreE{ 
    double a;

    public AccumulatoreE(double init_val){
        a = init_val;
    }

    public void addValue(double value){
        a += value;
    }

    public double getValue(){
        return a;
    }
}