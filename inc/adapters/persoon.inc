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
    
    
    private static $_Namen = false;
    private static function _Namen()
    {
        $Q = <<<EOT
        SELECT
            pers_id,
            CONCAT( 
                CASE titel
                    WHEN '' THEN 
                        CASE voornaam WHEN '' THEN voorletters ELSE voornaam END
                    ELSE
                        concat( titel, CASE voorletters when '' then '' else concat(' ',voorletters)END )
                END,
                ' ', 
                CASE tussenvoegsel 
                    WHEN '' THEN '' 
                    else concat( tussenvoegsel, ' ' ) 
                END, 
                achternaam 
            ) AS `Naam`
        FROM persoon
        WHERE ABS( pers_id - @pers_id ) < @dist
EOT;
        Persoon::$_Namen = new \Command( $Q, \DB::ro(), array('pers_id'), array('dist'=>5) );
        return Persoon::$_Namen;
    }
    static function Namen()
    {
        if ( !Persoon::$_Namen ) { return Persoon::_Namen(); }
        return Persoon::$_Namen;
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
    
    
    private static $_Search = false;
    private static function _Search()
    {
        $Q = <<<EOT
        SELECT pers_id FROM persoon
        WHERE
            achternaam LIKE concat('%',@term,'%') OR
            voornaam LIKE   concat('%',@term,'%') OR
            adres LIKE      concat('%',@term,'%') OR
            email LIKE      concat('%',@term,'%') OR
            banknummer LIKE concat('%',@term,'%') 
        ORDER BY pers_id DESC
EOT;
        Persoon::$_Search = new \Command( $Q, \DB::ro(), array('term') );
        return Persoon::$_Search;
    }
    static function Search()
    {
        if ( !Persoon::$_Search ) { return Persoon::_Search(); }
        return Persoon::$_Search;
    }
    
    
    private static $_Insert = false;
    private static function _Insert()
    {
        $Q = <<<EOT
        INSERT INTO persoon ( Achternaam ) VALUES ( 'Nieuw persoon' );
EOT;
        Persoon::$_Insert = new \Command( $Q, \DB::rw() );
        return Persoon::$_Insert;
    }
    static function Insert()
    {
        if ( !Persoon::$_Insert ) { return Persoon::_Insert(); }
        return Persoon::$_Insert;
    }
    
    
    private static $_Delete = false;
    private static function _Delete()
    {
        $Q = <<<EOT
        DELETE FROM persoon WHERE pers_id = @pers_id
            AND @weetjehetzeker = 'ja'
EOT;
        Persoon::$_Delete = new \Command( $Q, \DB::rw(), array('pers_id'), array('weetjehetzeker','neen'));
        return Persoon::$_Delete;
    }
    static function Delete()
    {
        if ( !Persoon::$_Delete ) { return Persoon::_Delete(); }
        return Persoon::$_Delete;
    }
    
    
    private static $_SGR = false;
    private static function _SGR()
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
        Persoon::$_SGR = new \Command( $Qs, \DB::ro(), array('pers_id') );
        return Persoon::$_SGR;
    }
    static function SGR()
    {
        if ( !Persoon::$_SGR ) { return Persoon::_SGR(); }
        return Persoon::$_SGR;
    }
}



