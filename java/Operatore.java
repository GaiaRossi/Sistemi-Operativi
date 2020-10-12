public class Operatore implements Runnable{
    
    public void run(){

        System.out.println("Avviato operatore con id " + Thread.currentThread().getId());

        if(!MainMagazzino.m.exist("bullone")){
            MainMagazzino.m.creaOggetto("bullone");
        }
        MainMagazzino.m.addOggetto("bullone", 1000);
        MainMagazzino.m.removeOggetto("bullone", 500);      
    }
}