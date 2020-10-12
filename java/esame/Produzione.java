public class Produzione{
    private int numero_prodotti;

    public Produzione(){
        numero_prodotti = 0;
    }

    public synchronized void incremento(){
        numero_prodotti++;
        this.simulate();
    }

    private synchronized void simulate(){
        if(Math.random() <= 0.25){
            try {
                Thread.sleep(200);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public synchronized void stampaProduzione(){
        System.out.println("Il numero di prodotti e': " + numero_prodotti);
    }
}