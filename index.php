<?php

require_once( "inc/db.inc" );

header("Content-type: text/plain");

# Als je dit kunt lezen, ben ik vergeten libapache2-php5 te installeren. Oeps.



$Q1 = <<<EOT
    SELECT Voornaam, Tussenvoegsel, Achternaam
    FROM persoon
    WHERE pers_id = @`per-soon`
EOT;
$Q2 = <<<EOT
    SELECT Klasse, Groepsnaam, van, tot
    FROM lidVan
        JOIN groep USING ( groep_id )
    WHERE pers_id = @`per-soon`
EOT;
$Query = new Command( array($Q1,$Q2), DB::ro() );
print_r( $Query->execute( array( 'per-soon' => 2021 ) ) );



?>


SERVER:<?=print_r($_SERVER,true)?>
POST:<?=print_r($_POST,true)?>
REQUEST:<?=print_r($_REQUEST,true)?>
