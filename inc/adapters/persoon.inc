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
}



