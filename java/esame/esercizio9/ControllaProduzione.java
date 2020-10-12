public class ControllaProduzione {
    
    private int numero_semilavorati, numero_prodotti_finiti;

    public ControllaProduzione(){
        numero_semilavorati = 0;
        numero_prodotti_finiti = 0;
    }

    public synchronized void incrementaSemilavorati(){
        numero_semilavorati++;
        System.out.println("Semilavorati attuali: " + getSemilavorati());
    }

    public synchronized void incrementaFiniti(){
        numero_semilavorati--;
        numero_prodotti_finiti++;
        System.out.println("Semilavorati attuali: " + getSemilavorati() + "\tFiniti attuali: " + getFiniti());
    }

    public synchronized int getSemilavorati(){
        return numero_semilavorati;
    }

    public synchronized int getFiniti(){
        return numero_prodotti_finiti;
    }

}