package logicmoo.cmd;

import java.util.*;

public class BodyTrustedTable {

	protected ArrayList trustTable = new ArrayList();

	public synchronized void add(BodyTrusted cmd) { trustTable.add(cmd); }
	public synchronized void remove(BodyTrusted cmd) { trustTable.remove(trustTable.indexOf(cmd)); }


	public synchronized void sort() {
		sort(0, trustTable.size()-1 );
		trustTable.trimToSize();
	}


	private void sort(int from, int to) {
		int i = from, j = to;
		Object center = trustTable.get( (from + to) / 2 );

		do {
			while( (i < to) &&
				( ((BodyTrusted)center).getName().compareTo( ((BodyTrusted)trustTable.get(i)).getName() ) > 0 ) ) i++;

			while( (j > from) &&
				( ((BodyTrusted)center).getName().compareTo( ((BodyTrusted)trustTable.get(j)).getName() ) < 0 ) ) j--;

			if (i < j) {
				Object temp = trustTable.get(i);
				trustTable.set( i, trustTable.get(j) );
				trustTable.set( j, temp );
			}

			if (i <= j) {
				i++;
				j--;
			}
		} while(i <= j);

		if (from < j)
			sort(from, j);
		if (i < to)
			sort(i, to);
	}


	public BodyTrusted complete(String cmd, int trust) {
		Object[] o;
		synchronized(trustTable) {
			o = trustTable.toArray();
		}
		for(int I = 0; I < o.length; I++) {
			BodyTrusted bt = (BodyTrusted) o[I];
			if(bt.getName().startsWith(cmd) && bt.getTrust() <= trust)
				return bt;
		}
      return null;
   }

}