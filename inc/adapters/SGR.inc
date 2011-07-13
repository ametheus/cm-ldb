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
        if ( !SGR::$_Groepen ) { return SGR::_Groepen(); }
        return SGR::$_Groepen;
    }
    
    
    private static $_insert_Groepen = false;
    private static function _insert_Groepen()
    {
        $Q = <<<EOT
        INSERT INTO lidVan( pers_id, groep_id, van, tot )
        VALUES ( @pers_id, @groep_id, @van, @tot )
EOT;
        SGR::$_insert_Groepen = new \Command( $Q, \DB::rw(),
            array('pers_id', 'groep_id', 'van', 'tot') );
        return SGR::$_insert_Groepen;
    }
    static function insert_Groepen()
    {
        if ( !SGR::$_insert_Groepen ) { return SGR::_insert_Groepen(); }
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
        if ( !SGR::$_delete_Groepen ) { return SGR::_delete_Groepen(); }
        return SGR::$_delete_Groepen;
    }
    
    
    private static $_update_Groepen = false;
    private static function _update_Groepen()
    {
        $Q = <<<EOT
        UPDATE lidVan SET
            groep_id = @n_groep_id,
            van = @n_van,
            tot = @n_tot
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
        if ( !SGR::$_update_Groepen ) { return SGR::_update_Groepen(); }
        return SGR::$_update_Groepen;
    }
}

