<?php

require_once( "inc/db.inc" );

header("Content-type: text/plain");

# Als je dit kunt lezen, ben ik vergeten libapache2-php5 te installeren. Oeps.

?>Als je dit kunt lezen, doet hij het nog niet. Maar wel bijna.


SERVER:<?=print_r($_SERVER,true)?>
POST:<?=print_r($_POST,true)?>
REQUEST:<?=print_r($_REQUEST,true)?>
