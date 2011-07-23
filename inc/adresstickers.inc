<?php

require_once("inc/db.inc");
require_once('fpdf.php');

class Adresstickers
{
    static $margin = 10;
    
    static function download( $table, $title = "Adresstickers" )
    {
        $p = new FPDF( 'P', 'mm', 'A4' );
        $paper_size = array( 210, 297 );
        
        $p->SetFont( 'Times', '', 12);
        $fheight = 12 * 0.3528;
        
        $p->SetAuthor( Auth::username(), true );
        $p->SetTitle( $title, true );
        $p->SetCreator( Config::full_app(), true );
        
        $p->SetMargins( Adresstickers::$margin, 0, Adresstickers::$margin );
        
        $L = Adresstickers::get_layout();
        $amt = Adresstickers::get_amt();
        $ssize = array( $paper_size[0]/$amt[0], $paper_size[1]/$amt[1] );
        
        $c = 0;
        while ( $c < 19 )
        {
            $p->AddPage();
            
            for ( $i = 0; $i < $amt[1]; $i++ )
            {
                for ( $j = 0; $j < $amt[0]; $j++ )
                {
                    $p->SetXY( Adresstickers::$margin + ($j * $ssize[0]),  $i * $ssize[1] );
                    
                    $p->MultiCell( $ssize[0] - ( 2 * Adresstickers::$margin ), 1.05 * $fheight, Adresstickers::get_layout() );
                    
                    $c++;
                    if ( $c >= 19 ) { break; }
                }
                
                if ( $c >= 19 ) { break; }
            }
        }
        
        $p->Output( $title, 'I' );
        
        exit;
    }
    
    static function get_layout()
    {
        // TODO: in $_REQUEST zoeken
        return "<naam>\n<adres>\n<postcode> <plaats>";
    }
    
    static function get_amt()
    {
        // TODO: in $_REQUEST zoeken
        return array( 3, 7 );
    }
}




