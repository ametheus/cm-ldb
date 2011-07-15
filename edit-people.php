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
            $rklasse = htmlentities($klasse);
            print( "                    <optgroup label=\"".ucfirst($rklasse)."\">\n" );
        }
        print( "                        <option value=\"{$groep["groep_id"]}\" klasse=\"{$rklasse}\">{$groep["groepsnaam"]}</option>\n" );
        # Ja, dat is ongeldig HTML5. Sorry, w3c.
    }
}
function Studies()
{
    $DA = Adapters\SGR::Studies();
    $s = $DA->execute();
    $rv = "\n";
    foreach ( $s as $studie )
    {
        $sn = htmlentities(ucfirst( $studie["studienaam"] ));
        $rv .= "                        <option value=\"{$studie["studie_id"]}\"\">{$sn}</option>\n";
    }
    return $rv . "                    ";
}
function Namen()
{
    $DA = Adapters\Persoon::Namen();
    $n = $DA->execute(array('pers_id'=>0,'dist'=>10000000));
    $rv = "\n";
    foreach ( $n as $P )
    {
        $pn = htmlentities($P['Naam']);
        $rv .= "                        <option value=\"{$P["pers_id"]}\"\">{$pn}</option>\n";
    }
    return $rv . "                    ";
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
elseif ( $_GET["json"] == "wijzig-SGR" )
{
    $transaction = json_decode( urldecode($_REQUEST["transaction"]), true );
    
    $DS = Adapters\SGR::delete_Studies();
    $IS = Adapters\SGR::insert_Studies();
    $US = Adapters\SGR::update_Studies();
    
    $DG = Adapters\SGR::delete_Groepen();
    $IG = Adapters\SGR::insert_Groepen();
    $UG = Adapters\SGR::update_Groepen();
    
    function valid_studie( $studie )
    {
        if ( !is_array($studie) ) { return false; }
        if ( !is_numeric( @$studie["studie_id"] ) ) { return false; }
        if ( !is_numeric( @$studie["afgestudeerd"] ) ) { return false; }
        return true;
    }
    function valid_groep( $groep, $check_tot=false )
    {
        if ( !is_array($groep) ) { return false; }
        if ( !is_numeric(@$groep["groep_id"]) ) { return false; }
        if ( !preg_match(DATE_REGEX,@$groep["van"])) { return false; }
        if ( $check_tot )
        {
            if (( @$groep["tot"] !== null ) && !preg_match(DATE_REGEX,@$groep["tot"]) ) { return false; }
        }
        return true;
    }
    
    foreach ( $transaction["studies"] as $X )
    {
        list($pers_id, $voor, $na) = $X;
        
        if ( ! is_numeric($pers_id) ) { continue; }
        if ( $voor === null )
        {
            if ( !valid_studie($na) ) { continue; }
            
            $IS->execute(array('pers_id'=>$pers_id,'studie_id'=>$na["studie_id"],
                'afgestudeerd'=>( $na["afgestudeerd"] > 0 ? 'ja' : 'neen' )));
        }
        elseif ( $na === null )
        {
            if ( !valid_studie($voor) ) { continue; }
            
            $DS->execute(array('pers_id'=>$pers_id,'studie_id'=>$voor["studie_id"]));
        }
        else
        {
            if ( !valid_studie($na) ) { continue; }
            if ( !valid_studie($voor) ) { continue; }
            
            $US->execute(array('pers_id'=>$pers_id,
                'studie_id'=>$voor["studie_id"], 'n_studie_id'=>$na["studie_id"],
                'n_afgestudeerd'=>( $na["afgestudeerd"] > 0 ? 'ja' : 'neen' )));
        }
    }
    foreach ( $transaction["groepen"] as $X )
    {
        list($pers_id, $voor, $na) = $X;
        
        if ( ! is_numeric($pers_id) ) { continue; }
        if ( $voor === null )
        {
            if ( !valid_groep($na,true) ) { continue; }
            
            $IG->execute(array('pers_id'=>$pers_id,'groep_id'=>$na["groep_id"],'van'=>$na["van"],'tot'=>$na["tot"]));
        }
        elseif ( $na === null )
        {
            if ( !valid_groep($voor,false) ) { continue; }
            
            $DG->execute(array('pers_id'=>$pers_id,'groep_id'=>$voor["groep_id"],'van'=>$voor["van"]));
        }
        else
        {
            if ( !valid_groep($na,true) ) { continue; }
            if ( !valid_groep($voor,false) ) { continue; }
            
            $UG->execute(array('pers_id'=>$pers_id,'groep_id'=>$voor["groep_id"],'van'=>$voor["van"],
                'n_groep_id'=>$na["groep_id"],'n_van'=>$na["van"],'n_tot'=>$na["tot"]));
        }
    }
}
else
{
    print "Onbekend JSON-object. Sorry.";
}
