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
        if ( count($arguments ) > 0 )
        {
            $args = "";
            foreach ( $arguments as $k=>$v )
            {
                $args .= ", @`" . mysql_real_escape_string($k,$this->_dbcon) . "` = " .
                    "'" . mysql_real_escape_string($v,$this->_dbcon) . "'";
            }
            
            $this->exec( "SET " . substr($args,1) );
        }
        
        return $this->exec( $query->text );
    }
    
    private function exec( $text )
    {
        $rs = mysql_query( $text, $this->_dbcon );
        if ( mysql_errno() )
        {
            throw new Exception( "MySQL error " . mysql_errno() . ":\n" . mysql_error() . "\nQuery:\n" . $text );
        }
        $rv = array();
        while ( $r = @mysql_fetch_assoc( $rs ) )
        {
            $rv[] = $r;
        }
        return $rv;
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

