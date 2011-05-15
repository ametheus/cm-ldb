<?php


require_once( "inc/adapters/persoon.inc" );

header( "Content-type: text/plain" );

$PA = Adapters\Persoon::IDs();
$IDs = $PA->execute(array());
$I = array();

foreach ( $IDs as $r )
{
    print( "{$r["pers_id"]} " );
    $I[] = $r["pers_id"];
}

print( "\n\nComplement:\n" );

for ( $i = 0; $i < 2657; $i++ )
{
    if ( in_array($i,$I) ) { continue; }
    print( "$i " );
}
