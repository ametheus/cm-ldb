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
   Adapter for personal data.
*/


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
    
    private static $_PersonPick = false;
    private static function _PersonPick()
    {
        $Q = <<<EOT
        SELECT
            pers_id AS id,
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
            ) AS `label`
        FROM persoon
        WHERE @q != '' AND (
            achternaam LIKE concat('%',@q,'%') OR
            voornaam LIKE   concat('%',@q,'%') OR
            adres LIKE      concat('%',@q,'%') OR
            email LIKE      concat('%',@q,'%') OR
            banknummer LIKE concat('%',@q,'%') )
        ORDER BY pers_id DESC
        LIMIT 15
EOT;
        Persoon::$_PersonPick = new \Command( $Q, \DB::ro(), array('q') );
        return Persoon::$_PersonPick;
    }
    static function PersonPick()
    {
        if ( ! Persoon::$_PersonPick ) { return Persoon::_PersonPick(); }
        return Persoon::$_PersonPick;
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
}



