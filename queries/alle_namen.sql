CREATE OR REPLACE VIEW AlleNamen AS

SELECT
    pers_id,
    CONCAT( 
        CASE titel
            WHEN '' THEN 
                CASE voornaam WHEN '' THEN voorletters ELSE voornaam END
            ELSE
                concat( titel, CASE voorletters when '' then '' else concat(' ',voorletters)END )
        END,
        ' ', 
        CASE tussenvoegsel 
            WHEN '' THEN '' 
            else concat( tussenvoegsel, ' ' ) 
        END, 
        achternaam 
    ) AS `Naam`
FROM persoon
