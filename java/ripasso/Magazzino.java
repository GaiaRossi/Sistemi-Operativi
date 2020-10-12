import java.util.HashMap;
import java.util.Map;

public class Magazzino {
    
    private HashMap<String, Integer> oggetti = new HashMap<String, Integer>();

    public synchronized void aggiungiOggetto(String nome_oggetto) throws Exception{
        oggetti.put(nome_oggetto, new Integer(0));
        Thread.sleep(500);
        System.out.println("Creato oggetto");
    }

    public synchronized void aggiungiQuantitaAdOggetto(String nome_oggetto, int value) throws Exception{
        oggetti.replace(nome_oggetto, oggetti.get(nome_oggetto), oggetti.get(nome_oggetto) + new Integer(value));
        Thread.sleep(500);
        System.out.println("Aggiunta quantita'");
    }

    public synchronized void rimuoviQuantitaAdOggetto(String nome_oggetto, int value) throws Exception{
        if(oggetti.get(nome_oggetto) - new Integer(value) >= 0){
            oggetti.replace(nome_oggetto,  oggetti.get(nome_oggetto), oggetti.get(nome_oggetto) - new Integer(value));
            Thread.sleep(500);
            System.out.println("Rimossa quantita'");
        }
    }

    public boolean isAlreadyDefined(String nome_oggetto){
        return oggetti.containsKey(nome_oggetto);
    }

    public void printMagazzino(){
        for(Map.Entry<String, Integer> entry : oggetti.entrySet()){
            String oggetto = entry.getKey();
            Integer valore = entry.getValue();

            System.out.println(oggetto + ": " + valore);
        }
    }
}