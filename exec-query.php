<?php


require_once( "inc/db.inc" );
require_once( "inc/query.inc" );
require_once( 'inc/maillijst.inc' );
require_once( 'inc/openoffice.inc' );
require_once( 'inc/adresstickers.inc' );
$Q = Query::query_from_file();

if ( ! $Q )
{
    die( "Query niet gevonden." );
}

function I($n)
{
    return str_repeat(' ',4*$n);
}

function QSA( $map )
{
    $qs = "";
    foreach ( $map as $k => $v )
    {
        $qs .= "&" . urlencode($k) . "=" . urlencode($v);
    }
    if ( strlen($qs) > 0 ) { $qs = substr($qs,1); }
    
    if ( strpos($_SERVER["REQUEST_URI"],'?') !== false )
    {
        return $_SERVER["REQUEST_URI"] . '&' . $qs;
    }
    else
    {
        return $_SERVER["REQUEST_URI"] . '?' . $qs;
    }
}

$Result = $Q->execute();

function get_a_table()
{
    global $Result;
    
    $tn = @$_REQUEST["table"];
    $table = @$Result[$tn];
    if ( ! $table )
    {
        throw new Exception( "Er is geen tabel met de naam \"{$tn}\"." );
    }
    
    return $table;
}

if ( @$_REQUEST["as"] == "e-mail" )
{
    $table = get_a_table();
    
    header( "Content-type: text/plain" );
    print( Maillijst::maak_lijst( $table ) );
}
elseif ( @$_REQUEST["as"] == "ods" )
{
    Openoffice::SpreadSheet( $Result, $Q->Title );
    exit;
}
elseif ( @$_REQUEST["as"] == "stickers" )
{
    $table = get_a_table();
    
    Adresstickers::download( $table );
    exit;
}




?><!DOCTYPE html>
<html>
    <head>
        <title><?=$Q->Title?></title>
        
        <link type="text/css" href="/css/common.css" rel="stylesheet" />
        <style type="text/css">
        span.maillijstlink
        {
            padding: 2px;
        }
        </style>
        <link type="text/css" href="/css/humanity/jquery-ui-1.8.12.custom.css" rel="stylesheet" />
        <script type="text/javascript" src="/js/jquery.js"></script>
        <script type="text/javascript" src="/js/jquery-ui.js"></script>
        <script type="text/javascript" src="/js/ZeroClipboard.js"></script>
        
        <script type="text/javascript">
        $(function(){
            $("#Table-tabs").tabs();
        })
        </script>
    </head>
    <body>
        <div class="ods"><a href="<?=QSA(array('as'=>'ods'))?>">Openen in OpenOffice</a></div>
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
    $tid = preg_replace('/[^a-zA-Z0-9]/','',$table);
    print( I(3)."<div id=\"table-$tid\">\n" );
    print( I(4)."<div>\n" );
    print( I(5)."<span class=\"total_count\">Totaal: <strong>".count($data)."</strong>.</span>\n" );
    if ( isset($data[0]) && (isset($data[0]['email']) || isset($data[0]['Email'])) )
    {
        print( I(5)."<span class=\"maillijstlink\">" .
            "<a id=\"maillijstlink_$tid\" href=\"" .
            QSA(array('as'=>'e-mail','table'=>$table)) .
            "\">Openen als maillijst</a></span>\n" );
        Maillijst::klembordknop( "maillijstlink_$tid", Maillijst::maak_lijst($data),
            array('height' => 18, 'width' => 135) );
    }
    if ( isset($data[0]) && (isset($data[0]['adres'])    || isset($data[0]['Adres']))
                         && (isset($data[0]['postcode']) || isset($data[0]['Postcode']))
                         && (isset($data[0]['plaats'])   || isset($data[0]['Plaats']))
                        ) // TODO: Elegantere manier
    {
        print( I(5)."<a id=\"stickerlink_$tid\" href=\"" . QSA(array('as'=>'stickers','table'=>$table)) .
            "\">Adresstickers maken</a></span>\n" );
    }
    print( I(5)."\n" );
    print( I(4)."</div>\n" );
    if ( count($data) > 0 )
    {
        print( I(4)."<table>\n" );
        print( I(5)."<thead>\n" );
        print( I(6)."<tr>\n" );
        foreach ( array_keys($data[0]) as $col )
        {
            print( I(7)."<th>{$col}</th>\n" );
        }
        print( I(6)."</tr>\n" );
        print( I(5)."</tbody>\n" );
        print( I(5)."<thead>\n" );
        foreach ( $data as $row )
        {
            print( I(6)."<tr>\n" );
            foreach ( $row as $val )
            {
                print( I(7)."<td>{$val}</td>\n" );
            }
            print( I(6)."</tr>\n" );
        }
        print( I(5)."</tbody>\n" );
        print( I(4)."</table>\n" );
    }
    print( I(3)."</div>\n" );
}
?>
        </div>
        <script type="text/javascript">
        $(function(){
            function reposition_clips()
            {
                var ind = $("#Table-tabs").tabs( "option", "selected" );
                
                var id = $($("#Table-tabs > div")[ind]).attr('id').substr("table-".length);
                
                for ( i in ZeroClipboard.clients )
                {
                    var fid = ZeroClipboard.clients[i].domElement.id.
                        substr("maillijstlink_".length);
                    
                    if ( id == fid )
                    {
                        ZeroClipboard.clients[i].show();
                    }
                    else
                    {
                        ZeroClipboard.clients[i].hide();
                    }
                }
            }
            
            reposition_clips();
            $("#Table-tabs").bind( "tabsshow", reposition_clips );
        })
        </script>
    </body>
</html>