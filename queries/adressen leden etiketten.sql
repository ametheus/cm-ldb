-- Spuwt een lijst uit met de adresgegevens van alle leden.


SELECT persoon.voornaam, persoon.tussenvoegsel, persoon.achternaam,
	persoon.adres, persoon.postcode, persoon.plaats, 
	persoon.email, persoon.mobiel, ( select studienaam from doetStudie 
		inner join studie ON studie.studie_id = doetStudie.studie_id 
		where doetStudie.pers_id = persoon.pers_id limit 1 ) AS `studie`
FROM persoon
WHERE ( select count(1) from lidVan
		inner join groep on groep.groep_id = lidVan.groep_id 
		where lidVan.pers_id = persoon.pers_id
			and groep.klasse IN ( 'grootkoor','kamerkoor', 'orkest' ) 
			and lidVan.van is not null  and lidVan.tot is null )
GROUP BY persoon.pers_id
ORDER BY voornaam, achternaam
