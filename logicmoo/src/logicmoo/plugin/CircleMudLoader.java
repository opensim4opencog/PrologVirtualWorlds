package logicmoo.plugin;

import logicmoo.*;
import logicmoo.api.*;
import logicmoo.util.*;


// Java
import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;


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

import ViolinStrings.Strings;


public class CircleMudLoader {
    public PrintWriter cycfile = null;
    public File loadDir = null;
    public LogicMooCycAccess cyc = null;
    public CycFort mudMt = null;

    public static void main(String arg[]) {
        int argmnt = 0;
        CircleMudLoader cml = new CircleMudLoader();
        cml.cycfile = new PrintWriter(System.out);
        
        if (arg[argmnt].equals("-o")) {
            argmnt++;
           try {
                cml.cycfile = new PrintWriter(new FileWriter(arg[argmnt++]));
           } catch ( Exception e ){
           }
        }

        if (arg[argmnt].equals("-cyc")) {
            cml.isAssertCyc = true;
            argmnt++;
            cml.mudMt = cml.cyc.makeCycConstant(arg[argmnt]);
	    argmnt++;
        }

        for ( int i=argmnt; i < arg.length; i++ ) {
            try {
                cml.loadFSO(new File(arg[i]));
            } catch ( Exception e ){
                e.printStackTrace();
            }
        }
        cml.cycfile.close();
    }


    public CircleMudLoader() {
        try {
            cyc = new LogicMooCycAccess();
            cyc.getCycAccess();
        } catch ( Exception e ) {
        }

    }

    public void loadFSO(File inputFile)  throws Exception {
        if ( inputFile.isDirectory() ) {
            loadDir(inputFile);
        } else {
            loadFile(inputFile);
        }
    }

    public void loadDir(File dir) throws Exception {
        File[] file = dir.listFiles();
        for ( int i = 0; i < file.length ; i++ ) loadFSO(file[i]);
    }

    public void loadFile(File inputFile)  throws Exception { 
        String fn = inputFile.getName();
        System.out.println("load file '" + fn + "'");

        if ( fn.endsWith(".zon") )
            loadZonFile(inputFile);
        else if ( fn.endsWith(".obj") )
            loadObjFile(inputFile);
        else if ( fn.endsWith(".mob") )
            loadMobFile(inputFile);
        else if ( fn.endsWith(".wld") )
            loadWorldFile(inputFile);
    }



    public void loadWorldFile(File file) throws Exception {
        assertData("");
        assertData(";; Sourcing " + file.getAbsoluteFile());
        BufferedReader br = new BufferedReader( new FileReader(file));
        try {
            while ( br.ready() ) parseRoom(br);
        } catch ( Exception e ){
        }
        br.close();
    }

    public static String getDirectionWards(String num) {
        return getDirection(num)+"-Generally";
    }



    public static String getDirection(String num) {
        if ( num.startsWith("D") ) num = num.substring(1);
        switch ( num.charAt(0) ) {
            case '0':
                return "North";
            case '1':
                return "East";
            case '2':
                return "South";
            case '3':
                return "West";
            case '4':
                return  "Up";
            case '5':
                return "Down";
        }
        return "North";
    }

    public static String getDirectionState(String num) {
        if ( num.startsWith("D") ) num = num.substring(1);
        switch ( num.charAt(0) ) {
            case '0':
                return "Open";
            case '1':
                return "Closed";
            case '2':
                return "Locked";
        }
        return "Open";
    }



    public void parseRoom(BufferedReader br)  throws Exception {

        String line = br.readLine(); 

        if ( line.startsWith("$") ) return;

        // Read #<virtual number>
        String vnum = getAsArea( line.substring(1).trim());
        assertData("");
        assertData("(isa " + vnum + " GeographicalRegion)");
        // Read <room name>~
        String roomname = readUpTilde(br);
        if ( roomname.length()>0 ) {
            assertData("(prettyName " + vnum + " " + quotedString(roomname) + ")");
            //assertData("(properNameStrings " + vnum + " " + quotedString(roomname) + ")");
        }
        // Read <Room Description>~
        String desc = readUpTilde(br);
        if ( desc.length()>0 ) assertData("(mudDescription " + vnum + " " + quotedString(desc) + ")");
        // Read <zone number> <room bitvector> <sector type>
        String[] splitter = br.readLine().trim().split(" ");
        if ( splitter.length==3 ) {
            getBitvectors(vnum,splitter[1]);
            getBittypes(vnum,splitter[2]);
        } else {
            System.out.println(vnum + " error " + splitter);
        }

        // Read {Zero or more Direction Fields and/or Extra Descriptions}
        while ( !((line=br.readLine()).startsWith("S")) ) {
            if ( line.startsWith("E") ) {
                // Read <keyword list>~
                splitter = readUpTilde(br).split(" ");
                // Read <description text>~   
                desc = readUpTilde(br);
                for ( int i =0 ; i < splitter.length;i++ )
                    //(thereExists ?Obj (and (in-ContGeneric ?Obj ?VNUM)()))
                    assertData("(mudPossiblyAlreadyInRoom " + vnum + " " + quotedString(splitter[i]) + " " + quotedString(desc) + " )");

            } else {

                // Read D<direction number>
                String direction = getDirectionWards(line);
                // Read <general description>~
                desc = readUpTilde(br);
                // Read <keyword list>~
                String[] kws = readUpTilde(br).split(" ");
                // Read <doorType flag> <key number> <room linked>
                splitter = readUpLine(br).split(" ");
                String doorType = "OpeningPortal";

                switch ( splitter[0].charAt(0) ) {
                    case '0':
                        doorType = "OpeningPortal";
                        break;
                    case '1':
                        doorType = "Doorway";
                        break;
                    case '2':
                        doorType = "SecurityDoorPortal";
                        break;
                }
                String lock = null;
                if ( !splitter[1].equals("-1") ) lock = "ArtifactCol"+splitter[1];

                String destination = getAsArea(splitter[2]);
                if ( splitter[2]=="-1" ) destination =  vnum;

                String exitToFn = "(PortalFromToFn " + vnum + " " + direction + " " + destination + " )"; 
                assertData("(isa " + exitToFn + " CycLNonAtomicReifiedTerm )");
                assertData("(isa " + exitToFn + " " + doorType + " )");
                assertData("(containsPortals " + vnum + " " + exitToFn +  " )");


                if ( desc.length()>0 )
                    assertData("(mudDescription " + exitToFn + " " + quotedString(desc)  + " )");

                if ( lock!=null ) assertData("(controls " + lock + " " + exitToFn + " )");

                for ( int i =0 ; i < kws.length;i++ ) {
                    try {
                        if ( kws[i].length()>0 ) assertData("(mudKeyword " + exitToFn + " " + quotedString(kws[i]) + " )");
                    } catch ( Exception e ){
                        e.printStackTrace();
                    }
                }

            }
        }
    }

    public boolean isAssertCyc = false;

    public void assertData(String form) {
        cycfile.println(form);
        form = form.trim();
        if ( form.length()<1 ) return;
        if ( form.startsWith(";") ) return;
        //System.out.println(form);
        if ( !isAssertCyc ) return;

        try {
            cyc.converseVoid(
                                  "(clet ((*the-cyclist* #$CycAdministrator))\n" +
                                  "   (without-wff-semantics (cyc-assert\n" +
                                  "    '" + cyc.toCycList(form).cyclify() + "\n" +
                                  "    " + mudMt.cyclify() + " '(:DIRECTION :FORWARD :STRENGTH :MONOTONIC) )))");

        } catch ( Exception e ) {
            e.printStackTrace();
            System.out.println(form);
        }
    }

    public String getAsArea(String vnum) {
        String result = vnum.trim();
        if ( !result.startsWith("Area") ) {
            result = "Area" + result;
        }
        return result;
    }

    public void getBitvectors(String vnum,String flags)  throws Exception  {
        try {
            int bits = Integer.parseInt(flags);
            if ( bits>0 ) {
                for ( int i = 0; i<16;i++ ) {
                    getBitvector(vnum,(new Double(Math.pow(2,i)).intValue()) & bits);
                }
            }
        } catch ( Exception e ){
            char[] ca = flags.trim().toLowerCase().toCharArray();
            for ( int i = 0; i<ca.length;i++ ) {
                getBitvector(vnum,ca[i]);
            }
        }

    }


    public void getBitvector(String vnum,int alphanum)  throws Exception  {
        switch ( alphanum ) {
            case 0:
                break;
            case 1:
            case 'a':
                assertData("(hasAttributes "+vnum+ " DarkMudAreaAttribute)");
                break;
            case 2:
            case 'b':
                assertData("(hasAttributes "+vnum+ " DeathMudAreaAttribute)");
                break;
            case 4:
            case 'c':
                assertData("(hasAttributes "+vnum+ " NoMobMudAreaAttribute)");
                break;
            case 8:
            case 'd':
                assertData("(isa "+vnum+ " InsideSurface)");
                break;
            case 16:
            case 'e':
                assertData("(hasAttributes "+vnum+ " PeacefullMudAreaAttribute)");
                break;
            case 32:
            case 'f':
                assertData("(hasAttributes "+vnum+ " SoundProofMudAreaAttribute)");
                break;
            case 64:
            case 'g':
                assertData("(hasAttributes "+vnum+ " NoTrackMudAreaAttribute)");
                break;
            case 128:
            case 'h':
                assertData("(hasAttributes "+vnum+ " NoMagicMudAreaAttribute)");
                break;
            case 256:
            case 'i':
                assertData("(isa "+vnum+ " CavityInteriorRegion)");
                break;
            case 512:
            case 'j':
                assertData("(hasAttributes "+vnum+ " PrivateMudAreaAttribute)");
                break;
            case 1024:
            case 'k':
                assertData("(hasAttributes "+vnum+ " GodOnlyMudAreaAttribute)");
                break;
            case 2048:
            case 'l':
                assertData("(isa "+vnum+ " ModernShelterConstruction)");
                break;
            case 4096:
            case 'm':
                assertData("(isa "+vnum+ " SpaceInAHOC)");                
                assertData("(hasAttributes "+vnum+ " HouseCrashMudAreaAttribute)");
                break;
            case 8192:
            case 'n':
                assertData("(isa "+vnum+ " Doorway)");
                assertData("(isa "+vnum+ " SpaceInAHOC)");                
                break;
            case 16384:
            case 'o':
                assertData("(hasAttributes "+vnum+ " OLCMudAreaAttribute)");
                break;
            case 32768:
            case 'p':
                assertData("(hasAttributes "+vnum+ " BFSMarkMudAreaAttribute)");
                break;
        }
    }

    public void getBittypes(String vnum,String flags) throws Exception {
        int flag = Integer.parseInt(flags);
        switch ( flag ) {
            case 0:
                assertData("(isa "+vnum+ " SpaceInAHOC)");
                break;
            case 1:
                assertData("(isa "+vnum+ " Street-Generic)");
                assertData("(isa "+vnum+ " OutdoorLocation)");
                break;
            case 2:
                assertData("(isa "+vnum+ " Plain-Topographical)");
                assertData("(isa "+vnum+ " OutdoorLocation)");
                break;
            case 3:
                assertData("(isa "+vnum+ " Forest)");
                assertData("(isa "+vnum+ " OutdoorLocation)");
                assertData("(isa "+vnum+ " EcologicalRegion)");
                break;
            case 4:
                assertData("(isa "+vnum+ " Plain-Topographical)");
                assertData("(isa "+vnum+ " EcologicalRegion)");
                assertData("(isa "+vnum+ " OutdoorLocation)");
                assertData("(isa "+vnum+ " MountainRange)");
                break;
            case 5:
                assertData("(isa "+vnum+ " EcologicalRegion)");
                assertData("(isa "+vnum+ " OutdoorLocation)");
                assertData("(isa "+vnum+ " MountainRange)");
                break;
            case 6:
            case 7:
            case 8:
                assertData("(isa "+vnum+ " BodyOfWater)");
                assertData("(isa "+vnum+ " EcologicalRegion)");
                if ( flag==7 ) {
                    assertData("(not (hasAttributes "+vnum+ " SwimmingAreaAttribute))");
                } else {
                    // 6,8
                    assertData("(hasAttributes "+vnum+ " SwimmingAreaAttribute)");   
                    if ( flag==8 ) {
                        assertData("(isa "+vnum+ " UnderwaterLocation)");
                    }
                }
                break;
            case 9:
                assertData("(isa "+vnum+ " FreeSpaceContent)");
                assertData("(hasAttributes "+vnum+ " FlyingAreaAttribute)");   
                break;
            default:
                break;
        }
    }

    public static String quotedString(String stuff) {

        return "\""+Strings.change(stuff,"\"","'")+"\"";
    }

    public static String readUpLine(BufferedReader br) {
        try {
            return br.readLine().trim();
        } catch ( Exception e ) {
            return " " + e;
        }
    }

    public static String readUpTilde(BufferedReader br) throws Exception {
        String line = br.readLine().trim();
        if ( line.endsWith("~") ) {
            if ( line.length()==1 ) return "";
            return line.substring(0,line.length()-1);
        }
        StringBuffer desc = new StringBuffer(line);
        while ( !(line =br.readLine().trim()).endsWith("~") ) {
            desc.append(" <br>" + line);
        }
        line = desc.toString();
        if ( line.endsWith("~") ) {
            return line.substring(0,line.length()-1);
        } else return line;
    }

    public void loadObjFile(File file) throws Exception{
        BufferedReader br = new BufferedReader( new FileReader(file));
        while ( br.ready() ) {
            try {
                parseObjects(br);
            } catch ( Exception e ) {
            }
        }
        br.close();
    }


    public void parseObjects(BufferedReader br) throws Exception {

        String line = br.readLine();
        if ( line.startsWith("$") ) return;

        // Read #<virtual number>
        parseObject(getAsArtifactCollection(line.substring(1).trim()),  br) ;

    }

    public String getAsArtifactCollection(String vnum) {
        String result = vnum.trim();
        if ( !result.startsWith("ArtifactCol") ) {
            result = "ArtifactCol" + result;
        }
        return result;
    }

    public void assertDataX(String vnum,String formula) {
        assertData("(implies (isa ?X " + vnum +  ") " + formula + " )");
    }

    public void  parseObject(String vnum, BufferedReader br) throws Exception {
        String line = null;
        assertData("");
        assertData("(isa " + vnum + " ArtifactTypeCollection)");
        // Read <alias list>~
        String[] kws = readUpTilde(br).split(" ");
        for ( int i = 0 ; i < kws.length; i++ ) {
            assertDataX(vnum,"(mudKeyword ?X " + quotedString(kws[i]) + ")");
        }
        // Read <short description>~
        String shortDesc = readUpTilde(br);
        if ( shortDesc.length()>0 ) assertDataX(vnum,"(prettyName ?X " + quotedString(shortDesc) + ")");
        // Read <long description>~
        String longDesc = readUpTilde(br);
        if ( longDesc.length()>0 ) assertDataX(vnum,"(mudAreaView ?X " + quotedString(longDesc) + ")");
        // Read <action description>~
        String actionDesc = readUpTilde(br);
        if ( actionDesc.length()>0 ) assertDataX(vnum,"(mudDescription ?X " + quotedString(actionDesc) + ")");
        // Read <type flag> <extra (effects) bitvector> <wear bitvector>
        String[] flags = readUpLine(br).split(" ");
        String[] values = readUpLine(br).split(" ");
        parseObjectType(vnum,Integer.parseInt(flags[0]),values);
        parseObjectAffectsWear(vnum,flags);
        // Read <weight> <cost> <rent per day>
        String[] wcr = readUpLine(br).split(" ");
        assertDataX(vnum,"(massOfObject ?X (Kilogram " + wcr[0] + "))");
        if ( !wcr[1].equals("0") ) assertDataX(vnum,"(cost ?X (Dollar-UnitedStates " + wcr[1] + "))");
        if ( !wcr[2].equals("0") ) assertDataX(vnum,"(cost ?X (DollarsPerHour " + wcr[2] + " ))");

        try {
            while ( !(line =  readUpLine(br).trim()).startsWith("$") ) {
                if ( line.startsWith("#") ) {
                    vnum = "ArtifactCol" + line.substring(1).trim();
                    parseObject(vnum, br);
                    return;
                } else if ( line.startsWith("E") ) {
                    //Read <keyword list>~
                    String pkws[] = readUpTilde(br).split(" ");
                    //Read <description text>~
                    String kwDesc = readUpTilde(br);
                    for ( int i = 0; i < pkws.length; i++ ) {
                        assertData("(implies (isa ?X " + vnum + ")(thereExists ?Obj (and (parts ?X ?Obj)(mudDescription ?Obj "+ quotedString(kwDesc) +")(mudKeyword ?Obj "+  quotedString(pkws[i]) +" ))))");
                    }
                }
            }
        } catch ( Exception e ) {
        }
    }


    public void parseObjectType(String vnum, int flag0, String[]values ) {
        switch ( flag0 ) {
            case 0:
                break;
            case 1:
                assertData("(genls "+vnum+ " LightingDevice)");
                assertDataX(vnum,"(duration ?X "+values[2]+")");
                break;
            case 2:
                // Scroll
            case 10:
                // Potion
                if ( flag0 == 2 ) {
                    assertData("(genls "+vnum+ "  InformationBearingObject)");
                    assertData("(genls "+vnum+ "  Paper)");
                } else {
                    assertData("(genls "+vnum+ " DrugSubstance)");
                }
                assertDataX(vnum,"(mudLevelOf ?X "+values[0] + ")");
                if ( !values[1].equals("0") ) assertDataX(vnum,"(intangibleParts ?X MudSpell"+values[1] + ")");
                if ( !values[2].equals("0") ) assertDataX(vnum,"(intangibleParts ?X MudSpell"+values[2] + ")");
                if ( !values[3].equals("0") ) assertDataX(vnum,"(intangibleParts ?X MudSpell"+values[3] + ")");
                break;
            case 3:
                //Wand 
            case 4:
                //Staff 
                assertData("(genls "+vnum+ " RodShape)");
                assertData("(genls "+vnum+ " ControlDevice)");
                if ( flag0==4 ) assertData("(genls "+vnum+ " Weapon)");
                assertDataX(vnum,"(mudLevelOf ?X "+values[0] + ")");
                assertDataX(vnum,"(chargeCapacity ?X "+values[1] + ")");
                assertDataX(vnum,"(chargeRemaining ?X "+values[2] + ")");
                if ( !values[3].equals("0") ) assertDataX(vnum,"(intangibleParts ?X MudSpell"+values[3] + ")");
                break;
            case 5:
                // Weapon
            case 6:
                // FireWeapon
            case 7:
                // MissileWeapon
                assertData("(genls "+vnum+ " Weapon)");

                if ( flag0==6 ) {
                    assertData("(genls "+vnum+ " MudFireWeapon)");
                    assertData("(genls "+vnum+ " ProjectileLauncher)");
                }
                if ( flag0==7 ) {
                    assertData("(genls "+vnum+ " MudMissileWeapon)");
                    assertData("(genls "+vnum+ " ProjectileLauncher)");
                }
                assertData("(damageNumberDice "+vnum+ " "+values[1] + ")");
                assertData("(damageSizeDice "+vnum+ " "+values[2] + ")");
                parseObjectWeaponMsg(vnum, Integer.parseInt( values[3]));
                break;
            case 8:
                // TreasureObject
                //assertData("(isa "+vnum+ " TreasureObject)");
                assertData("(genls "+vnum+ " TenderObject)");
                break;
            case 9:
                // Armor
                assertData("(genls "+vnum+ " MudArmor)");
                assertData("(armorLevel "+vnum+ " "+values[0] + ")");
                break;
            case 11:
                // SomethingToWear
                assertData("(genls "+vnum+ " SomethingToWear)");
                break;
            case 12:
                // Unkonwn
                assertData("(genls "+vnum+ " Artifact-Generic)");
                break;
            case 13:
                // Garbage
                assertData("(genls "+vnum+ " Artifact-Generic)");
                assertDataX(vnum,"(cost ?X (Dollar-UnitedStates 0))");
                break;
            case 14:
                // Trap
                assertData("(genls "+vnum+ " Artifact-Generic)");
                assertData("(hasAttributes "+vnum+ " MudTrap)");
                break;
            case 15:
                // Container
                assertData("(genls "+vnum+ " Container)");
                assertDataX(vnum,"(volumeContained ?X "+values[0] + ")");
                if ( Integer.parseInt( values[2])>0 ) 
                    assertDataX(vnum,"(containerKey ?X ArtifactCol"+values[2] + ")");
                break;
            case 16:
                // Note
                assertData("(genls "+vnum+ "  InformationBearingObject)");
                assertData("(genls "+vnum+ " Paper)");
                assertDataX(vnum,"(nativeLanguage ?X "+values[0] + ")");
                break;
            case 17:
                // Flask
            case 23:
                // Founter
                assertData("(genls "+vnum+ " FluidReservoir)");
                assertData("(genls "+vnum+ " Container)");
                if ( flag0==23 ) assertData("(genls "+vnum+ " BodyOfWater)");
                assertDataX(vnum,"(volumeOfObject ?X (Liter  "+values[0] + "))");
                assertDataX(vnum,"(volumeContained ?X (Liter  "+values[0] + "))");
                parseContainerState(vnum, Integer.parseInt( values[1] ));
                if ( Integer.parseInt(values[3])>0 ) assertData("(hasAttributes "+vnum+ " ContainerPosionedAttribute)");
                String contents =  parseFluid(Integer.parseInt(values[2])); 
                assertData("(genls " + contents + " LiquidTangibleThing)");
                assertData("(quantityInContainer "+vnum+ " " +contents + " " + values[1] + ")");
                break;
            case 18:
                // KEY
                assertData("(genls "+vnum+ " ControlDevice)");
                break;
            case 19:
                // FOOD
                assertData("(genls "+vnum+ " Food)");
                break;
            case 20:
                // GOLD
                assertData("(genls "+vnum+ " Currency)");
                break;
            case 21:
                // PENCIL
                assertData("(genls "+vnum+ " WritingDevice)");
                break;
            case 22:
                // BOAT
                assertData("(genls "+vnum+ " Watercraft)");
                break;
            default:
                break;
        }
    }


    public void parseContainerState(String vnum, int bit) {
        switch ( bit ) {
            case 1:
                assertData("(genls "+vnum+ " ContainerCloseable)");
                break;
            case 2:
                assertData("(genls "+vnum+ " ContainerPickproof)");
                break;
            case 4:
                assertData("(genls "+vnum+ " ContainerClosed)");
                break;
            case 8:
                assertData("(genls "+vnum+ " ContainerLocked)");
                break;
        }
    }

    public String parseFluid(int bit) {
        String  contents = "Oil";
        switch ( bit ) {
            case 0:
                contents = "Water";
                break;
            case 1:
                contents = "Beer";
                break;
            case 2:
                contents = "Wine";
                break;
            case 3:
                contents = "Ale";
                break;
            case 4:
                contents = "DarkAle";
                break;
            case 5:
                contents = "Whiskey";
                break;
            case 6:
                contents = "Lemonade";
                break;
            case 7:
                contents = "FireBrt";
                break;
            case 8:
                contents = "LocalSpc";
                break;
            case 9:
                contents = "Slime";
                break;
            case 10:
                contents = "Milk";
                break;
            case 11:
                contents = "Tea";
                break;
            case 12:
                contents = "Coffee";
                break;
            case 13:
                contents = "Blood";
                break;
            case 14:
                contents = "SaltWater";
                break;
            case 15:
                contents = "ClearWater";
                break;
        }
        return "Fluid"+contents;
    }

    public void parseObjectAffectsWear(String vnum, String[] flags) {
        try {
            int bits = Integer.parseInt(flags[1]);
            if ( bits>0 ) {
                for ( int i = 0; i<9;i++ ) {
                    parseObjectAffectBit1(vnum,(new Double(Math.pow(2,i)).intValue()) & bits);
                }
            }
        } catch ( Exception e ){
            char[] ca = flags[1].trim().toLowerCase().toCharArray();
            for ( int i = 0; i<ca.length;i++ ) {
                parseObjectAffectBit1(vnum,ca[i]);
            }
        }

        try {
            int bits = Integer.parseInt(flags[2]);
            if ( bits>0 ) {
                for ( int i = 0; i<14;i++ ) {
                    parseObjectAffectBit2(vnum,(new Double(Math.pow(2,i)).intValue()) & bits);
                }
            }
        } catch ( Exception e ){
            char[] ca = flags[2].trim().toLowerCase().toCharArray();
            for ( int i = 0; i<ca.length; i++ ) {
                parseObjectAffectBit2(vnum,ca[i]);
            }
        }

    }

    public void parseObjectAffectBit1(String vnum, int bit) {
        switch ( bit ) {
            case 0:
                break;
            case 1:
            case 'a':
                assertData("(hasAttributes "+vnum+ " MudGlow)");
                assertData("(genls "+vnum+ " LightingDevice)");
                break;
            case 2:
            case 'b':
                assertData("(hasAttributes "+vnum+ " MudHum)");
                assertData("(genls "+vnum+ " InformationStore)");
                break;
            case 4:
            case 'c':
                assertData("(hasAttributes "+vnum+ " MudNoRent)");
                break;
            case 8:
            case 'd':
                assertData("(hasAttributes "+vnum+ " MudNoDonate)");
                break;
            case 16:
            case 'e':
                assertData("(hasAttributes "+vnum+ " MudNoInvisable)");
                break;
            case 32:
            case 'f':
                assertData("(hasAttributes "+vnum+ " MudInvisable)");
                break;
            case 64:
            case 'g':
                assertData("(hasAttributes "+vnum+ " MudMagic)");
                break;
            case 128:
            case 'h':
                assertData("(hasAttributes "+vnum+ " MudNoDrop)");
                break;
            case 256:
            case 'i':
                assertData("(hasAttributes "+vnum+ " MudBlessed)");
                break;
            default:
                break;
        }

    }

    public void  parseObjectAffectBit2(String vnum, int bit) {
        switch ( bit ) {
            case 0:
                break;
            case 1:
            case 'a':
                assertData("(genls "+vnum+ " PortableObject)");
                break;
            case 2:
            case 'b':
                assertDataX(vnum,"(wornOn ?X  Finger)");
                assertData("(genls "+vnum+ " SomethingToWear)");
                break;
            case 4:
            case 'c':
                assertDataX(vnum,"(wornOn ?X Neck)");
                assertData("(genls "+vnum+ " SomethingToWear)");
                break;
            case 8:
            case 'd':
                assertDataX(vnum,"(wornOn ?X Torso)");
                assertData("(genls "+vnum+ " SomethingToWear)");
                break;
            case 16:
            case 'e':
                assertDataX(vnum,"(wornOn ?X  Head-AnimalBodyPart)");
                assertData("(genls "+vnum+ " SomethingToWear)");
                break;
            case 32:
            case 'f':
                assertDataX(vnum,"(wornOn ?X  Leg)");
                assertData("(genls "+vnum+ " SomethingToWear)");
                break;
            case 64:
            case 'g':
                assertDataX(vnum,"(wornOn ?X  Foot-AnimalBodyPart)");
                assertData("(genls "+vnum+ " SomethingToWear)");
                break;
            case 128:
            case 'h':
                assertDataX(vnum,"(wornOn ?X  Hand)");
                assertData("(genls "+vnum+ " SomethingToWear)");
                break;
            case 256:
            case 'i':
                assertDataX(vnum,"(wornOn ?X  Arm)");
                assertData("(genls "+vnum+ " SomethingToWear)");
                break;
            case 512:
            case 'j':
                assertData("(genls "+vnum+ " MudSheildWear)");
                assertData("(genls "+vnum+ " SomethingToWear)");
                break;
            case 1024:
            case 'k':
                assertDataX(vnum,"(wornOn ?X Trunk-BodyCore)");
                assertData("(genls "+vnum+ " SomethingToWear)");
                break;
            case 2048:
            case 'l':
                assertData("(genls "+vnum+ " MudArroundWaistWear)");
                assertData("(genls "+vnum+ " SomethingToWear)");
                break;
            case 4096:
            case 'm':
                assertData("(genls "+vnum+ " MudArroundWristWear)");
                assertData("(genls "+vnum+ " SomethingToWear)");
                break;
            case 8192:
            case 'n':
                assertData("(genls "+vnum+ " MudWeildWear)");
                assertData("(genls "+vnum+ " SomethingToWear)");
                break;
            case 16384:
            case 'o':
                assertData("(genls "+vnum+ " MudHoldWear)");
                assertData("(genls "+vnum+ " PortableObject)");
                break;
            default:
                break;
        }
    }




    public void  parseObjectWeaponMsg(String vnum, int bit) {
        switch ( bit ) {
            case 0:
                assertData("(hasAttributes "+vnum+ " WeaponHitting)");
                break;
            case 1:
                assertData("(hasAttributes "+vnum+ " WeaponStinging)");
                break;
            case 2:
                assertData("(hasAttributes "+vnum+ " WeaponWhipping)");
                break;
            case 3:
                assertData("(hasAttributes "+vnum+ " WeaponSlashing)");
                break;
            case 4:
                assertData("(hasAttributes "+vnum+ " WeaponBiting)");
                break;
            case 5:
                assertData("(hasAttributes "+vnum+ " WeaponBludgeon)");
                break;
            case 6:
                assertData("(hasAttributes "+vnum+ " WeaponCrushing)");
                break;
            case 7:
                assertData("(hasAttributes "+vnum+ " WeaponPounding)");
                break;
            case 8:
                assertData("(hasAttributes "+vnum+ " WeaponClawing)");
                break;
            case 9:
                assertData("(hasAttributes "+vnum+ " WeaponMauling)");
                break;
            case 10:
                assertData("(hasAttributes "+vnum+ " WeaponThrashing)");
                break;
            case 11:
                assertData("(hasAttributes "+vnum+ " WeaponPeircing)");
                break;
            case 12:
                assertData("(hasAttributes "+vnum+ " WeaponBlasting)");
                break;
            case 13:
                assertData("(hasAttributes "+vnum+ " WeaponPunching)");
                break;
            case 14:
                assertData("(hasAttributes "+vnum+ " WeaponStabing)");
                break;
        }

    }


    public void loadMobFile(File file) throws Exception {
        assertData("");
        assertData(";; Sourcing " + file.getAbsoluteFile());
        BufferedReader br = new BufferedReader( new FileReader(file));
        try {
            while ( br.ready() ) parseMobs(br);
        } catch ( Exception e ){
        }
        br.close();
    }



    public void parseMobs(BufferedReader br) throws Exception {

        String line = br.readLine();
        if ( line.startsWith("$") ) return;

        // Read #<virtual number>
        String vnum = "NpcCol" + line.substring(1).trim();
        parseMob( vnum,  br) ;

    }
    public void  parseMob(String vnum, BufferedReader br) throws Exception {
        String line = null;
        assertData("");
        assertData("(isa " + vnum + " NpcAgentCollection)");
        assertData("(genls " + vnum + " Agent)");
        // Read <alias list>~
        String[] kws = readUpTilde(br).split(" ");
        for ( int i = 0 ; i < kws.length; i++ ) {
            assertDataX(vnum,"(mudKeyword ?X " + quotedString(kws[i]) + ")");
        }
        // Read <short description>~
        String shortDesc = readUpTilde(br);
        if ( shortDesc.length()>0 ) assertDataX(vnum,"(prettyName ?X " + quotedString(shortDesc) + ")");
        // Read <long description>~
        String longDesc = readUpTilde(br);
        if ( longDesc.length()>0 ) assertDataX(vnum,"(mudAreaView ?X " + quotedString(longDesc) + ")");
        // Read <detailed description>~
        String actionDesc = readUpTilde(br);
        if ( actionDesc.length()>0 ) assertDataX(vnum,"(mudDescription ?X " + quotedString(actionDesc) + ")");
        // Read <action bitvector> <affection bitvector> <alignment> <type flag>
        String[] flags = readUpLine(br).split(" ");
        try {
            int bits = Integer.parseInt(flags[0]);
            if ( bits>0 ) {
                for ( int i = 0; i<17;i++ ) {
                    parseMobBitV0(vnum,(new Double(Math.pow(2,i)).intValue()) & bits);
                }
            }
        } catch ( Exception e ){
            char[] ca = flags[0].trim().toLowerCase().toCharArray();
            for ( int i = 0; i<ca.length;i++ ) {
                parseMobBitV0(vnum,ca[i]);
            }
        }
        try {
            int bits = Integer.parseInt(flags[1]);
            if ( bits>0 ) {
                for ( int i = 0; i<17;i++ ) {
                    parseMobBitV1(vnum,(new Double(Math.pow(2,i)).intValue()) & bits);
                }
            }
        } catch ( Exception e ){
            char[] ca = flags[1].trim().toLowerCase().toCharArray();
            for ( int i = 0; i<ca.length;i++ ) {
                parseMobBitV1(vnum,ca[i]);
            }
        }

        // Alignment flags[2]
        assertDataX(vnum,"(alignmentOfNPC ?X "+Integer.parseInt(flags[2])+")");

        // Read <level> <thac0> <armor class> <max hit points> <bare hand damage>
        String[] ltamb = readUpLine(br).split(" ");
        assertDataX(vnum,"(mudLevelOf ?X "+ltamb[0]+")");
        assertData("(mudToHitArmorClass0 " + vnum + " "+ltamb[1]+")");
        assertData("(mudArmorClass " + vnum + " "+ltamb[2]+")");
        assertData("(mudMaxHitPoints " + vnum + " "+quotedString(ltamb[3])+")");
        assertData("(mudBareHandDamage " + vnum + " "+ quotedString( ltamb[4])+")");
        // Read <gold> <experience points>
        String[] ge = readUpLine(br).split(" ");
        assertDataX(vnum,"(mudGold ?X "+ge[0]+")");
        assertDataX(vnum,"(mudExperience ?X "+ge[1]+")");
        // Read <load position> <default position> <sex>
        String[] lps = readUpLine(br).split(" ");
        assertDataX(vnum,"(mudPosition ?X Posture"+ getPosture(lps[0])+")");
        assertData("(mudPositionFavored " + vnum + " Posture"+getPosture(lps[1])+")");
        assertData("(genls " + vnum + " " + getGender(lps[2])+")");
        // Type flags[3]
        if ( flags[3].startsWith("S") ) return;
        while ( !(line =  readUpLine(br).trim()).startsWith("$") ){
            if ( line.startsWith("E") ) return;
        }
    }

    public String getGender(String flag) {
        switch ( flag.charAt(0) ) {
            case '0':
                return "NeuterAnimal";
            case '1':
                return "MaleAnimal";
            case '2':
                return "FemaleAnimal";
        }
        return "NeuterAnimal";
    }

    public String getPosture(String flag) {
        switch ( flag.charAt(0) ) {
            case '0':
                return "Dead";
            case '1':
                return "Wounded";
            case '2':
                return "Incapictated";
            case '3':
                return "Stunned";
            case '4':
                return "Sleeping";
            case '5':
                return "Resting";
            case '6':
                return "Sitting";
            case '7':
                return "Fighting";
            case '8':
                return "Standing";
        }
        return "Standing";
    }


    public void parseMobBitV0(String vnum, int flag){
        switch ( flag ) {
            case 0:
                break;
            case 1:
            case 'a':
                assertData("(hasAttributes "+vnum+ " NPCCodedExternally)");
                break;
            case 2:
            case 'b':
                assertData("(hasAttributes "+vnum+ " NPCNeverWanders)");
                break;
            case 4:
            case 'c':
                assertData("(hasAttributes "+vnum+ " NPCScavenger)");
                break;
            case 8:
            case 'd':
                assertData("(hasAttributes "+vnum+ " NPCInternal)");
                break;
            case 16:
            case 'e':
                assertData("(hasAttributes "+vnum+ " NPCNoBackstab)");
                break;
            case 32:
            case 'f':
                assertData("(hasAttributes "+vnum+ " NPCAgressive)");
                break;
            case 64:
            case 'g':
                assertData("(hasAttributes "+vnum+ " NPCZoned)");
                break;
            case 128:
            case 'h':
                assertData("(hasAttributes "+vnum+ " NPCWimpy)");
                break;
            case 256:
            case 'i':
                assertData("(hasAttributes "+vnum+ " NPCAgroEvil)");
                break;
            case 512:
            case 'j':
                assertData("(hasAttributes "+vnum+ " NPCAgroGood)");
                break;
            case 1024:
            case 'k':
                assertData("(hasAttributes "+vnum+ " NPCAgroNeutral)");
                break;
            case 2048:
            case 'l':
                assertData("(implies (isa ?X "+vnum+ ")(isa ?X IntelligentAgent))");
                break;
            case 4096:
            case 'm':
                assertData("(hasAttributes "+vnum+ " NPCHelper)");
                break;
            case 8192:
            case 'n':
                assertData("(hasAttributes "+vnum+ " NPCNoCharmed)");
                break;
            case 16384:
            case 'o':
                assertData("(hasAttributes "+vnum+ " NPCNoSummoned)");
                break;
            case 32768:
            case 'p':
                assertData("(hasAttributes "+vnum+ " NPCNoSleep)");
                break;
            case 65536:
            case 'q':
                assertData("(hasAttributes "+vnum+ " NPCNoBash)");
                break;
            case 131072:
            case 'r':
                assertData("(hasAttributes "+vnum+ " NPCNoBlind)");
                break;
            default:
                break;
        }
    }

    public void parseMobBitV1(String vnum, int flag){
        switch ( flag ) {
            case 0:
                break;
            case 1:
            case 'a':
                assertData("hasAttributes",vnum,"Npc","BLIND","Mob is blind.");
                break;
            case 2:
            case 'b':
                assertData("hasAttributes",vnum,"Npc","INVISIBLE","Mob is invisible.");
                break;
            case 4:
            case 'c':
                assertData("(hasAttributes "+vnum+ " NPC_DETECT_ALIGN)");
                break;
            case 8:
            case 'd':
                assertData("(hasAttributes "+vnum+ " NPC_DETECT_INVIS)");
                break;
            case 16:
            case 'e':
                assertData("(hasAttributes "+vnum+ " NPC_DETECT_MAGIC)");
                break;
            case 32:
            case 'f':
                assertData("(hasAttributes "+vnum+ " NPC_SENSE_LIFE)");
                break;
            case 64:
            case 'g':
//                assertData("(behaviorCapable "+vnum+ " NPC_WATERWALK)");
//                assertData("(typeBehaviorCapable "+vnum+ " ?Role ?Event))");
                assertData("(hasAttributes "+vnum+ " NPC_WATERWALK)");
                break;
            case 128:
            case 'h':
                assertData("(hasAttributes "+vnum+ " NPC_SANCTUARY)");
                break;
            case 256:
            case 'i':
                assertData("(genls "+vnum+ " (GroupFn Agent-Generic))");
                break;
            case 512:
            case 'j':
                assertData("(hasAttributes "+vnum+ " NPC_CURSE)");
                break;
            case 1024:
            case 'k':
                assertData("(hasAttributes "+vnum+ " NPC_INFRAVISION)");
                break;
            case 2048:
            case 'l':
                assertData("(hasAttributes "+vnum+ " NPC_POISON)");
                break;
            case 4096:
            case 'm':
                assertData("(hasAttributes "+vnum+ " NPC_PROTECT_EVIL)");
                break;
            case 8192:
            case 'n':
                assertData("(hasAttributes "+vnum+ " NPC_PROTECT_GOOD)");
                break;
            case 16384:
            case 'o':
                assertData("(hasAttributes "+vnum+ " NPC_SLEEP)");
                break;
            case 32768:
            case 'p':
                assertData("(hasAttributes "+vnum+ " NPC_NOTRACK)");
                break;
            case 65536:
            case 'q':
                assertData("(hasAttributes "+vnum+ " NPC_UNUSED16)");
                break;
            case 131072:
            case 'r':
                assertData("(hasAttributes "+vnum+ " NPC_UNUSED17)");
                break;
            case 262144:
            case 's':
                assertData("(hasAttributes "+vnum+ " NPC_SNEAK)");
                break;
            case 524288:
            case 't':
                assertData("(hasAttributes "+vnum+ " NPC_HIDE)");
                break;
            case 1048576:
            case 'u':
                assertData("(hasAttributes "+vnum+ " NPC_UNUSED20)");
                break;
            case 2097152:
            case 'v':
                assertData("(hasAttributes "+vnum+ " NPC_CHARM)");
                break;
            default:
                break;
        }
    }

    public void assertData(String pred,String vnum,String classed,String value,String Desc)  {
        assertData("(" + pred + " " + vnum + " " + classed + "_" + value + ")");
    }

    public void loadZonFile(File file) throws Exception {
        assertData("");
        assertData(";; Sourcing " + file.getAbsoluteFile());
        BufferedReader br = new BufferedReader( new FileReader(file));
        try {
            while ( br.ready() ) parseZone(br);
        } catch ( Exception e ){
        }
        br.close();
    }



    public void parseZone(BufferedReader br) throws Exception {
        // Read #<virtual number>
        String line = br.readLine();
        if ( line.startsWith("$") ) return;
        // Read <zone name>~
        readUpTilde(br);
        // Read <top room number> <lifespan> <reset mode>
        String timer = readUpLine(br).split("0")[1];
        parseZoneParts(timer,br) ;
    }
    public void parseZoneParts(String timer, BufferedReader br) throws Exception {
        String line = null;
        String npc = null;
        while ( !(line =  readUpLine(br).trim()).startsWith("$") ){
            String[] arg = line.split(" ");
            switch ( line.charAt(0) ) {
                case '*':
                    break;
                case 'S':
                    return;
                case 'M':
                    assertDataTimed("(thereExists ?X (and (isa ?X NpcCol"+ arg[2] + ") (in-ContGeneric ?X "+ getAsArea( arg[4]) +")))",timer);
                //    assertDataTimed("(in-ContGeneric (InstanceClonedFn NpcCol"+ arg[2] + " "+ getAsArea( arg[4]) +") "+ getAsArea( arg[4]) +" )",timer);
                    npc = " NpcCol"+ arg[2] + " ";
                    break;
                case 'O':
                    // 
                    assertDataTimed("(thereExists ?X (and (isa ?X ArtifactCol"+ arg[2] + ") (in-ContGeneric ?X "+ getAsArea( arg[4]) +")))",timer);
                    //assertDataTimed("(in-ContGeneric (InstanceClonedFn ArtifactCol"+ arg[2] + " "+ getAsArea( arg[4]) +") " + getAsArea( arg[4]) + " )",timer);
                    break;

                case 'G':
                    assertDataTimed("(mudInventory ArtifactCol"+ arg[2]+"  " + npc + ") ",timer);
                    break;
                case 'E':
                    //wearsClothing
                    assertDataTimed("(mudWorn " + npc +" " + bodyPlaceWorn(arg[3])+ "  ArtifactCol"+ arg[2]+" )",timer);
                    break;
                case 'P':
                    assertDataTimed("(mudInventory  ArtifactCol"+ arg[2] + "  ArtifactCol"+ arg[4] + " )",timer);
                    break;
                case 'D':
                    assertDataTimed("(mudExitState "+ getAsArea(arg[2]) + "  " + getDirectionWards(arg[3]) + " PortalState"+getDirectionState( arg[4]) + " )",timer);
                    break;
                case 'R':
                    break;
                default:
                    break;
            }
        }
    }

    public String bodyPlaceWorn(String arg) {
        return "Worn"+bodyPlace(arg);
    }

    public String bodyPlace(String arg) {
        switch ( Integer.parseInt(arg) ) {
            case 0:
                return "AsLight";
            case 1:
                return "OnRightFinger";
            case 2:
                return "OnLeftFinger";
            case 3:
            case 4:
                return "AroundNeck";
            case 5:
                return "OnBody";
            case 6:
                return "OnHead";
            case 7:
                return "OnLegs";
            case 8:
                return "OnFeet";
            case 9:
                return "OnHands";
            case 10:
                return "OnArms";
            case 11:
                return "AsSheild";
            case 12:
                return "AboutBody";
            case 13:
                return "AroundWaist";
            case 14:
                return "AroundRightWrist";
            case 15:
                return "AroundLeftWrist";
            case 16:
                return "Wielded";
            case 17:
                return "Held";
        }
        return "AsHeld";
    }

    public void assertDataTimed(String fact,String timer) {
        assertData(fact);
        //assertData("(mudResetAssert " + fact + " " + timer + ")");
    }

}




