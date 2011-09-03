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
   Database abstraction layer.
*/

require_once( "config.inc" );
require_once( "auth.inc" );

if ( ! Auth::check() )
{
    Auth::deny_access( "Authorisatie ging grandioos mis." );
    exit;
}

define( "DATE_FREGEX", "[0-9]{4}-[0-9]{2}-[0-9]{2}" );
define( "DATE_REGEX", "/".DATE_FREGEX."/" );

# A regular expression to validate an e-mail address according to RFC 2822.
# Source: http://www.regular-expressions.info/email.html
define( "EMAIL_FREGEX", '(?:[a-z0-9!#$%&\'*+\\/=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&\'*+\\/=?^_`{|}~-]+)*|"(?:[\\x01-\\x08\\x0b\\x0c\\x0e-\\x1f\\x21\\x23-\\x5b\\x5d-\\x7f]|\\\\[\\x01-\\x09\\x0b\\x0c\\x0e-\\x7f])*")@(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\\[(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?|[a-z0-9-]*[a-z0-9]:(?:[\\x01-\\x08\\x0b\\x0c\\x0e-\\x1f\\x21-\\x5a\\x53-\\x7f]|\\\\[\\x01-\\x09\\x0b\\x0c\\x0e-\\x7f])+)\\])' );
define( "EMAIL_REGEX", "/".EMAIL_FREGEX."/" );


class Connection
{
    private $_dbcon;
    private $_log_file = false;
    private $_columns = null;
    
    function __construct( $host, $username, $password, $database, $log_file = false )
    {
        $this->_dbcon = mysql_connect( $host, $username, $password );
        mysql_select_db( $database, $this->_dbcon );
        mysql_set_charset('utf8',$this->_dbcon);
        
        if ( $log_file !== false )
        {
            $this->_log_file = @fopen( $log_file, 'a+' );
            if ( $this->_log_file )
            {
                fwrite( $this->_log_file, "/*/\n" );
            }
        }
    }
    
    function execute( Command $query, $arguments )
    {
        if ( $this->_log_file )
        {
            $this->log( sha1(print_r($query->text,true)) . ' - ' . print_r($arguments,true) );
        }
        
        if ( count($arguments ) > 0 )
        {
            $args = "";
            foreach ( $arguments as $k=>$v )
            {
                $V = ( ( $v === null ) ? "NULL" : "'" . mysql_real_escape_string($v,$this->_dbcon) . "'" );
                $args .= ", @`" . mysql_real_escape_string($k,$this->_dbcon) . "` = " . $V;
            }
            
            $this->exec( "SET " . substr($args,1) );
        }
        
        if ( is_array( $query->text ) )
        {
            $rv = array();
            $fieldinfo = array();
            
            foreach ( $query->text as $k=>$v )
            {
                $rv[$k] = $this->exec( $v );
                $fieldinfo[$k] = $this->_columns;
            }
            
            $this->_columns = $fieldinfo;
            return $rv;
        }
        else
        {
            return $this->exec( $query->text );
        }
    }
    
    function insert_id()
    {
        return mysql_insert_id( $this->_dbcon );
    }
    function field_info()
    {
        return $this->_columns;
    }
    
    private function log( $text )
    {
        if ( !$this->_log_file ) { return; }
        
        fwrite(
            $this->_log_file,
            date('c') . ' - ' .
            preg_replace('/\\s+/',' ',Auth::username()) . ' - ' .
            preg_replace('/\\s+/',' ',$text) .
            "\n"
        );
    }
    
    private function exec( $text )
    {
        $rs = mysql_query( $text, $this->_dbcon );
        if ( mysql_errno() )
        {
            throw new Exception( "MySQL error " . mysql_errno() . ":\n" . mysql_error() . "\nQuery:\n" . $text );
        }
        
        if ( is_resource($rs) )
        {
            $this->_columns = array();
            $fields = mysql_num_fields($rs);
            for ( $i = 0; $i < $fields; $i++ )
            {
                $this->_columns[] = array(
                    'name' => mysql_field_name($rs, $i),
                    'type' => mysql_field_type($rs,$i)
                );
            }
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
    # 
    # TODO:  Typed arguments!
    # 
    
    private $_text;
    private $_connection;
    private $_mandatory;
    private $_default;
    
    private $_fieldinfo;
    
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
        elseif ( $name == "fieldinfo" )
        {
            return $this->_fieldinfo;
        }
    }
    
    function execute( $arguments=array() )
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
        
        $rv = $this->_connection->execute( $this, $arguments );
        $this->_fieldinfo = $this->_connection->field_info();
        
        //print_r( $this->_fieldinfo );
        
        return $rv;
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
            $db = Config::get('sql_readonly');
            DB::$_ro = new Connection(
                $db['hostname'],
                $db['username'],
                $db['password'],
                $db['database'],
                $db['log file']
            );
        }
    }
    
    private static function construct_rw()
    {
        if ( Auth::check() > 2 )
        {
            $db = Config::get('sql_readwrite');
            DB::$_rw = new Connection(
                $db['hostname'],
                $db['username'],
                $db['password'],
                $db['database'],
                $db['log file']
            );
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

