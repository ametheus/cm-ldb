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


header( "Content-type: text/plain" );

if ( $_GET["json"] == "IDs" )
{
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
elseif (( $_GET["json"] == "details" ) && ( is_numeric(@$_REQUEST["pers_id"]) ))
{
    $Detail = Adapters\Persoon::Detail();
    $Ds = $Detail->execute(array('pers_id'=>$_REQUEST["pers_id"]));
    print(json_encode($Ds));
}
elseif (( $_GET["json"] == "wijzig" ) && ( is_numeric(@$_POST["pers_id"]) ))
{
    $Modify = Adapters\Persoon::Modify();
    $args = array();
    foreach ( $Modify->mandatory_args() as $arg )
    {
        $args[$arg] = @$_POST[$arg];
    }
    $Ds = $Modify->execute($args);
    print( is_array($Ds) ? 'true' : 'false' );
}
else
{
    print "Onbekend JSON-object. Sorry.";
}
