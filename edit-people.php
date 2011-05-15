<?php


require_once( "inc/adapters/persoon.inc" );

//header( "Content-type: text/plain" );

//$PA = Adapters\Persoon::IDs();
//$IDs = $PA->execute(array());


if ( ! isset($_GET["json"]) )
{
    include( "edit-people.html" );
    exit;
}


if ( $_GET["json"] == "IDs" )
{
    header( "Content-type: text-plain" );
    $PA = Adapters\Persoon::IDs();
    $IDs = $PA->execute(array());
    print( "[" );
    foreach ( $IDs as $i=>$r )
    {
        if ( $i ) { print(","); }
        print $r["pers_id"];
    }
    print( "]" );
}
