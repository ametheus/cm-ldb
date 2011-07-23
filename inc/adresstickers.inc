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
        $p->SetAutoPageBreak( false );
        
        
        $L = Adresstickers::get_layout();
        $lines = count(explode("\n",$L));
        $amt = Adresstickers::get_amt();
        $ssize = array( $paper_size[0]/$amt[0], $paper_size[1]/$amt[1] );
        
        $W = $ssize[0] - ( 2 * Adresstickers::$margin );
        $H = 1.05 * $fheight;
        
        $c = 0;
        $max = count($table);
        while ( $c < $max )
        {
            $p->AddPage();
            
            for ( $i = 0; $i < $amt[1]; $i++ )
            {
                for ( $j = 0; $j < $amt[0]; $j++ )
                {
                    $X = Adresstickers::$margin + ($j * $ssize[0]);
                    $Y = ($i * $ssize[1]) + ($ssize[1] - $lines*$H*1.2)/2;
                    $p->SetXY( $X, $Y );
                    
                    $T = $L;
                    foreach ( $table[$c] as $k => $v )
                    {
                        $T = str_ireplace( "<{$k}>", $v, $T );
                    }
                    $T = utf8_decode(trim(preg_replace('/[ ]+/',' ',$T)));
                    
                    $p->MultiCell( $W, $H, $T );
                    
                    $c++;
                    if ( $c >= $max ) { break; }
                }
                
                if ( $c >= $max ) { break; }
            }
        }
        
        $p->Output( $title, 'I' );
        
        exit;
    }
    
    static function get_layout()
    {
        // TODO: in $_REQUEST zoeken
        return "<titel> <voornaam> <tussenvoegsel> <achternaam>\n<adres>\n<postcode> <plaats>";
    }
    
    static function get_amt()
    {
        // TODO: in $_REQUEST zoeken
        return array( 3, 7 );
    }
}




