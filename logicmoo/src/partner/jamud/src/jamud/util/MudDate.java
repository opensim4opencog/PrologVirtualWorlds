package jamud.util;


import jamud.Jamud;


public class MudDate {


    private long time;

    public long getMudTimeMillis() {
	return this.time;
    }



    public MudDate() {
	this.time = Jamud.getMudTimeMillis();
    }


    public MudDate(long time) {
	this.time = time;
    }



    

}
