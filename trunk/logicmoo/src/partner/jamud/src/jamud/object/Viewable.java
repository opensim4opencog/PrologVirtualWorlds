package jamud.object;

import jamud.util.FlagList;

public abstract interface Viewable {

    public void setName(String name);

    public String getName();


    public void setDescription(String desc);
    
    public String getDescription();
    
    
    public void setShortDescription(String sdesc);
    
    public String getShortDescription();
    
    
    public void setKeywords(FlagList keywords);
    
    public FlagList getKeywords();

    public boolean matchKeyword(String keyword);
}
