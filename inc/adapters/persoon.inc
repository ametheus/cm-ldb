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
        WHERE ABS( pers_id - @pers_id ) < @dist
EOT;
        Persoon::$_Detail = new \Command( $Q, \DB::ro(), array('pers_id'), array('dist'=>5) );
        return Persoon::$_Detail;
    }
    static function Detail()
    {
        if ( ! Persoon::$_Detail ) { return Persoon::_Detail(); }
        return Persoon::$_Detail;
    }
    
    
    private static $_Modify = false;
    private static function _Modify()
    {
        $Q = <<<EOT
        UPDATE persoon SET
            achternaam        = @achternaam,
            aangetrouwdenaam  = @aangetrouwdenaam,
            voorletters       = @voorletters,
            voornaam          = @voornaam,
            tussenvoegsel     = @tussenvoegsel,
            titel             = @titel,
            geslacht          = @geslacht,
            adres             = @adres,
            postcode          = @postcode,
            plaats            = @plaats,
            land              = @land,
            post              = @post,
            telefoon          = @telefoon,
            mobiel            = @mobiel,
            email             = @email,
            geboortedatum     = @geboortedatum,
            sterftedatum      = @sterftedatum,
            banknummer        = @banknummer,
            tenaamstelling    = @tenaamstelling,
            nationaliteit     = @nationaliteit,
            voorkeurstaal     = @voorkeurstaal,
            opm               = @opm
        WHERE pers_id = @pers_id
EOT;
        Persoon::$_Modify = new \Command( $Q, \DB::rw(), array(
            'achternaam',       'aangetrouwdenaam', 'voorletters',
            'voornaam',         'tussenvoegsel',    'titel',
            'geslacht',         'adres',            'postcode',
            'plaats',           'land',             'post',
            'telefoon',         'mobiel',           'email',
            'geboortedatum',    'sterftedatum',     'banknummer',
            'tenaamstelling',   'nationaliteit',    'voorkeurstaal',
            'opm',              'pers_id'));
        return Persoon::$_Modify;
    }
    static function Modify()
    {
        if ( ! Persoon::$_Modify ) { return Persoon::_Modify(); }
        return Persoon::$_Modify;
    }
}



