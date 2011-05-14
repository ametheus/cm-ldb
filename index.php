<?php

require_once( "inc/db.inc" );
require_once( "inc/query.inc" );

header("Content-type: text/plain");

# Als je dit kunt lezen, ben ik vergeten libapache2-php5 te installeren. Oeps.


$qs = Query::queries_in_dir( Config::$query_dir );
foreach ( $qs as $q )
{
    print( $q->Title . "\n" );
}



?>


SERVER:<?=print_r($_SERVER,true)?>
POST:<?=print_r($_POST,true)?>
REQUEST:<?=print_r($_REQUEST,true)?>
