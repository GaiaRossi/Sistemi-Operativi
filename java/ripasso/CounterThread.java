import java.util.Random;

public class CounterThread implements Runnable{

    private Accumulatore accumulatore;

    public CounterThread(Accumulatore actual_accumulatore){
        accumulatore = actual_accumulatore;
    }

    public void run(){
        Random random = new Random();
        double randomValue = random.nextDouble();
        accumulatore.addValue(randomValue);
        System.out.println("Ho aggiunto " + randomValue);
    }
}