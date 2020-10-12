public class MainEs7Lab12 {
    public static void main(String[] args) {
        Produzione produzione = new Produzione();
        Macchina macchina;
        Thread thread[] = new Thread[10];

        int i;
        for (i = 0; i < 10; i++) {
            macchina = new Macchina(produzione);
            thread[i] = new Thread(macchina);
            thread[i].start();
        }

        System.out.println("Thread partiti");
        System.out.println("Attesa terminazione");

        for (i = 0; i < 10; i++) {
            try {
                thread[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        System.out.println("Thread therminati");
        produzione.stampaProduzione();
    }
}