package logicmoo.api;

import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;


import bsh.*;

import org.opencyc.cycobject.*;


public interface IActorCommandParser  {
    public void setData(LogicMooCycAccess cm, CycFort user,CycFort loc,CycFort mt, IMooClient mc);
    public boolean enact(Object channel,Object user,Object event);
    public bsh.Interpreter getBeanShell();
    public void moveTo(CycFort cycobj,CycFort  cycwhere);
    public CycFort getUserName();
    public void setUserName(CycFort name);
    public CycFort getUserLocation();
    public void setUserLocation(CycFort loc);
    public CycFort getUserMt();
    public void setUserMt(CycFort mt);
    public String getInterp();
    public void setInterp(String interp);
}



