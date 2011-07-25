<?php

class Config
{
    private static $version = '3.2.7';
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
    }
}


Config::import('inc/defaultsettings.inc');
Config::import('settings.php');


