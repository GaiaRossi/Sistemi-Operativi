import java.util.HashMap;
import java.util.Map;
import java.util.Random;
import java.util.concurrent.atomic.AtomicBoolean;

public class ThreadLoad implements Runnable{
    private HashMap<Integer, Double> processi = new HashMap<Integer, Double>();
    private int numero_processi;
    private AtomicBoolean running = new AtomicBoolean(false);

    public ThreadLoad(int numero_processi){
        this.numero_processi = numero_processi;
        generaProcessi();
    }
    private void generaProcessi(){
        int i;
        Integer pid;
        Double cpu_load;
        //devo creare questo oggetto random
        //per fare una chiamata non statica
        Random rand = new Random();
        for(i = 0; i < numero_processi; i++){
            pid = new Integer(rand.nextInt(10));
            cpu_load = new Double(rand.nextDouble()*100);
            processi.put(pid, cpu_load);
        }
    }

    private synchronized void changeCpuLoads(){
        Random rand = new Random();
        for(Map.Entry<Integer, Double> entry : processi.entrySet()){
            Integer key = entry.getKey();
            processi.replace(key, rand.nextDouble()*100);
        }
    }

    public boolean isInterrupted(){
        return !running.get();
    }

    public void interrupt(){
        running.set(false);
        Thread.currentThread().interrupt();
    }

    public void run(){
        running.set(true);
        while(running.get()){
            changeCpuLoads();
            try {
                Thread.sleep(50);
            } catch (Exception e) {
            }
        }
    }

    //0 e' il pid
    //1 e' l'uso della cpu
    public synchronized double[] getHigherCPUUsage(){
        
        double max = -1;
        int maxPid = -1;
        for(Map.Entry<Integer, Double> entry : processi.entrySet()){
            Integer key = entry.getKey();
            Double value = entry.getValue();
            if(value.doubleValue() > max){
                max = value.doubleValue();
                maxPid = key.intValue();
            }
        }

        double[] return_values = new double[2];
        return_values[0] = maxPid;
        return_values[1] = max;
        return return_values;
    }
}