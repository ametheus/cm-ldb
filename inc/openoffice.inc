<?php

class Openoffice
{
    static function SpreadSheet( $result )
    {
        header("Content-type: text/plain" );
        print( "Een echt .ods-bestand gaat nu even niet lukken.\n".
            "In plaats daarvan krijg je het manifestbestand: \n\n".
            Openoffice::manifest_file( $result ) );
    }
    
    private static function manifest_file( $result )
    {
        $rv = "";
        
        $rv .= "<manifest:manifest>\n";
        $rv .= "\t<manifest:file-entry manifest:media-type=\"application/vnd.oasis.opendocument.spreadsheet\" manifest:version=\"1.2\" manifest:full-path=\"/\"/>\n";
        $rv .= "\t<manifest:file-entry manifest:media-type=\"text/xml\" manifest:full-path=\"content.xml\"/>\n";
        $rv .= "\t<manifest:file-entry manifest:media-type=\"text/xml\" manifest:full-path=\"styles.xml\"/>\n";
        $rv .= "\t<manifest:file-entry manifest:media-type=\"text/xml\" manifest:full-path=\"meta.xml\"/>\n";
        $rv .= "\t<manifest:file-entry manifest:media-type=\"text/xml\" manifest:full-path=\"settings.xml\"/>\n";
        $rv .= "</manifest:manifest>";
        
        return $rv;
    }
}


