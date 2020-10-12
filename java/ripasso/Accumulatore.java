public class Accumulatore {
    private double valore_accumulatore;

    public Accumulatore(double initial_value){
        valore_accumulatore = initial_value;
    }

    public void addValue(double valueToAdd){
        valore_accumulatore += valueToAdd;
    }

    public double getValoreAccumulatore(){
        return valore_accumulatore;
    }
}