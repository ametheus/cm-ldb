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
        print( I(4)."<h3>$cat</h3>\n" );
        foreach ( $qs as $q )
        {
            print( I(4)."<a href=\"/query/{$q->file}\" title=\"{$q->Description}\" target=\"_new\">\n" );
            print( I(5)."<span class=\"title\">{$q->Title}</span>\n" );
            print( I(5)."<span class=\"description\">{$q->Description}</span>\n" );
            print( I(5)."<span class=\"author\">{$q->Author}</span>\n" );
            print( I(4)."</a>\n" );
        }
    }
}



?><!DOCTYPE html>
<html>
    <head>
        <title><?=Config::short_app()?></title>
        
        <link rel="stylesheet" href="/css/common.css" />
        <link rel="stylesheet" href="/css/query-list.css" />
    </head>
    <body>
        <div id="MainQueryView">
            <div class="query_list tile_view">
<?php LoQ() ?>
            </div>
        </div>
        <div id="Modules">
            <a id="Mod-Bewerken" href="/bewerken"><div>Bewerken</div></a>
            <a id="Mod-Logout" href="/logout">
                <div>
                    Uitloggen
                    <br />
                    <span style="font-size: 75%; color: #999999;">Ingelogd als <?=Auth::username()?></span>
                </div>
            </a>
        </div>
    </body>
</html>