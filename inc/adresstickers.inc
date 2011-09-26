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
   Download a PDF with address labels.
*/

require_once("inc/db.inc");
require_once('fpdf/fpdf.php');

class Adresstickers
{
    protected $pdf;
    public $title;
    
    protected $sizes;
    protected $image = null;
    
    protected $layout;
    protected $offset;
    
    function __construct( StickerTemplate $template, $title = "Adresstickers" )
    {
        if ( $template->unit != 'mm' ) { throw new Exception('Sorry, ik kan (nog) niet werken in maateenheid ['.$template->unit.'].'); }
        $this->pdf = new FPDF( $template->orientation, $template->unit, $template->paper_size );
        
        $this->pdf->SetAuthor( Auth::username(), true );
        $this->pdf->SetTitle( $title, true );
        $this->title = $title;
        $this->pdf->SetCreator( Config::full_app(), true );
        
        $this->pdf->SetMargins( $template->margins->page, 0, $template->margins->page );
        $this->pdf->SetAutoPageBreak( false );
        
        
        $this->pdf->SetFont( $template->font->family, '', $template->font->size );
        $fheight = $template->font->size * 0.3528; // Pt to mm
        
        $amt = $template->amount;
        
        $paper_size = $template->paper_size;
        $MX = $template->margins->left;
        $MY = $template->margins->top;
        $DX = $template->margins->hskip;
        $DY = $template->margins->vskip;
        $padding = $template->margins->padding;
        
        
        // Paper (printable area) width/height
        $PW = $paper_size[0] - (2 * $MX);
        $PH = $paper_size[1] - (2 * $MY);
        
        // Block (sticker+margins) width/height
        $BW = ( $PW + $DX ) / $amt[0];
        $BH = ( $PH + $DY ) / $amt[1];
        // ( We need to add a virtual separator to the right, as there are $amt[0]-1 separators total. )
        
        // Sticker width/height
        $SW = $BW - $DX;
        $SH = $BH - $DY;
        
        // Line width/height
        $LW = $SW - 2*$padding;
        $LH = 1.07 * $fheight;
        
        $this->sizes = new stdClass();
        $this->sizes->margin  = array( $MX, $MY );
        $this->sizes->block   = array( $BW, $BH );
        $this->sizes->sticker = array( $SW, $SH );
        $this->sizes->padding = $padding;
        $this->sizes->amount  = $amt;
        $this->sizes->line    = array( $LW, $LH );
        
        $this->prepare_image( $SW, $SH );
    }
    
    protected function prepare_image( $SW, $SH )
    {
        if ( !( $Ifile = Config::get('sticker_logo_file') ) || ( !file_exists($Ifile) )) { return; }
        
        try
        {
            list($IW,$IH) = getimagesize($Ifile);
        }
        catch (Exception $e) { return; }
        // Determine if the aspect ratio of the image is smaller or larger
        // than that of the sticker
        
        if ( ($IW/$IH) > ($SW/$SH) )
        {
            // Logo should go in the bottom half
            $IH = $IH * ($SW/$IW);
            $IW = $SW;
            
            $IX = 0;
            $IY = $SH - $IH;
        }
        else
        {
            // Logo should go in the right half
            $IW = $IW * ($SH/$IH);
            $IH = $SH;
            
            $IX = $SW - $IW;
            $IY = 0;
        }
        
        
        $this->image = new stdClass();
        $this->image->file = $Ifile;
        $this->image->size = array( $IW, $IH );
        $this->image->position = array( $IX, $IY );
    }
    
    protected function draw_logo( $sticker_origin )
    {
        if ( !$this->image ) { return; }
        
        
        
        $this->pdf->Image(
            $this->image->file,
            
            $sticker_origin[0] + $this->image->position[0],
            $sticker_origin[1] + $this->image->position[1],
            
            $this->image->size[0],
            $this->image->size[1]
        );
    }
    
    function append_from_table( $table )
    {
        $this->prepare_layout( $table );
        $M = $this->sizes->margin;
        $B = $this->sizes->block;
        
        $ex = array(0,0);
        $c = 0;
        $max = count($table);
        while ( $c < $max )
        {
            $this->pdf->AddPage();
            
            for ( $j = 0; $j < $this->sizes->amount[1]; $j++ )
            {
                for ( $i = 0; $i < $this->sizes->amount[0]; $i++ )
                {
                    $this->draw_sticker(
                        array(
                            $M[0] + ($i * $B[0]),
                            $M[1] + ($j * $B[1]) ),
                        $table[$c] );
                    
                    $c++;
                    if ( $c >= $max ) { break; }
                }
                if ( $c >= $max ) { break; }
            }
        }
    }
    
    protected function prepare_layout( $table )
    {
        $this->layout = Adresstickers::get_layout( $table );
        $lines = count(explode("\n",$this->layout));
        
        $this->offset = array(
            // X offset
            $this->sizes->padding,
            
            // Y offset
            $this->sizes->padding +
                max((($this->sizes->sticker[1] - $lines*$this->sizes->line[1]*1.3) / 2), 0)
        );
    }
    
    protected function draw_sticker( $block_origin, $row )
    {
        // Insert logo watermark
        $this->draw_logo( $block_origin );
        
        $X = $block_origin[0] + $this->offset[0];
        $Y = $block_origin[1] + $this->offset[1];
        $this->pdf->SetXY( $X, $Y );
        
        $L = $this->sizes->line;
        
        $T = $this->layout;
        foreach ( $row as $k => $v )
        {
            $T = str_ireplace( "<{$k}>", $v, $T );
        }
        $T = utf8_decode(trim(preg_replace('/[ ]+/',' ',$T)));
        
        $this->pdf->MultiCell( $L[0], $L[1], $T, 0, 'L', false );
        
        // Count eccentricities
        /*if ( $p->GetY() > 8 )
            $ex[0]++;
        if ( $p->GetY() > 8 )
            $ex[1]++;*/
    }
    
    function debug_page()
    {
        $ex = array(0,0);
        // Paper size:  {$paper_size[0]}x{$paper_size[1]}mm
        // Horizontal, vertical separator:  {$DX}mm / {$DY}mm
        // Paper (printable area) width/height:  {$PW}x{$PH}mm
        
        $debug_page = @<<<EOT

Left, top margin:  {$this->sizes->margin[0]}mm / {$this->sizes->margin[1]}mm
Padding:  {$this->sizes->padding}mm

Logo size:  {$this->image->size[0]}x{$this->image->size[1]}mm
Logo origin:  [{$this->image->position[0]},{$this->image->position[1]}]

Block (sticker+margins) width/height:  {$this->sizes->block[0]}x{$this->sizes->block[1]}mm
Sticker width/height:  {$this->sizes->sticker[0]}x{$this->sizes->sticker[1]}mm
Text offset:  [{$this->offset[0]},{$this->offset[1]}]
Line width/height:  {$this->sizes->line[0]}x{$this->sizes->line[1]}mm


Eccenticities:  {$ex[0]}.
Critical:  {$ex[1]}.
EOT;

        $this->pdf->AddPage();
        $this->pdf->SetXY(20,30);
        $this->pdf->MultiCell( 150, 1.3*$this->sizes->line[1], $debug_page, 0, 'L' );
        
        //$this->pdf->SetXY(1,1);
        //$this->pdf->Cell( $paper_size[0]-2, $paper_size[1]-2, "", 1 );
    }
    
    function download()
    {
        $this->pdf->Output( $this->title, 'I' );
    }
    
    static $margin = 6;
    
    static function quick_download( $table, $title = "Adresstickers" )
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
        
        // Paper (printable area) width/height
        $PW = $paper_size[0] - (2 * $MX);
        $PH = $paper_size[1] - (2 * $MY);
        
        // Block (sticker+margins) width/height
        $BW = ( $PW + $DX ) / $amt[0];
        $BH = ( $PH + $DY ) / $amt[1];
        // ( We need to add a virtual separator to the right, as there are $amt[0]-1 separators total. )
        
        // Sticker width/height
        $SW = $BW - $DX;
        $SH = $BH - $DY;
        
        // Line width/height
        $LW = $SW - 2*$padding;
        $LH = 1.07 * $fheight;
        
        
        $L = Adresstickers::get_layout( $table );
        $lines = count(explode("\n",$L));
        
        $Ifile = false;
        $IW = $IH = $IX = $IY = 0;
        if (( $Ifile = Config::get('sticker_logo_file') ) && ( file_exists($Ifile) ))
        {
            list($IW,$IH) = getimagesize($Ifile);
            // Determine if the aspect ratio of the image is smaller or larger
            // than that of the sticker
            
            if ( ($IW/$IH) > ($SW/$SH) )
            {
                // Logo should go in the bottom half
                $IH = $IH * ($SW/$IW);
                $IW = $SW;
                
                $IX = 0;
                $IY = $SH - $IH;
            }
            else
            {
                // Logo should go in the right half
                $IW = $IW * ($SH/$IH);
                $IH = $SH;
                
                $IX = $SW - $IW;
                $IY = 0;
            }
        }
        
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
                    if ( $Ifile )
                    {
                        // Insert logo watermark
                        $p->Image( $Ifile,
                            $MX + ($j * $BW) + $IX,
                            $MY + ($i * $BH) + $IY,
                            $IW, $IH
                        );
                    }
                    
                    $X = $MX + ($j * $BW) + $padding;
                    $Y = $MY + ($i * $BH) + $padding + ($SH-$lines*$fheight*1.3)/2;
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

Logo size:  {$IW}x{$IH}mm
Logo origin:  [{$IX},{$IY}]

Paper (printable area) width/height:  {$PW}x{$PH}mm
Block (sticker+margins) width/height:  {$BW}x{$BH}mm
Sticker width/height:  {$SW}x{$SH}mm
Line width/height:  {$LW}x{$LH}mm


Eccenticities:  {$ex[0]}.
Critical:  {$ex[1]}.
EOT;
        $p->AddPage();
        $p->SetXY(20,30);
        $p->MultiCell( 150, 1.4*$fheight, $debug_page, 0, 'L' );
        
        $p->SetXY(1,1);
        $p->Cell( $paper_size[0]-2, $paper_size[1]-2, "", 1 );
        
        
        $p->Output( $title, 'I' );
        
        exit;
    }
    
    static function get_layout( $table = array() )
    {
        // TODO: in $_REQUEST zoeken
        $S = "{naam} {titel} {voornaam} {tussenvoegsel} {achternaam}\n{adres}\n{postcode} {plaats}";
        
        if ( count($table) == 0 ) { return preg_replace( '/\{([^>]+)\}/', '<\1>', $S ); }
        
        $keys = array_keys( $table[0] );
        foreach ( $keys as $k )
        {
            $S = str_ireplace( '{'.$k.'}', '<'.$k.'>', $S );
        }
        $S = preg_replace( '/\{[^>]+\}/', '', $S );
        
        return $S;
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




class StickerTemplate
{
    public $margins;
    public $amount;
    public $unit;
    public $paper_size;
    public $orientation;
    
    function __construct( $margins, $amount, $font = array('family'=>'Times','size'=>11), $paper_size = array(210,297), $orientation = 'P', $unit = 'mm' )
    {
        $this->unit = $unit;
        $this->orientation = $orientation;
        $this->paper_size = $paper_size;
        $this->amount = $amount;
        
        $this->font = new stdClass();
        $this->font->family = $font['family'];
        $this->font->size   = $font['size'];
        
        $this->margins = new stdClass();
        $this->margins->left    = $margins['left'];
        $this->margins->top     = $margins['top'];
        $this->margins->hskip   = $margins['hskip'];
        $this->margins->vskip   = $margins['vskip'];
        $this->margins->padding = $margins['padding'];
        $this->margins->page    = @$margins['page'] or 6;
    }
    
    static function get( $name )
    {
        switch( $name )
        {
            case ( 'HEMA' ):
            default:
                return new StickerTemplate( array('left'=>7,'top'=>15,'hskip'=>2.5,'vskip'=>0,'padding'=>3), array(3,7) );
        }
    }
}




