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
   Configuration.
*/

class Config
{
    private static $version = '3.2.7c';
    static function version() { return Config::$version; }
    static function short_app() { return "Ledendatabase v" . Config::$version; }
    static function full_app()  { return "LSKO Collegium Musicum - Ledendatabase - v" . Config::$version; }
    static function version_ua() { return "LSKO_Collegium_Musicum-Ledendatabase/" . Config::$version; }
    
    private static $tries = 2;
    private static $data = array();
    
    static function get( $what )
    {
        if ( array_key_exists($what,Config::$data) )
        {
            return Config::$data[$what];
        }
        throw new Exception( "[{$what}] ain't no country I ever heard of. Do they speak English in [{$what}]?" );
    }
    
    static function import( $filename )
    {
        if ( Config::$tries == 0 )
        {
            throw new Exception( "Can only import settings twice. Sorry." );
        }
        Config::$tries--;
        
        @include( $filename );
        
        if ( isset($query_dir) ) { Config::$data['query_dir'] = $query_dir; }
        if ( isset($default_domain) ) { Config::$data['default_domain'] = $default_domain; }
        
        if ( isset($sticker_logo_file) ) { Config::$data['sticker_logo_file'] = $sticker_logo_file; }
        
        if ( isset($secret_key) ) { Config::$data['secret_key'] = $secret_key; }
        
        if ( isset($sql_connection_readonly)  ) { Config::import_connection("sql_readonly", $sql_connection_readonly);  }
        if ( isset($sql_connection_readwrite) ) { Config::import_connection("sql_readwrite",$sql_connection_readwrite); }
        
        if ( isset($database_backup_file) ) { Config::$data["database_backup_file"] = $database_backup_file; }
    }
    
    private static function import_connection( $name, $conn )
    {
        if ( !array_key_exists($name,Config::$data) )
        {
            Config::$data[$name] = array();
        }
        foreach ( array('hostname','username','password','database','log file') as $k )
        {
            if ( array_key_exists($k,$conn) )
            {
                Config::$data[$name][$k] = $conn[$k];
            }
        }
    }
    
    private static $_mysql_version = false;
    function mysql_version()
    {
        $XC = "MySQL version";
        
        if ( Config::$_mysql_version === false )
        {
            if ( xcache_isset($XC) )
            {
                Config::$_mysql_version = xcache_get($XC);
            }
            else
            {
                $output = shell_exec('mysql -V'); 
                preg_match('@[0-9]+\.[0-9]+\.[0-9]+@', $output, $version); 
                Config::$_mysql_version = $version[0];
                
                xcache_set( $XC, Config::$_mysql_version, 86400 );
            }
        }
        return Config::$_mysql_version;
    }
    function ubuntu_version()
    {
        $LSB = file_get_contents("/etc/lsb-release");
        $Ls = explode( "\n", $LSB );
        foreach ( $Ls as $l )
        {
            @list($k, $v) = explode( "=", $l );
            if ( $k == "DISTRIB_RELEASE" )
                return $v;
        }
        return "1998";
    }
    private static $_backup_stats = false;
    static function backup_stats( $k )
    {
        if ( Config::$_backup_stats === false )
        {
            $F = Config::$data["database_backup_file"];
            while ( file_exists($F) && is_link($F) )
            {
                $F = readlink($F);
            }
            if ( ! file_exists($F) )
            {
                Config::$_backup_stats = array( 'size'=>'0B', 'date'=>0 );
            }
            else
            {
                $S = stat($F);
                Config::$_backup_stats = array(
                    'size' => $S[7],
                    'date' => $S[9]
                );
            }
            Config::$_backup_stats["ago"] = time() - Config::$_backup_stats["date"];
        }
        
        return Config::$_backup_stats[$k];
    }
}


Config::import('inc/defaultsettings.inc');
Config::import('settings.php');


