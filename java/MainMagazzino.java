//per la versione non thread-safe vai a togliere
//i synchronized dal magazzino

public class MainMagazzino{
    static Magazzino m = new Magazzino();
    public static void main(String args[]){
        
        int n = 0, i;

        //controllo argomenti
        if(args.length == 0){
            System.err.println("Uso: java MainMagazzino <num_operatori>");
            System.exit(1);
        }
        try {
            n = Integer.parseInt(args[0]);
            if(n <= 0){
                System.err.println("Il numero degli operatori deve essere maggiore di 0");
                System.exit(1);
            }
        } catch (Exception e) {
            System.err.println("Inserire il corretto numero di operatori");
            System.exit(1);
        }

        //se sono qui ho gia' n
        Thread[] threads = new Thread[n];

        //creazione e avvio degli operatori
        for(i = 0; i < n; i++){
            threads[i] = new Thread(new Operatore());
            threads[i].start();
        }

        //attendo terminazione degli operatori
        for(i = 0; i < n; i++){
            try{
                threads[i].join();
            }
            catch(Exception e){
                e.printStackTrace();
                System.exit(2);
            }
        }

        MainMagazzino.m.print();

    }
}