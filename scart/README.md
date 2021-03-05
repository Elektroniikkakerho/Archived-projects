> Tämä projektikuvaus on siirretty tänne elektroniikkakerhon vanhoilta verkkosivuilta.

# SCART-jäynä

Tämä laite syntyi 3 viikon melkein täyspäiväisen uurastusen tuloksena keväällä 2004. Tarkoituksena oli tehdä killan jäynäkisajoukkueen käyttöön SCART-liitin, jonka sisään sijoitettu PIC16F84A-mikrokontrolleri -pohjainen laite näyttäisi TV:ssä valintaruutua

```

[ ] Kiinnostaa
[ ] Ei kiinnosta

```


Projekti onnistui hienosti tuoden killalle diplomin teknisimmästä jäynästä. Vain jäynän käytäntöönpanon suunnittelu jäi turhan vähäiseksi, mutta se voidaan laittaa kiireisen aikataulun syyksi.

Laitteen ytimenä on siis jo edellä mainittu PIC16F84A. Sen ja välttämättömien kiteen ja kondensaattoreiden lisäksi vaaditaan vain kaksi vastusta hoitamaan lähtevän TV-signaalin DA-muunnos ja jokunen vastus ja transistori kytkemään SCART-liittimen tulovalinnan ja kuvasuhteen ohjauspinniä 12 V:iin. Virtalähteenään laite käyttää fyysiseltä kooltaan jonkin verran normaalia sormiparistoa pienempää 12 V:n paristoa.

Projektin haaste oli saada PIC tekemään TV-signaalia ohjelmallisesti. TV-signaalilta vaadittiin alle 1 us:n ajoitustarkkuutta, joten koodin kirjoittaminen PIC:lle, joka 12 MHz kiteellä kellotettuna suorittaa käskyjä 4 MHz taajuudella, oli hieman vaativaa. Aliohjelmia ei voinut juurikaan käyttää, koska hyppyihin olisi mennyt liikaa aikaa, ja silmukoiden määrää karsi mm. se, että ei olisi ollut aikaa mahdollisten toistoehtojen tutkimiselle.

Tämän projektin tekemisessä oli korvaatonta apua [Rickard Guneen](http://www.rickard.gunee.com/projects) PIC-Pong -projektia esittelevistä sivuista. Alle linkittämässäni omassa koodissanikin on vielä jäljellä jokunen suoraan Guneen sivuilla esitetyistä esimerkistä lainattu rivi. Kiitokset Rickard Guneelle hänen loistavista kotisivuistaan.

Rickard Gunee's [home site](http://www.rickard.gunee.com/projects) was an invaluable source of help for this project. There are still a few rows of his example code left in my code linked below. Big thanks to Rickard Gunee for his great site.

[PIC:n pyörittämä assembly-koodi](stuf/tv-jayna.asm)

---

Copyright Antti Gärding 2004
