<?php

require_once( "inc/db.inc" );

header("Content-type: text/plain");

# Als je dit kunt lezen, ben ik vergeten libapache2-php5 te installeren. Oeps.



$Q = <<<EOT
    SELECT Voornaam, Tussenvoegsel, Achternaam
    FROM persoon
    WHERE pers_id = @persoon
EOT;
$Query = new Command( $Q, DB::ro() );
print_r( $Query->execute( array( 'persoon' => 2021 ) ) );



?>


SERVER:<?=print_r($_SERVER,true)?>
POST:<?=print_r($_POST,true)?>
REQUEST:<?=print_r($_REQUEST,true)?>
