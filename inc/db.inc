<?php

require_once( "auth.inc" );

if ( ! Auth::check() )
{
    Auth::deny_access( "Authorisatie ging grandioos mis." );
    exit;
}


class Config
{
    static $query_dir = 'queries';
    
    # Database connectivity is configured below.
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
        
        if ( is_array( $query->text ) )
        {
            $rv = array();
            foreach ( $query->text as $k=>$v )
            {
                $rv[$k] = $this->exec( $v );
            }
            return $rv;
        }
        else
        {
            return $this->exec( $query->text );
        }
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
    private $_mandatory;
    private $_default;
    
    function __construct( $text, $connection, $mandatory=array(), $default=array() )
    {
        $this->_text = $text;
        $this->_connection = $connection;
        $this->_mandatory = $mandatory;
        $this->_default = $default;
        
        foreach ( $mandatory as $key )
            if ( array_key_exists($key,$default) )
                trigger_error("Key [$key] is mandatory, but has a default value.",E_USER_NOTICE);
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
        foreach ( $this->_mandatory as $arg )
        {
            if ( ! array_key_exists($arg,$arguments) )
            {
                throw new Exception("Mandatory argument [$arg] missing.");
            }
        }
        foreach ( $this->_default as $key=>$val )
        {
            if ( ! array_key_exists($key,$arguments) ) { $arguments[$key] = $val; }
        }
        return $this->_connection->execute( $this, $arguments );
    }
    
    function mandatory_args()
    {
        return $this->_mandatory;
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
                "localhost",
                "ldb_ro",
                "c9tdOACz",
                "ledendatabase" );
        }
    }
    
    private static function construct_rw()
    {
        if ( Auth::check() > 2 )
        {
            DB::$_rw = new Connection(
                "localhost",
                "ldb_rw",
                "7SHrklIx",
                "ledendatabase" );
        }
        else
        {
            DB::$_rw = DB::ro();
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

