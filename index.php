<?php

require_once( "inc/db.inc" );
require_once( "inc/query.inc" );

function I($n){return str_repeat("    ",$n);}

# Als je dit kunt lezen, ben ik vergeten libapache2-php5 te installeren. Oeps.

function LoQ()
{
    $queries = Query::queries_in_dir( Config::$query_dir );
    $qlist = Query::sort_query_list( $queries );
    foreach ( $qlist as $cat=>$qs )
    {
        print( I(3)."<h3>$cat</h3>\n" );
        foreach ( $qs as $q )
        {
            print( I(3)."<a href=\"/query/{$q->file}\">\n" );
            print( I(4)."<span class=\"Title\">{$q->Title}</span>\n" );
            print( I(4)."<span class=\"Description\">{$q->Description}</span>\n" );
            print( I(4)."<span class=\"Author\">{$q->Author}</span>\n" );
            print( I(3)."</a>\n" );
        }
    }
}



?><!DOCTYPE html>
<html>
    <head>
        <title>Ledendatabase v3.2.0</title>
    </head>
    <body>
        <div id="QueryList">
<?php LoQ() ?>
        </div>
    </body>
</html>