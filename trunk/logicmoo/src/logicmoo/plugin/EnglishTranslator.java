package logicmoo.plugin;

import logicmoo.*;
import logicmoo.api.*;
import logicmoo.obj.*;
import logicmoo.util.*;



// Java
import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;

import com.inzoom.comjni.*; 
import com.inzoom.axjni.*;
import logicmoo.nlp.ParserTypeLib.*;



// BeanShell
import bsh.*;
import bsh.util.*;

// OpenCyc
import org.opencyc.api.*;
import org.opencyc.creation.*;
import org.opencyc.cycobject.*;
import org.opencyc.kif.*;
import org.opencyc.util.*;
import org.opencyc.webserver.WebServer;
import org.apache.oro.util.*;

import java.beans.*;

// Util
import ViolinStrings.*;

public class EnglishTranslator {

    private Parser nlparser = null;

    private LogicMooCycAccess cyc = null;

    public EnglishTranslator(LogicMooCycAccess cycA) throws Exception {

	if( cycA==null ) {
	    cyc = new LogicMooCycAccess();
	} else {
	    cyc = cycA;
	}
	cyc.getCycAccess();
	startConnection();
    }

    public void startConnection() {
	pushNLSystemToProlog();
    }

    public void pushNLSystemToProlog() {

    }

    public static void main(String [] arg) {
	try {
	    Parser nlparser = new Parser();
	    System.out.println(nlparser);
	    nlparser.SetSentence("The sleepy brown fox jumped over the moon.");
	    System.out.println( nlparser.GetSentence());
	    ISegRecProperties props =  nlparser.GetChart();
	    System.out.println(props);
	    
	    for (int propnum = 1;true; propnum++) {
		com.inzoom.comjni.Variant pn = new com.inzoom.comjni.Variant(propnum);
		ISegRecProperty prop= props.GetProperty(pn.getVariant());
		System.out.println( prop.GetName() + " (" + prop.GetType() + ") = " + prop.GetValue());
	    }
	} catch( Exception e ) {
	    e.printStackTrace();
	}

    }

    public CycList toAssertion(String english, String userctx) {
	return new CycList();
    }

    public void setSwitch(String switz, String value) {
	try {
	    NLPSetting nlpset = null;
	    INLPSettingCol setcol =  nlparser.GetSettings();
	    IUnknown enum =  setcol._NewEnum();
	    System.out.println(enum);
	    com.inzoom.comjni.IEnumVARIANT enumer = com.inzoom.comjni.jcw.IEnumVARIANTJCW.getIEnumVARIANTFromUnknown(enum);
	    System.out.println(enumer);
	} catch( Exception e ) {
	    e.printStackTrace();
	}

	/*
	/Sub SetSwitch(nm$, val)
	  Dim i%, switch As NLPSetting
	  For Each switch In ParseTree.Settings
	If switch.Name = nm Then
	  switch.Value = val
	  For i = 0 To SetList.ListCount
	    If InStr(SetList.List(i), nm & " = ") = 1 Then
	      SetList.RemoveItem i
	      SetList.AddItem switch.Name & " = " & switch.Value, i
	      Exit Sub
	    End If
	  Next i
	  Call LoadSettings
	  Exit Sub
	End If
	  Next switch
	End Sub
	
	*/
    }


}

