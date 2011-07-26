<?php

require_once("inc/db.inc");
require_once('fpdf/fpdf.php');

class Adresstickers
{
    static $margin = 6;
    
    static function download( $table, $title = "Adresstickers" )
    {
        $p = new FPDF( 'P', 'mm', 'A4' );
        $paper_size = array( 210, 297 );
        
        $p->SetAuthor( Auth::username(), true );
        $p->SetTitle( $title, true );
        $p->SetCreator( Config::full_app(), true );
        
        $p->SetMargins( Adresstickers::$margin, 0, Adresstickers::$margin );
        $p->SetAutoPageBreak( false );
        
        
        $p->SetFont( 'Times', '', 11);
        $fheight = 11 * 0.3528;
        
        $amt = Adresstickers::get_amt();
        
        list(
            // Left, top margin
            $MX, $MY,
            // Horizontal, vertical separator
            $DX, $DY,
            // Padding
            $padding
        ) = Adresstickers::get_margins();
        
        // Paper (printable area) height/width
        $PW = $paper_size[0] - (2 * $MX);
        $PH = $paper_size[1] - (2 * $MY);
        
        // Block (sticker+margins) height/width
        $BW = ( $PW + $DX ) / $amt[0];
        $BH = ( $PH + $DY ) / $amt[1];
        // ( We need to add a virtual separator to the right, as there are $amt[0]-1 separators total. )
        
        // Sticker height/width
        $SW = $BW - $DX;
        $SH = $BH - $DY;
        
        // Line height/width
        $LW = $SW - 2*$padding;
        $LH = 1.07 * $fheight;
        
        
        $L = Adresstickers::get_layout();
        $lines = count(explode("\n",$L));
        
        $ex = array(0,0);
        $c = 0;
        $max = count($table);
        while ( $c < $max )
        {
            $p->AddPage();
            
            for ( $i = 0; $i < $amt[1]; $i++ )
            {
                for ( $j = 0; $j < $amt[0]; $j++ )
                {
                    $X = $MX + ($j * $BW);
                    $Y = $MY + ($i * $BH);
                    $p->SetXY( $X, $Y );
                    
                    $T = $L;
                    foreach ( $table[$c] as $k => $v )
                    {
                        $T = str_ireplace( "<{$k}>", $v, $T );
                    }
                    $T = utf8_decode(trim(preg_replace('/[ ]+/',' ',$T)));
                    
                    $p->MultiCell( $LW, $LH, $T, 0, 'L', false );
                    
                    // Count eccentricities
                    if ( $p->GetY() > 8 )
                        $ex[0]++;
                    if ( $p->GetY() > 8 )
                        $ex[1]++;
                    
                    $c++;
                    if ( $c >= $max ) { break; }
                }
                
                if ( $c >= $max ) { break; }
            }
        }
        
        $debug_page = <<<EOT
Paper size:  {$paper_size[0]}x{$paper_size[1]}mm

Left, top margin:  {$MX}mm / {$MY}mm
Horizontal, vertical separator:  {$DX}mm / {$DY}mm
Padding:  {$padding}mm

Paper (printable area) height/width:  {$PW}x{$PH}mm
Block (sticker+margins) height/width:  {$BW}x{$BH}mm
Sticker height/width:  {$SH}x{$SW}mm
Line height/width:  {$LW}x{$LH}mm


Eccenticities:  {$ex[0]}.
Critical:  {$ex[1]}.
EOT;
        $p->AddPage();
        $p->SetXY(20,30);
        $p->MultiCell( 150, 1.4*$fheight, $debug_page, 0, 'L' );
        
        
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
    
    /**
     * @return array( left, top, between_horiz, between_vert, padding )
     */
    static function get_margins()
    {
        // TODO: In $_REQUEST zoeken
        return array( 7, 15, 2.5, 0, 3 );
    }
}




