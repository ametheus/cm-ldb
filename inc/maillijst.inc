<?php


class Maillijst
{
    static function gmail_to_link( $to, $subject=false, $cc=false, $bcc=false, $body=false )
    {
        $rv = 'https://mail.google.com/a/collegiummusicum.nl/?view=cm&fs=1&tf=1&source=mailto&to=' . urlencode($to);
        if ( $subject !== false ) { $rv .= '&subject=' . urlencode($subject); }
        if ( $cc      !== false ) { $rv .= '&cc='      . urlencode($cc); }
        if ( $bcc     !== false ) { $rv .= '&bcc='     . urlencode($bcc); }
        if ( $body    !== false ) { $rv .= '&body='    . urlencode($body); }
        
        if ( strlen($rv) > 2000 )
        {
            throw new Exception( "Gmail-URL [{$rv}] te lang." );
        }
        
        return $rv;
    }
    
    static function klembordknop( $id, $clip, $style = array() )
    {
        $URL = Maillijst::gmail_to_link('',false,false,'Plak hier de maillijst',false);
        
        $id   = mysql_escape_string( $id   );
        $clip = mysql_escape_string( $clip );
        $URL  = mysql_escape_string( $URL  );
        ?>
        <script type="text/javascript">
        $(function(){
            if ( typeof(ZeroClipboard) == 'undefined' )
            {
                alert('ZeroClipboard niet gevonden.');
                return;
            }
            
            ZeroClipboard.setMoviePath( '/img/ZeroClipboard10.swf' );
            
            var clippy = new ZeroClipboard.Client();
            clippy.glue( '<?=$id?>', undefined, <?=json_encode($style,JSON_FORCE_OBJECT)?> );
            clippy.setText( '<?=$clip?>' );
            
            clippy.addEventListener( 'complete', function(client, text) {
                window.open( '<?=$URL?>' );
            } );
        });
        </script><?php
    }
    
    static function maak_lijst( $table )
    {
        $ml = "";
        if ( count($table) > 0 )
        {
            $ec = array_key_exists( "email", $table[0] ) ? 'email' : 'Email';
            if ( ! array_key_exists( $ec, $table[0] ) )
            {
                throw new Exception( "Dit is volgens mij geen maillijst: [" .
                    implode(']-[',array_keys($table[0])) . ']' );
            }
            foreach ( $table as $row )
            {
                if ( ! preg_match(EMAIL_REGEX,$row[$ec]) ) { continue; }
                
                $em = $row[$ec];
                unset( $row[$ec] );
                $ml .= ",\n\"" . implode( " ", $row ) . "\" <" . $em . ">";
            }
            $ml = substr($ml,2);
        }
        return $ml;
    }
}




