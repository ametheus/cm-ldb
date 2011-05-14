<?php

require_once( "auth.inc" );

if ( ! Auth::check() )
{
    Auth::deny_access( "Authorisatie ging grandioos mis." );
    exit;
}


class Connection
{
    private $_dbcon;
    
    function __construct( $host, $username, $password, $database )
    {
        $this->_dbcon = mysql_connect( $host, $username, $password );
        mysql_select_db( $database, $this->_dbcon );
    }
    
    function execute( Command $query, $arguments )
    {
        $Q = "";
        foreach ( $arguments as $k=>$v )
        {
            if ( ! preg_match( '/^[a-zA-Z0-9_]+$/', $k ) )
            {
                throw new Exception( "Parameter '" . mysql_escape_string($k) .
                    "' is invalid." );
            }
            $Q .= "SET @$k = '" . mysql_escape_string($v) . "';\n";
        }
        
        $Q .= "\n\n" . $query->text;
        
        print $Q;
    }
}


class Command
{
    private $_text;
    private $_connection;
    
    function __construct( $text, $connection )
    {
        $this->_text = $text;
        $this->_connection = $connection;
    }
    
    function __get( $name )
    {
        if ( $name == "text" )
        {
            return $this->_text;
        }
    }
    
    function execute( $arguments )
    {
        return $this->_connection->execute( $this, $arguments );
    }
}


class DB
{
    private static $_ro = null;
    private static $_rw = null;
    
    private static function construct_ro()
    {
        if ( Auth::check() > 0 )
        {
            DB::$_ro = new Connection(
                "www.collegiummusicum.nl",
                "deb7007_ldb_ro",
                "c9tdOACz",
                "deb7007_nerdcie" );
        }
    }
    
    private static function construct_rw()
    {
        if ( Auth::check() > 2 )
        {
            DB::$_rw = new Connection(
                "www.collegiummusicum.nl",
                "deb7007_ldb_rw",
                "7SHrklIx",
                "deb7007_nerdcie" );
        }
    }
    
    static function ro()
    {
        if ( ! DB::$_ro )
        {
            DB::construct_ro();
        }
        return DB::$_ro;
    }
    static function rw()
    {
        if ( ! DB::$_rw )
        {
            DB::construct_rw();
        }
        return DB::$_rw;
    }
}

