<?php


require_once( 'inc/db.inc' );


function protected_param_names()
{
    return array('query');
}



function process_parameters( Query $Q )
{
    $params = $Q->Parameters;
    $ppn = protected_param_names();
    
    if ( count($params) == 0 ) { return array(); }
    
    $rv = array();
    
    foreach ( $params as $name=>$paramspec )
    {
        if ( in_array($name,$ppn) )
        {
            throw new Exception( "Parameter name [{$name}] is reserved.  Query: " . $Q->file );
        }
        
        if ( array_key_exists($name, $_REQUEST) )
        {
            $rv[$name] = $_REQUEST[$name];
            continue;
        }
    }
    
    # Controleer of er genoeg informatie is
    if ( count($rv) == count($params) ) { return $rv; }
    
    # Zoniet, maak een mooi formuliertje voor elke parameter.
    header( "content-type: text/plain" );
    foreach ( $params as $name=>$paramspec )
    {
        print( "Found parameter [$name] of type [{$paramspec['type']}], and I have no idea what to do with it.\n" );
    }
    exit;
}


