<?php

namespace Adapters;

require_once("inc/db.inc");

class SGR
{
    private static $_Load = false;
    private static function _Load()
    {
        $Qs = array(
            'studies' => "SELECT studie_id, studienaam,
                            case afgestudeerd when 'ja' then 1 else 0 end as afgestudeerd
                          FROM studie
                            JOIN doetStudie USING ( studie_id )
                          WHERE pers_id = @pers_id
                          ORDER BY studienaam",
            'groepen' => "SELECT groep_id, klasse, groepsnaam, van, tot
                          FROM lidVan
                            JOIN groep USING ( groep_id )
                          WHERE pers_id = @pers_id
                          ORDER BY van",
            'relaties'=> "SELECT * FROM relaties
                          WHERE pers_id_1 = @pers_id OR pers_id_2 = @pers_id"
        );
        SGR::$_Load = new \Command( $Qs, \DB::ro(), array('pers_id') );
        return SGR::$_Load;
    }
    static function Load()
    {
        if ( !SGR::$_Load ) { return SGR::_Load(); }
        return SGR::$_Load;
    }
}



