<?php

/*
    Copyright (C) 2011 Thijs van Dijk
    
    This file is part of CM-LDB.

    CM-LDB is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CM-LDB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CM-LDB.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
   This is the main page. Currently, it spouts a query list.
*/

require_once( "inc/db.inc" );
require_once( "inc/query.inc" );

function I($n){return str_repeat("    ",$n);}

# Als je dit kunt lezen, ben ik vergeten libapache2-php5 te installeren. Oeps.

function LoQ()
{
    $queries = Query::queries_in_dir( Config::get('query_dir') );
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