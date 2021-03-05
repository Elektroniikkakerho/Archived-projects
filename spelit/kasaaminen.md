> Tämä projektikuvaus on siirretty tänne elektroniikkakerhon vanhoilta verkkosivuilta.

[Projektin etusivu](README.md)

# AVR – Speden spelit – rakennusohjeet


### Piirilevy

Kasaaminen kannattaa aloittaa pääpiirilevystä, jolle tulee suurin osa laitteen komponenteista. Ensimmäisenä kannattaa juottaa paikoilleen mikropiirien kannat, joina käytetään holkkirimaa. Kantojen tarkoitus on suojata mikropiirejä palamiselta juotettaessa. Kun kannat ovat paikoillaan, muiden osien paikkojen löytäminen on paljon helpompaa. Seuraavaksi asennetaan paikoilleen passiivikomponentit eli vastukset, kondensaattorit ja kide. Niiden keskinäisellä juottamisjärjestyksellä ei ole merkitystä ja ne voi asentaa kummin päin haluaa, toisin sanoen niiden polariteetilla ei ole merkitystä. Tosin jos käyttää nappuloiden suotokondensaattoreina elektrolyyttikondensaattoreita, niin silloin polariteetillä on väliä, mutta tästä asiasta lisää myöhemmin. Passiivikomponenttien jälkeen juotetaan paikoilleen transistorit.


Seuraavaksi pitää valita, tekeekö pelistä yhdellä 9 V:n paristolla vai kolmella 1,5 V:n sormiparistolla toimivan. Jos haluaa käyttää 9 V:n paristoa, asennetaan jänniteregulaattori 7805 paikalleen osasijoittelukuvan mukaisesti. Haluttaessa käyttää sormiparistoja, regulaattoria jätetään pois ja se korvataan yhdellä hyppylangalla. Regulaattoria varten on piirilevyllä kolme vierekkäistä reikää. Hyppylanka juotetaan paikalleen siten, että se yhdistää regulaattorin rei'istä ulommaiset ja keskimmäinen reikä jää käytettäessä tyhjäksi.


Pääpiirilevyn jälkeen kannattaa jatkaa näytön piirilevyyn. Sen voi myös halutessaan jättää kokonaan pois, silloin täytyy vain korvata piirilevyn vedot ja yksi hyppylanka seitsemällä näytön jalkojen välille juotettavalla hyppylangalla ja lisäksi juottaa näytöön tulevat 9 johtoa suoraan näytön jalkoihin. Käytettäessä piirilevyä, siihen kannattaa juottaa aluksi holkkirimasta tehty kanta aivan kuten pääpiirilevyynkin. Rimat ovat yhdeksän nastaisia ja sen kokoisina ne eivät sovi muualle kuin oikeille paikoilleen. Kun holkkirimat on juotettu paikoilleen, pitää levylle lisätä vielä yksi hyppylanka. Se tulee niiden kahden reiällisen padin väliin, joihin ei holkkirimojen juottamisen jälkeen ole vielä juotettu mitään. Loput yhdeksän padia, jotka ovat kaikki reiättömiä, on tarkoitettu näytön ohjausjohtojen juottamiseen ja niitä tarvitaan vasta myöhemmin.


### Johdot

Johtojen juottamisen voi aloittaa vaikkapa paristonepparista. Se kytketään siten, että punainen johto tulee piirilevyn plus-merkillä, ja musta johto miinus-merkillä merkittyyn paikaan. Jos haluaa varustaa pelin kytkimellä, ettei paristoa tarvitse pelaamisen loputtua irrottaa, se juotetaan punaiseen johtoon niin, että punainen johto menee kytkimelle ja kytkimeltä lähtee toinen johto piirilevyn plus-reikään. Jos laitat laitteeseen kytkimen, mutta et ole aivan varma miten se kytketään, niin kysy apua. Siitä on vaikea antaa yleispätevää ohjetta, koska kytkimiä on niin monenlaisia.


Paristonepparin ja mahdollisen kytkimen juottamisen jälkeen olkoon näytön vuoro. Ensin juotetaan piirilevylle näytön ohjaamiseen tarvittavat yhdeksän johtoa. Johdoista seitsemän tulee osasijoittelukuvassa nimellä Display merkittyyn kohtaan reikiin A - G. Loput kaksi tulevat piirilevyn vastakkaiseen laitaan reikiin DISP1+ ja DISP10+. Johtojen toiset päät juotetaan näytön piirilevylle sen kuparipuolella olevien merkinöjen mukaisesti, jotka ovat yhteneviä pääpiirilevyn osasijoittelukuvan merkintöjen kanssa.


Viimeinen johdotukseen liittyvä operaatio on nappuloiden ja valojen asentaminen. Valoina voi käyttää oman valintansa mukaan joko ledejä tai tavallisia hehkulamppuja. Niiden ohjaukset eroavat hieman toisistaan, mutta piirilevy ja ohjelma soveltuvat sellaisinaan kumpaankin käyttöön. Jos pelin jänniteähteenä on tarkoitus käyttää paristoja, kannattaa valita valoiksi ledit niiden pienemmän virrankulutuksen takia. Kaikki valoihin liittyvät liitännät ovat piirilevylle osasijoittelukuvan Lights-tekstin yläpuolella. Ledejä käytettäessä piirilevylle ei asenneta osasijoittelukuvassa valojen reikien viereen piirrettyjä transistoreja. Ledit juotetaan paikoilleen siten, että ledin plus-napa tulee kyseistä valoa vastaavan transistorin ylimmän jalan paikalle ja miinus-napa vastaavan valon LEDx-nimiseen kohtaan. Ledien kanssa on oltava tarkka, jotta ne tulevat oikein päin. Plus-johdin on yleensä hieman pidempi ja miinus-johtimen puolella on yleensä pieni viiste ledin kauluksessa. Hehkulampun toinen napa juotetaan kohtaan BLBx ja toinen kohtaan LTx. Hehkulampuilla ei ole polariteettia eli on saman tekevää, kummin päin niiden navat kytketään.


Nappulat asennetaan samoin kuin leditkin niiden omille paikoilleen, jotka löytyvät osasijoittelukuvasta sanan Buttons yläpuolelta. Nappuloiden polariteetilla ei ole merkitystä.


VINKKI: Jos haluat säästää vaivaa johdottamisessa, niin voit jättää sekä valoissa että nappuloissa kolme kahdeksasta pääpiirilevyltä lähtevästä johdosta juottamatta. Ledejä varten olevat LEDx-padit on kaikki kytketty maahan, joten neljän LEDx-kohdista ledeille menevän johdon sijaan voidaan juottaa vain yksi tällainen johto, joka menee yhdelle ledeistä, ja sitten yhdistää ledien miinus-navat suoraan viemättä johtoja piirilevylle. Hehkulamppuja käytettäessä puolestaan Ltx-padit on kaikki kytketty 5 V:iin, joten niistä tulevat johdot voi korvata vastaavalla kytkennällä kuin ledien kanssa voi tehdä. Nappuloidenkin toiset navat on kaikki kytketty 100 ohm vastuksen kautta 5 V:iin, joten niidenkin kohdalla voidaan 4 pääpiirilevyltä lähtevää johtoa korvata edellä selostettuun tapaan.


### Testaaminen

Tässä vaiheessa, kun peli on mikropiirejä vaille valmis, siihen kytketään jännite. Jos sinulla ei ole aiempaa kokemusta elektroniikan rakentelusta, kysy neuvoa osaavammilta. Testaamisen tarkoitus on varmistaa, että kytkentä on kasattu oikein ja että mikropiireille tulee oikeat jännitteet.


### Viimeistely

Jos olet noudattanut tätä ohjetta, eikä kytkennästä löytynyt virheitä, edessäsi pitäisi nyt olla lähes valmis peli. Seuraavaksi on aika ohjelmoida mikrokontrolleri. Kysy sitä varten apua kerhon vetäjiltä. Kun mikrokontrolleri on ohjelmoitu, on jäljellä enää mikropiirien asettaminen kantoihinsa. Mikropiirit on ehdottomasti laitettava paikoilleen oikein päin, muuten ne voivat tuhoutua. Kummankin mikropiirin toisessa päässä on pieni viiste, joka näkyy myös osasijoittelukuvassa ja jonka perusteella piiri voidaan laittaa oikeaan asentoon.


### Lopuksi

Mikropiirien asentamisen jälkeen laite on valmis. Siihen voi kytkeä jännitteen, ja jos kaikki on kunnossa, pelaamisen voi aloittaa. Jos peli ei tunnu toimivan, kysy apua pelin suunnittelijalta tai joltakulta muulta osaavalta.

---

Copyright Antti Gärding 2003, 2004
