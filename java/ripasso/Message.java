import java.io.Serializable;

public class Message implements Serializable{
    private String messaggio;

    public Message(String messaggio){
        this.messaggio = messaggio;
    }

    public String getMessaggio(){
        return messaggio;
    }
}