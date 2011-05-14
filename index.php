<?php

require_once( "inc/db.inc" );
require_once( "inc/query.inc" );

header("Content-type: text/plain");

# Als je dit kunt lezen, ben ik vergeten libapache2-php5 te installeren. Oeps.


$queries = Query::queries_in_dir( Config::$query_dir );
$qlist = Query::sort_query_list( $queries );
foreach ( $qlist as $cat=>$qs )
{
    print( "\n   * [$cat]\n" );
    foreach ( $qs as $q )
    {
        print( $q->Title . "\n" );
    }
}

