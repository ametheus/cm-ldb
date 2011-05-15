<?php

namespace Adapters;

require_once("inc/db.inc");

class Persoon
{
    private static $_IDs = false;
    private static function _IDs()
    {
        $Q = <<<EOT
        SELECT pers_id
        FROM persoon
        ORDER BY pers_id
EOT;
        
        Persoon::$_IDs = new \Command( $Q, \DB::ro() );
        return Persoon::$_IDs;
    }
    static function IDs()
    {
        if ( ! Persoon::$_IDs ) { return Persoon::_IDs(); }
        return Persoon::$_IDs;
    }
    
    
    private static $_Detail = false;
    private static function _Detail()
    {
        $Q = <<<EOT
        SELECT * FROM persoon
        WHERE ABS( pers_id - @persoon ) < 5
EOT;
        Persoon::$_Detail = new \Command( $Q, \DB::ro(), array('pers_id') );
        return Persoon::$_Detail;
    }
    static function Detail()
    {
        if ( ! Persoon::$_Detail ) { return Persoon::_Detail(); }
        return Persoon::$_Detail;
    }
}



