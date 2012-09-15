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
   Adapter voor Studies/groepen/relaties.
*/

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
            'relaties'=> "
                        SELECT * FROM (
                          SELECT
                            pers_id_2 as pers_id,
                            relatie
                          FROM relaties
                          WHERE pers_id_1 = @pers_id
                          
                          UNION
                          
                          SELECT
                            pers_id_1 as pers_id,
                            CASE relatie WHEN 'ouder' THEN 'kind' ELSE relatie END AS relatie
                          FROM relaties
                          WHERE pers_id_2 = @pers_id
                        ) prelaties GROUP BY pers_id ORDER BY pers_id"
        );
        SGR::$_Load = new \Command( $Qs, \DB::ro(), array('pers_id') );
        return SGR::$_Load;
    }
    static function Load()
    {
        if ( ! SGR::$_Load ) { return SGR::_Load(); }
        return SGR::$_Load;
    }
    
    
    
    private static $_Groepen = false;
    private static function _Groepen()
    {
        $Q = <<<EOT
            SELECT groep_id, klasse, groepsnaam
            FROM groep
            ORDER BY groep_id
EOT;
        SGR::$_Groepen = new \Command( $Q, \DB::ro() );
        return SGR::$_Groepen;
    }
    static function Groepen()
    {
        if ( ! SGR::$_Groepen ) { return SGR::_Groepen(); }
        return SGR::$_Groepen;
    }
    
    private static $_Studies = false;
    private static function _Studies()
    {
        $Q = <<<EOT
        SELECT studie_id, studienaam FROM studie
        ORDER BY studienaam
EOT;
        SGR::$_Studies = new \Command( $Q, \DB::ro() );
        return SGR::$_Studies;
    }
    static function Studies()
    {
        if ( ! SGR::$_Studies ) { return SGR::_Studies(); }
        return SGR::$_Studies;
    }
    
    
    
    private static $_insert_Groepen = false;
    private static function _insert_Groepen()
    {
        $Q = <<<EOT
        INSERT INTO lidVan( pers_id, groep_id, van, tot )
		  VALUES (
            @pers_id, @groep_id, @van,
				IF( LENGTH(@tot) < 4 OR SUBSTR(@tot,1,2) = '00', NULL, @tot )
        )
EOT;
        SGR::$_insert_Groepen = new \Command( $Q, \DB::rw(),
            array('pers_id', 'groep_id', 'van', 'tot') );
        return SGR::$_insert_Groepen;
    }
    static function insert_Groepen()
    {
        if ( ! SGR::$_insert_Groepen ) { return SGR::_insert_Groepen(); }
        return SGR::$_insert_Groepen;
    }
    
    
    private static $_delete_Groepen = false;
    private static function _delete_Groepen()
    {
        $Q = <<<EOT
        DELETE FROM lidVan
        WHERE pers_id = @pers_id
            AND groep_id = @groep_id
            AND van = @van
        LIMIT 1
EOT;
        SGR::$_delete_Groepen = new \Command( $Q, \DB::rw(),
            array('pers_id', 'groep_id', 'van') );
        return SGR::$_delete_Groepen;
    }
    static function delete_Groepen()
    {
        if ( ! SGR::$_delete_Groepen ) { return SGR::_delete_Groepen(); }
        return SGR::$_delete_Groepen;
    }
    
    
    private static $_update_Groepen = false;
    private static function _update_Groepen()
    {
        $Q = <<<EOT
        UPDATE lidVan SET
            groep_id = @n_groep_id,
            van = @n_van,
            tot = IF( length(@tot) < 4 OR substr(@n_tot,1,2)='00', NULL, @n_tot )
        WHERE pers_id = @pers_id
            AND groep_id = @groep_id
            AND van = @van
        LIMIT 1
EOT;
        SGR::$_update_Groepen = new \Command( $Q, \DB::rw(),
            array('pers_id', 'groep_id', 'van', 'n_groep_id', 'n_van', 'n_tot') );
        return SGR::$_update_Groepen;
    }
    static function update_Groepen()
    {
        if ( ! SGR::$_update_Groepen ) { return SGR::_update_Groepen(); }
        return SGR::$_update_Groepen;
    }
    
    
    
    private static $_insert_Studies = false;
    private static function _insert_Studies()
    {
        $Q = <<<EOT
        INSERT INTO doetStudie ( pers_id, studie_id, afgestudeerd )
        VALUES ( @pers_id, @studie_id, @afgestudeerd )
EOT;
        SGR::$_insert_Studies = new \Command( $Q, \DB::rw(),
            array( 'pers_id', 'studie_id', 'afgestudeerd' ) );
        return SGR::$_insert_Studies;
    }
    static function insert_Studies()
    {
        if ( ! SGR::$_insert_Studies ) { return SGR::_insert_Studies(); }
        return SGR::$_insert_Studies;
    }
    
    private static $_delete_Studies = false;
    private static function _delete_Studies()
    {
        $Q = <<<EOT
        DELETE FROM doetStudie
        WHERE pers_id = @pers_id
            AND studie_id = @studie_id
        LIMIT 1
EOT;
        SGR::$_delete_Studies = new \Command( $Q, \DB::rw(),
            array( 'pers_id', 'studie_id' ) );
        return SGR::$_delete_Studies;
    }
    static function delete_Studies()
    {
        if ( ! SGR::$_delete_Studies ) { return SGR::_delete_Studies(); }
        return SGR::$_delete_Studies;
    }
    
    private static $_update_Studies = false;
    private static function _update_Studies()
    {
        $Q = <<<EOT
        UPDATE doetStudie SET
            studie_id = @n_studie_id,
            afgestudeerd = @n_afgestudeerd
        WHERE pers_id = @pers_id
            AND studie_id = @studie_id
EOT;
        SGR::$_update_Studies = new \Command( $Q, \DB::rw(),
            array( 'pers_id', 'studie_id', 'n_studie_id', 'n_afgestudeerd' ) );
        return SGR::$_update_Studies;
    }
    static function update_Studies()
    {
        if ( ! SGR::$_update_Studies ) { return SGR::_update_Studies(); }
        return SGR::$_update_Studies;
    }
    
    
    
    private static $_insert_Relaties = false;
    private static function _insert_Relaties()
    {
        $Q = <<<EOT
        INSERT INTO relaties ( pers_id_1, pers_id_2, relatie )
        VALUES ( @pers_id_1, @pers_id_2, @relatie )
EOT;
        SGR::$_insert_Relaties = new \Command( $Q, \DB::rw(),
            array( 'pers_id_1', 'pers_id_2', 'relatie' ) );
        return SGR::$_insert_Relaties;
    }
    static function insert_Relaties()
    {
        if ( ! SGR::$_insert_Relaties ) { return SGR::_insert_Relaties(); }
        return SGR::$_insert_Relaties;
    }
    
    private static $_delete_Relaties = false;
    private static function _delete_Relaties()
    {
        $Q = <<<EOT
        DELETE FROM relaties
        WHERE ( pers_id_1 = @pers_id_1 AND pers_id_2 = @pers_id_2 )
            OR ( pers_id_1 = @pers_id_2 AND pers_id_2 = @pers_id_1 )
EOT;
        SGR::$_delete_Relaties = new \Command( $Q, \DB::rw(),
            array( 'pers_id_1', 'pers_id_2' ) );
        return SGR::$_delete_Relaties;
    }
    static function delete_Relaties()
    {
        if ( ! SGR::$_delete_Relaties ) { return SGR::_delete_Relaties(); }
        return SGR::$_delete_Relaties;
    }
}

