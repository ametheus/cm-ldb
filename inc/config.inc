<?php

class Config
{
    static $query_dir = 'queries';
    
    private static $version = '3.2.7';
    static function version() { return Config::$version; }
    static function short_app() { return "Ledendatabase v" . Config::$version; }
    static function full_app()  { return "LSKO Collegium Musicum - Ledendatabase - v" . Config::$version; }
    static function version_ua() { return "LSKO_Collegium_Musicum-Ledendatabase/" . Config::$version; }
    
    # Note: Database connectivity is configured below.
}





