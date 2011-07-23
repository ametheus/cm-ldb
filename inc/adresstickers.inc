<?php

require_once("inc/db.inc");
require_once('fpdf.php');

class Adresstickers
{
    static function download( $table, $title = "Adresstickers" )
    {
        $p = new FPDF( 'P', 'mm', 'A4' );
        
        $p->AddPage();
        $p->SetFont( 'Arial', 'B', 16);
        $p->MultiCell( 170, 10, "Dit lijkt al meer op een pdf.\nVeel meer." );
        
        $p->Output( $title, 'I' );
        
        exit;
    }
}




