public class Operatore implements Runnable{

    private Magazzino magazzino;
    private String nome_oggetto;

    public Operatore(Magazzino magazzino, String nome_oggetto){
        this.magazzino = magazzino;
        this.nome_oggetto = nome_oggetto;
    }

    private void addObject(String nome_oggetto) throws Exception{
        if(!doesObjectExists(nome_oggetto)){
            magazzino.aggiungiOggetto(nome_oggetto);
        }
    }

    private boolean doesObjectExists(String nome_oggetto){
        return magazzino.isAlreadyDefined(nome_oggetto);
    }

    private void addQuantity(String nome_oggetto, int quantity) throws Exception{
        if(doesObjectExists(nome_oggetto)){
            magazzino.aggiungiQuantitaAdOggetto(nome_oggetto, quantity);
        }
    }

    private void removeQuantita(String nome_oggetto, int quantity) throws Exception{
        if(doesObjectExists(nome_oggetto)){
            magazzino.rimuoviQuantitaAdOggetto(nome_oggetto, quantity);
        }
    }

    public void run(){
        try{
            addObject(nome_oggetto);
            addQuantity(nome_oggetto, 1000);
            removeQuantita(nome_oggetto, 500);
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }
    
}