<?php

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
}


Config::import('inc/defaultsettings.inc');
Config::import('settings.php');


