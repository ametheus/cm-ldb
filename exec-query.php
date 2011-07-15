<?php


require_once( "inc/db.inc" );
require_once( "inc/query.inc" );
$Q = Query::query_from_file();

if ( ! $Q )
{
    die( "Query niet gevonden." );
}

$Result = $Q->execute();

if ( @$_REQUEST["as"] == "e-mail" )
{
	// https://mail.google.com/a/collegiummusicum.nl/?view=cm&fs=1&tf=1&source=mailto&bcc=test
	$tn = @$_REQUEST["table"];
	$table = @$Result[$tn];
	if ( ! $table )
	{
		throw new Exception( "Er is geen tabel met de naam \"{$tn}\"." );
	}
	
	$ml = "";
	foreach ( $table as $row )
	{
		if ( ! array_key_exists( "email", $row ) )
		{
			throw new Exception( "Dit is volgens mij geen maillijst." );
		}
		if ( ! preg_match(EMAIL_REGEX,$row["email"]) ) { continue; }
		$ml .= ",\n" . $row["email"];
	}
	
	header('Content-type: text/plain');
	print( substr($ml,2) );
}




?><!DOCTYPE html>
<html>
    <head>
        <title><?=$Q->Title?></title>
        
        <link type="text/css" href="/css/common.css" rel="stylesheet" />	
        <link type="text/css" href="/css/humanity/jquery-ui-1.8.12.custom.css" rel="stylesheet" />	
		<script type="text/javascript" src="/js/jquery-1.5.1.min.js"></script>
		<script type="text/javascript" src="/js/jquery-ui-1.8.12.custom.min.js"></script>
        
        <script type="text/javascript">
        $(function(){
            $("#Table-tabs").tabs();
        })
        </script>
    </head>
    <body>
        <div id="Table-tabs">
            <ul>
<?php
foreach ( array_keys($Result) as $table )
{
    $tid = preg_replace('/[^a-zA-Z0-9]/','',$table);
    print( "                <li><a href=\"#table-$tid\">$table</a></li>\n" );
}
?>
            </ul>
<?php
foreach ( $Result as $table=>$data )
{
    if ( count($data) == 0 ) { continue; }
    $tid = preg_replace('/[^a-zA-Z0-9]/','',$table);
    print( "            <div id=\"table-$tid\">\n" );
    print( "            <table>\n" );
    print( "                <tr>\n" );
    foreach ( array_keys($data[0]) as $col )
    {
        print( "                    <th>{$col}</th>\n" );
    }
    print( "                </tr>\n" );
    foreach ( $data as $row )
    {
        print( "                <tr>\n" );
        foreach ( $row as $val )
        {
            print( "                    <td>{$val}</td>\n" );
        }
        print( "                </tr>\n" );
    }
    print( "            </table>\n" );
    print( "            </div>\n" );
}
?>
        </div>
    </body>
</html>