package logicmoo.cmd;

import java.util.*;

public class PlayerTrustedTable {

	protected ArrayList trustTable = new ArrayList();

	public synchronized void add(PlayerTrusted cmd) { trustTable.add(cmd); }
	public synchronized void remove(PlayerTrusted cmd) { trustTable.remove(trustTable.indexOf(cmd)); }


	public void sort() {
		sort(0, trustTable.size()-1 );
		trustTable.trimToSize();
	}


	private void sort(int from, int to) {
		// sort using Quicksort
		int i = from, j = to;
		Object center = trustTable.get( (from + to) / 2 );

		do {
			while( (i < to) &&
				( ((PlayerTrusted)center).getName().compareTo( ((PlayerTrusted)trustTable.get(i)).getName() ) > 0 ) ) i++;

			while( (j > from) &&
				( ((PlayerTrusted)center).getName().compareTo( ((PlayerTrusted)trustTable.get(j)).getName() ) < 0 ) ) j--;

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


   public PlayerTrusted complete(String cmd, int trust) {
		Object[] o;
		synchronized(trustTable) {
			o = trustTable.toArray();
		}
		for(int I = 0; I < o.length; I++) {
			PlayerTrusted pt = (PlayerTrusted) o[I];
			if(pt.getTrust() <= trust && pt.getName().startsWith(cmd))
				return pt;
		}
		return null;
	}

}