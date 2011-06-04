<?php


require_once( "inc/adapters/persoon.inc" );
require_once( "inc/adapters/SGR.inc" );

//header( "Content-type: text/plain" );

//$PA = Adapters\Persoon::IDs();
//$IDs = $PA->execute(array());


function Groepen()
{
    $DA = Adapters\SGR::Groepen();
    $g = $DA->execute();
    $klasse = false;
    $rv = "\n";
    foreach ( $g as $groep )
    {
        if ( $groep["klasse"] != $klasse )
        {
            if ( $klasse ) { print( "                    </optgroup>\n" ); }
            $klasse = $groep["klasse"];
            print( "                    <optgroup label=\"" .
                  htmlentities(ucfirst($klasse)) .
                  "\">\n" );
        }
        print( "                        <option value=\"{$groep["groep_id"]}\">{$groep["groepsnaam"]}</option>\n" );
    }
}



if ( ! isset($_GET["json"]) )
{
    include( "edit-people.html" );
    exit;
}


function print_pers_ids( $array )
{
    print( "[" );
    foreach ( $array as $i=>$r )
    {
        if ( $i ) { print(","); }
        print $r["pers_id"];
    }
    print( "]" );
}


header( "Content-type: text/plain" );

if ( $_GET["json"] == "IDs" )
{
    $PA = Adapters\Persoon::IDs();
    print_pers_ids( $PA->execute(array()) );
}
elseif (( $_GET["json"] == "namen" ) && ( is_numeric(@$_REQUEST["pers_id"]) ))
{
    $PA = Adapters\Persoon::Namen();
    $args = array( 'pers_id'=>$_REQUEST["pers_id"] );
    if ( is_numeric(@$_REQUEST["dist"]) ) { $args["dist"] = $_REQUEST["dist"]; }
    print(json_encode($PA->execute( $args )));
}
elseif (( $_GET["json"] == "details" ) && ( is_numeric(@$_REQUEST["pers_id"]) ))
{
    $Detail = Adapters\Persoon::Detail();
    $Ds = $Detail->execute(array('pers_id'=>$_REQUEST["pers_id"]));
    print(json_encode($Ds));
}
elseif (( $_GET["json"] == "zoek" ) && ( isset($_REQUEST["term"]) ))
{
    $Search = Adapters\Persoon::Search();
    $IDs = $Search->execute(array('term'=>$_REQUEST["term"]));
    print_pers_ids( $IDs );
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
elseif (( $_GET["json"] == "invoegen" ) && ( isset($_POST["failsafe"]) ))
{
    $I = Adapters\Persoon::Insert();
    $I->execute();
    print DB::rw()->insert_id();
}
elseif ( $_GET["json"] == "verwijderen" )
{
    $D = Adapters\Persoon::Delete();
    $ar = $D->execute(array('pers_id'=>@$_POST['pers_id'],'weetjehetzeker'=>@$_POST['weetjehetzeker']));
    print ( is_array($ar) ? 'true' : 'false' );
}
elseif ( $_GET["json"] == "SGR" )
{
    $DA = Adapters\SGR::Load();
    $rv = $DA->execute(array('pers_id'=>$_REQUEST["pers_id"]));
    print( json_encode($rv) );
}
else
{
    print "Onbekend JSON-object. Sorry.";
}
