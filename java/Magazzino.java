import java.util.HashMap;
import java.util.Map;

public class Magazzino {
    private HashMap<String, Integer> oggetti = new HashMap<String, Integer>();

    public void print() {
        for (Map.Entry<String, Integer> entry : oggetti.entrySet()) {
            String key = entry.getKey();
            Integer value = entry.getValue();

            System.out.println("Nel magazzino sono presenti " + value + " unita' di " + key);
        }
    }

    public synchronized boolean exist(String oggetto) {
        try {
            Thread.sleep(500);
        } catch (Exception e) {}

        return oggetti.containsKey(oggetto);
    }

    public synchronized void addOggetto(String oggetto, int quantita) {
        if (this.exist(oggetto)) {
            System.out.println("E' stato richiesto un addOggetto");
            oggetti.replace(oggetto, oggetti.get(oggetto) + new Integer(quantita));
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                e.printStackTrace();
                System.exit(1);
            }
        }
        else{
            System.out.println("L'oggetto di cui si vuole modificare la quantita' non esiste");
        }
    }

    public synchronized void removeOggetto(String oggetto, int quantita){
        if(this.exist(oggetto) && oggetti.get(oggetto) >= quantita){
            System.out.println("E' stato richiesto un removeOggetto");
            oggetti.replace(oggetto, oggetti.get(oggetto) - new Integer(quantita));
            try {
                Thread.sleep(500);
            } catch (Exception e) {
                e.printStackTrace();
                System.exit(1);
            }
        }
        else{
            System.out.println("L'oggetto di cui si vuole modificare la quantita' non esiste");
        }
    }

    public synchronized void creaOggetto(String oggetto){
        if(!this.exist(oggetto)){
            System.out.println("E' stato richiesto un creaOggetto");
            oggetti.put(oggetto, new Integer(0));
            try {
                Thread.sleep(500);
            } catch (Exception e) {
                e.printStackTrace();
                System.exit(1);
            }
        }
        else{
            System.out.println("L'oggetto che vuoi creare esiste gia'");
        }
    }
}