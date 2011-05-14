<?php


require_once( "inc/db.inc" );
require_once( "inc/query.inc" );
$Q = Query::query_from_file();

if ( ! $Q )
{
    die( "Query niet gevonden." );
}

print $Q->Title;

