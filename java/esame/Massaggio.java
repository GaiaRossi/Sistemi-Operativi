import java.io.Serializable;

public class Massaggio implements Serializable{
    private String messaggio = "Fatto";
    
    public String getMessaggio(){
        return messaggio;
    }
}