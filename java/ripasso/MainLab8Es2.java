public class MainLab8Es2 {
    public static void main(String[] args) throws Exception{
        final int N = 5;
        Magazzino magazzino = new Magazzino();
        int i;
        Operatore operatore;
        Thread thread[] = new Thread[N];

        System.out.println("Avvio operatori");

        for(i = 0; i < N; i++){
            operatore = new Operatore(magazzino, "bulloni");
            thread[i] = new Thread(operatore);
            thread[i].start();
        }

        System.out.println("Operatori avviati");
        System.out.println("Attesa terminazione");

        for(i = 0; i < N; i++){
            thread[i].join();
        }

        magazzino.printMagazzino();

        System.out.println("Thread terminati");
        System.out.println("Terminazione main");
    }
}