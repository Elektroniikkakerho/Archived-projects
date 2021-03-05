/*Ultra-kewl-RSS-parseri BY:
Antti Pohjola
*/

/* TODO
Tee lis�� ohjelmaan ominaisuus, joka tunnistaa rss:n version ja lukee sit� _oikein_. 
T�m� nykyinen versio k�y sivua vain rivi rivilt� l�pi :-) aivan riitt�v� purkka uutisten lukemiseen tosin.
*/

import java.io.*;
import java.net.*;


public class parseri {
	
       static String[] kellonaika = new String [50];
       static String[] uutinen = new String [50];
       static int rivilaskuri;
       
       public static void main ( String [] argv ) throws IOException {
       		//URLreadin j�lkeen tehd��n hoidetaan viel� automaagisesti parsimiset, eli maista ei tarvitse v�litt��
       		readFromURL(); 
     
       		while (true) {
                	try {
                    	Thread.sleep(600000); // 10 min = 600000
        		readFromURL(); 
                    	//System.out.println("No cookies today :(");
			
                 	} catch (InterruptedException e) {
                   	System.out.println("hitsit");
                	 }
		}
	}


/* Luetaan URL:st� teksti talteen */
	public static void readFromURL() throws IOException { 
		
		
		String tiedosto = "public_html/testi.txt";
		String merkkijono = "";
		String testijono = "";
		int alku = 0;
		int loppu = 0;
       		File roinaa = new File(tiedosto);
       		int rivilaskuri = 0;
       		int loppu_aika = 0;
       		int alku_aika = 0;
       		int index = 0;
       		int index2 = 0;
       		
   		try {
   			//http://dmz.fi/hesari.php, http://www.kaleva.fi/rss/index.xml, http://www.itviikko.fi/uutisboksi/rssindex.xml
      			URL url = new URL("http://www.kaleva.fi/rss/index.xml");
      			URLConnection connection = url.openConnection();
      			connection.setDoInput(true);
      			InputStream inStream = connection.getInputStream();
      			BufferedReader input =
         		new BufferedReader(new InputStreamReader(inStream));

      			String line = "";
      			String otsikko = "title";
      			// String kello = "timestamp"; for rss version 0.9
      			String kello = "pubDate";
      			rivilaskuri = 0;
      			
      			while ((line = input.readLine()) != null){
      					/*Tehd��n �beria taas ja kirjoitetaan timestamp ja otsikko taulukkoon */
      					/* Huom. Hox. kellonaika l�ytyy rss 0.9 versiosta suoraan timestampin sis�lt�, mutta
      					versio 2.0 n�ytt�� esim. t�lt� <pubDate>Wed, 05 Jan 2005 23:02:00 +0300</pubDate>
      					T�m� on jo aika mones versio t�st� ohjelmasta :-)*/
      					loppu_aika = otsikko_loppu(line,kello);
      					if (loppu_aika !=0) {
      						alku_aika = otsikko_alku(line,kello);
      						kellonaika[index] = line.substring(alku_aika+17,alku_aika+22);
      						index++;
      						rivilaskuri++;
      						//System.out.println(kellonaika[index-1]);
      					}
      				
      					
      					loppu = otsikko_loppu(line,otsikko);
      					if (loppu != 0){
      						alku = otsikko_alku(line,otsikko);
      						uutinen[index2] = (line.substring(alku,loppu));
      						//System.out.println(uutinen[index2]);
         					index2++;		
         				}
			}
   		}

   		catch (Exception e){

      		System.out.println(e.toString());
   		}
   		//System.out.println(rivilaskuri);
   		Kirjoita(rivilaskuri);
   	}
   	
   	/* Metodi otsikko_loppu, joka palattaa <tagi> tagiin p��ttyv�n merkkijonon kohdan numeerisesti*/
   	public static int otsikko_loppu(String jono, String tagi) {
   		int temp = jono.length();
   		int k = 0;
   		boolean alku = false;
   		boolean loppu = false;
   		while ( temp > k) { 
			while (temp > k) {
					if (jono.substring(k,temp).equals("</"+tagi+">")) {
						return (k);
					}
					
				
				k++;			
			}
			temp--;
		}
	return 0;
	}
	
	/* Metodi otsikko_alku, joka palauttaa <title> tagista alkavan merkkojonon kohdan numeerisesti */
	public static int otsikko_alku(String jono, String tagi){
		int temp = jono.length();
   		int k = 0;
   		while ( temp > k) { 
			while (temp > k) {
					if (jono.substring(k,temp).equals("<"+tagi+">")) {
						return temp;
					}
				temp--;			
			}
			temp = jono.length();
			k++;
		}
	return 0;
	}

	
	/* Metodi korjaa kalevan sivun ��kk�set 
	&amp;auml; on �
	&amp;ouml; on �
	&amp;Ouml; on �
	&amp;Auml; on �*/
	public static String Kaleva(String vanha) {
	//	System.out.println(vanha.substring(0,6));
		int temp = vanha.length();
		int k = 0;
		int k2 = 10;
		String uusi = "";
		
		while ( (temp + 1)  > k2) {
		//	System.out.println(vanha.substring(k,k2));
			if (vanha.substring(k,k2).equals("&amp;auml;")) {
			//	System.out.print("you get a free cookie!");
				uusi = (vanha.substring(0,k)+"�"+vanha.substring(k2,temp));
				vanha = uusi;
				k = -1;
				k2 = 9;
				temp = uusi.length();
			}
			else if (vanha.substring(k,k2).equals("&amp;ouml;")) {
				uusi = (vanha.substring(0,k)+"�"+vanha.substring(k2,temp));
				vanha = uusi;
				k = -1;
				k2 = 9;
				temp = uusi.length();
			}	
			else if (vanha.substring(k,k2).equals("&amp;Ouml;")) {
				uusi = (vanha.substring(0,k)+"�"+vanha.substring(k2,temp));
				vanha = uusi;
				k = -1;
				k2 = 9;
				temp = uusi.length();
			}	
			else if (vanha.substring(k,k2).equals("&amp;Auml;")) {
				uusi = (vanha.substring(0,k)+"�"+vanha.substring(k2,temp));
				vanha = uusi;
				k = -1;
				k2 = 9;
				temp = uusi.length();
			}	
			
				k++;
				k2++;		
		}
		return vanha;
	}
	
	public static void Kirjoita(int rivit_lkm) throws IOException { 
		int index = 0;
		File roinaa = new File("public_html/testi.txt");
		BufferedWriter out = new BufferedWriter(new FileWriter(roinaa));
		
		/*Optimointi kalevan sivua varten: kirjoitetaan se kaleva.plus rivi kahteen kertaan ilman timestamppia*/
		out.write(uutinen[index]+"       ");
		//System.out.println(uutinen[index]);
		out.write( "\n" );
		out.write(uutinen[index+1]+"       ");
		//System.out.println(uutinen[index+1]);
         	out.write( "\n" );
         	for (int i = 1; i < rivit_lkm; i++){
         		out.write("<"+kellonaika[i]+"> "+uutinen[i+1]+"       ");
         		out.write( "\n" );
         		//System.out.println("<"+kellonaika[i]+"> "+uutinen[i+1]+"       ");
		}
		out.close();
		
	}
}
// siin�p� se, ehk� joskus jaksan kirjoittaa t�m�n uusiksi ja integroida k�ytt�liittym��n. En kuitenkaan pid�tt�isi hengityst�.
