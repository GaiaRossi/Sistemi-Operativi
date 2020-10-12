import java.util.Random;
import java.util.concurrent.atomic.AtomicBoolean;

public class MacchinaA extends Thread{
    
    private AtomicBoolean running = new AtomicBoolean(false);
    private ControllaProduzione controllaProduzione;

    public MacchinaA(ControllaProduzione controllaProduzione){
        this.controllaProduzione = controllaProduzione;
    }

    public void stopThread(){
        running.set(false);
        Thread.currentThread().interrupt();
    }

    public void run(){
        running.set(true);
        while(running.get()){
            try {
                int sleep_time = randomTime();
                Thread.sleep(sleep_time);
                controllaProduzione.incrementaSemilavorati();
            } catch (Exception e) {
                //e.printStackTrace();
            }
        }
    }

    private int randomTime(){
        Random value = new Random();
        return value.nextInt(101) + 400;
    }
}