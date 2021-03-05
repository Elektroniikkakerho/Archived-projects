> Tämä projektikuvaus on siirretty tänne elektroniikkakerhon vanhoilta verkkosivuilta.

[Projektin etusivu](README.md)

### Yleistä

Laitteen ytimenä on Atmelin AT90S2313-mikrokontrolleri. Sen lisäksi kytkentään tarvitaan vain 8-bittinen siirtorekisteripiiri 74HC595, kaksi transistoria ja kourallinen passiivikomponentteja. Lisäksi tarvitaan tietysti neljä nappulaa ja valoa sekä kahden numeron 7-segmenttinäyttö.


### Nappulat

Kukin nappula tarvitsee ympärilleen yhden kondensaattorin ja 3 vastusta. Nappuloiden tulee olla sulkevia, eli nappulan navat yhdistyvät kun nappulaa painetaan. Mikrokontrollerin nappulanlukupinnit ovat normaalisti yhdistettyinä maahan 4,7 kohm alasvetovastuksen kautta. Kun nappulaa painetaan, lukupinniin kytkeytyy 5 V:n jännite nappulan kontaktien ja 100 ohm vastuksen kautta. Tällöin lukupinnille muodostuu resistiivinen jännitejako, jonka vaimennus on kuitenkin niin pieni, että sitä ei käytännössä huomaa. Näillä kahden vastuksen ansiosta lukupinni on kytkeytyneenä maahan eli loogiseen 0-tilaan nappulan ollessa levossa ja nappulaa painettaessa lukupinnin jännite nousee 5 V:iin eli loogiseen 1-tilaan.


Nappulat eivät koskaan toimi aivan täsmällisesti, vaan painettaessa ja päästettäessä syntyy värähtelyjä, joiden aikana ei voida tarkalleen sanoa, onko nappula painettuna vai ei. Tästä syystä mikrokontrolleri voisi tulkita yhden painalluksen useammaksi, jolloin pelaaminen menisi mahdottomaksi. Häiritsevät värähtelyt suodatetaan pois nappuloiden yhteyteen kytketyillä kondensaattoreilla. Kondensaattorin jännitehän ei voi muuttua askelmaisesti, vaan kondensaattoria täytyy ladata tai purkaa. Nappulaa painettaessa kondensaattori latautuu nopeasti loogiseen 1-tilaan pienen, vain 100 ohm ylösvetovastuksen kautta. Kun nappula päästetään, kondensaattorin varaus alkaa purkautua 4,7 kohm vastuksen kautta ja jännite laskee nollaan, mutta hitaammin kuin nousi 5 V:iin. Näin on värähtelyt eivät pääset häiritsemään pelaamista.


Kun peli sammutetaan, nappuloiden suodatuskondensaattoreihin voivat jäädä varaus. Tämä varaus voisi vaurioittaa mikrokontrolleria, jos se pääsisi esteettä purkautumaan mikrokontrollerin kautta. Tämän estämiseksi jokaisen nappulan yhteyteen on kytketty vielä kolmannet vastukset lukupinnien ja kondensaattorien väliin.


### Valojen ohjaus

Kunkin valon ohjaukseen on mikrokontrollerissa oma pinninsä. Valoksi voidaan kytkeä joko tavallinen ledi tai vaihtoehtoisesti vaikkapa hehkulamppu. Käytettäessä lediä ledin plus-napa kytketään 5 V jännitteeseen ja ledin miinus-napa viedään n. 175 ohm vastuksen kautta mikrokontrollerin kyseistä valoa ohjaavalle pinnille. Ledin etuvastuksen arvo 175 ohm on mitoitettu punaiselle/keltaiselle/vihreälle ledille. Muun värisiä ledejä käytettäessä etuvastuksen arvo pitää laskea uudelleen ledin kynnysjännitteen ja virran tarpeen mukaan.


Käytettäessä hehkulamppua, täytyy lampun ja mikrokontrollerin välissä käyttää kytkintransistoria mikrokontrollerin rajoitetun virranantokyvyn takia. Tässä kytkennässä käytetään NPN-transistoria, jonka emitteri kytketään maahan, kanta vastuksen kautta mikrokontollerin ohjausnastaan ja kollektori lampun toiseen napaan. Lampun toinen napa kytketään tarvittavaan (riippu lampusta) jänniteeseen mahdollisen etuvastuksen kautta.


Vaikka ledit ja hehkulamput kytketään hieman eri tavoin, ne toimivat mikrokontrollerin kannalta samoin, joten sama koodi yhtälailla kummankin vaihtoehdon kanssa.


### Pistenäyttöjen ohjaus

Näyttöjen ohjaamiseen tarvitaan 2 + 7 ohjauspinniä. Kahdella pinnillä ohjataan transistoreita, jotka antavat virtaa näytöille eli kytkevät 7-segmenttinäytön ledien plus-navat 5 V:n jännitteeseen. Kytkemällä vain toinen transistoreista kerrallaan johtavaksi voidaan valita sytytettävä näyttö. Loput seitsemän pinniä käytetään näyttöjen ledien miinus-napojen kytkemiseen vastuksen kautta joko maahan tai 5 V:iin. Kun ledin miinus-napa on kytketty maahan, sen läpi kulkee virta ja siihen syttyy valo, ja kun miinus-napa on kytketty 5 V:iin, virta ei kulje eikä valoa näy.


Haluttu numero saadaan siis näytölle, kun ohjataan seitsemän alasvetopinniä näytölle halutun numeron 7-segmentti-koodia vastaaviin tiloihin ja valitaan transistoreilla näyttö, johon numero halutaan. Näin saadaan kumpi tahansa kahdesta näytöstä näyttämään mitä tahansa numeroa. Näyttöjä on kuitenkin kaksi ja ne pitäisi saada toimimaan yhtä aikaa. Yhtäaikainen toiminta ei ole tällä kytkennällä mahdollinen, joten pitää käyttää kiertotietä. Kun näyttöjä välkytetään vuorotellen riittävän suurella taajuudella, silmä ei ehdi huomata välkkymistä ja luulee kummankin näytön palavan jatkuvasti. Välkkymistaajuuden alaraja on n. 50 Hz, mutta tässä laitteessa käytetään n. 250 Hz:n taajuutta. Se on enemmän kuin riittävästi, mutta ei kuitenkaan vielä liikaa.


Kuten edellä mainittiin, näyttöjen ohjaamiseen tarvitaan yhdeksän ohjauspinniä. Tämän laitteen käyttämässä mikrokontrollerissa niitä ei kuitenkaan ole niin paljoa, että näytöille liikenisi tarvittava määrä. Siksi näyttöjen ohjaamisessa turvaudutaan toiseen mikropiiriin, 8-bittiseen siirtorekisteriin 74HC595. Se on piiri, jonka ohjaamiseen tarvitaan tässä kytkennässä kolme ohjauspinniä ja sillä pystytään muuntamaan sarjamuodossa sille lähetetty 8-bittinen tavu rinnakkaismuotoon. Siirtorekisterissä on kahdeksan mikrokontrollerista käsin ohjattavaa ulostuloa, joista seitsemää käytetään määräämään näytössä palava numero. Yksi lähdöistä jää käyttämättä, joten jos haluaa alkaa itse virittelemään peliä, niin tuon ylimääräisen pinnin voisi laittaa ohjaamaan vaikkapa summeria.


Transistorien ohjaamiseen tarvittavat kaksi ohjauspinniä otetaan suoraan mikrokontrollerista. Siirtorekisteriä varten taas tarvitaan edellä mainitut kolme ohjauspinniä. Näin ollen siirtorekisteriä käyttämällä voidaan tehdä viidellä ohjauspinnillä se, mikä muuten vaatisi yhdeksän ohjauspinniä.

---

Copyright Antti Gärding 2003, 2004
