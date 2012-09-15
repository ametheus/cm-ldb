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

function I($n){return str_repeat("  ",$n);}

# Als je dit kunt lezen, ben ik vergeten libapache2-php5 te installeren. Oeps.

function LoQ()
{
    $queries = Query::queries_in_dir( Config::get('query_dir') );
    $qlist = Query::sort_query_list( $queries );
    foreach ( $qlist as $cat=>$qs )
    {
        print( I(5)."<h3><a href=\"#{$cat}\">{$cat}</a></h3>\n" );
        print( I(5)."<div>\n" );
        foreach ( $qs as $q )
        {
            print( I(6)."<a href=\"/query/{$q->file}\" title=\"{$q->Description}\" target=\"_new\">\n" );
            print( I(7)."<span class=\"title\">{$q->Title}</span>\n" );
            print( I(7)."<span class=\"description\">{$q->Description}</span>\n" );
            print( I(7)."<span class=\"author\">{$q->Author}</span>\n" );
            print( I(6)."</a>\n" );
        }
        print( I(5)."</div>\n" );
    }
}


header( "Content-type: text/html;charset=UTF-8" );
include("inc/templates/main-page.html");
exit;
