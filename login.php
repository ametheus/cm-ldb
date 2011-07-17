<?php

require_once( "inc/auth.inc" );

if ( Auth::check() )
{
    if ( isset($_GET["continue"]) )
    {
        header("402 Found");
        header("Location: " . str_replace("\n","",$_GET["continue"]) );
        
        die( "<a href=\"".$_GET["continue"]."\">Klik hier om verder te gaan</a>" );
    }
}

die( "<a href=\"/\">Interessant. Deze boodschap hoor je nooit te kunnen zien.</a>" );


