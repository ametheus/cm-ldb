<?php

require_once( "inc/db.inc" );

class Openoffice
{
    static function SpreadSheet( $result )
    {
        header("Content-type: text/plain" );
        print( "Een echt .ods-bestand gaat nu even niet lukken.\n".
            "In plaats daarvan krijg je het metabestand: \n\n".
            Openoffice::meta_file() );
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
    
    private static function mimetype_file()
    {
        return 'application/vnd.oasis.opendocument.spreadsheet';
    }
    
    private static function meta_file()
    {
        $rv = "";
        
        $rv .= "<office:document-meta office:version=\"1.2\" grddl:transformation=\"http://docs.oasis-open.org/office/1.2/xslt/odf2rdf.xsl\">\n";
        $rv .= "\t<office:meta>\n";
        # TODO: Gebruikersnaam
        $rv .= "\t\t<meta:initial-creator>".'De database'."</meta:initial-creator>\n";
        $rv .= "\t\t<dc:creator>".'De database'."</dc:creator>\n";
        $rv .= "\t\t<meta:creation-date>".date('c')."</meta:creation-date>\n";
        $rv .= "\t\t<dc:date>".date('c')."</dc:date>\n";
        $rv .= "\t\t<meta:editing-duration>PT0M1S</meta:editing-duration><!-- Probably even shorter. -->\n";
        $rv .= "\t\t<meta:editing-cycles>1</meta:editing-cycles>\n";
        $rv .= "\t\t<meta:generator>" . Config::full_app() . "</meta:generator>\n";
        $rv .= "\t</office:meta>\n";
        $rv .= "</office:document-meta>\n";
        
        return $rv;
    }
}


