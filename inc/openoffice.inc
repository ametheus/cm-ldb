<?php

require_once( "inc/db.inc" );

class Openoffice
{
    static function SpreadSheet( $result )
    {
        header("Content-type: text/plain" );
        print( "Een echt .ods-bestand gaat nu even niet lukken.\n".
            "In plaats daarvan krijg je het [settings.xml]-bestand: \n\n".
            Openoffice::settings_file( $result ) );
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
    
    private static function settings_file( $result )
    {
        $rv = "";
        
        $rv .= "<office:document-settings xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\" office:version=\"1.2\" xmlns:ooo=\"http://openoffice.org/2004/office\" xmlns:config=\"urn:oasis:names:tc:opendocument:xmlns:config:1.0\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"> \n";
        $rv .= "  <office:settings> \n";
        $rv .= "    <config:config-item-set config:name=\"ooo:view-settings\"> \n";
        $rv .= "      <config:config-item config:name=\"VisibleAreaTop\" config:type=\"int\">0</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"VisibleAreaLeft\" config:type=\"int\">0</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"VisibleAreaWidth\" config:type=\"int\">6774</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"VisibleAreaHeight\" config:type=\"int\">1799</config:config-item> \n";
        $rv .= "      <config:config-item-map-indexed config:name=\"Views\"> \n";
        $rv .= "        <config:config-item-map-entry> \n";
        $rv .= "          <config:config-item config:name=\"ViewId\" config:type=\"string\">view1</config:config-item> \n";
        $rv .= "          <config:config-item-map-named config:name=\"Tables\"> \n";
        
        $active_sheet = false;
        foreach ( $result as $name => $sheet )
        {
            if ( $active_sheet === false ) { $active_sheet = $name; }
            
            $rv .= "            <config:config-item-map-entry config:name=\"{$name}\"> \n";
            $rv .= "              <config:config-item config:name=\"CursorPositionX\" config:type=\"int\">0</config:config-item> \n";
            $rv .= "              <config:config-item config:name=\"CursorPositionY\" config:type=\"int\">1</config:config-item> \n";
            $rv .= "              <config:config-item config:name=\"HorizontalSplitMode\" config:type=\"short\">0</config:config-item> \n";
            $rv .= "              <config:config-item config:name=\"VerticalSplitMode\" config:type=\"short\">2</config:config-item> \n";
            $rv .= "              <config:config-item config:name=\"HorizontalSplitPosition\" config:type=\"int\">0</config:config-item> \n";
            $rv .= "              <config:config-item config:name=\"VerticalSplitPosition\" config:type=\"int\">1</config:config-item> \n";
            $rv .= "              <config:config-item config:name=\"ActiveSplitRange\" config:type=\"short\">2</config:config-item> \n";
            $rv .= "              <config:config-item config:name=\"PositionLeft\" config:type=\"int\">0</config:config-item> \n";
            $rv .= "              <config:config-item config:name=\"PositionRight\" config:type=\"int\">0</config:config-item> \n";
            $rv .= "              <config:config-item config:name=\"PositionTop\" config:type=\"int\">0</config:config-item> \n";
            $rv .= "              <config:config-item config:name=\"PositionBottom\" config:type=\"int\">1</config:config-item> \n";
            $rv .= "              <config:config-item config:name=\"ZoomType\" config:type=\"short\">0</config:config-item> \n";
            $rv .= "              <config:config-item config:name=\"ZoomValue\" config:type=\"int\">100</config:config-item> \n";
            $rv .= "              <config:config-item config:name=\"PageViewZoomValue\" config:type=\"int\">60</config:config-item> \n";
            $rv .= "              <config:config-item config:name=\"ShowGrid\" config:type=\"boolean\">true</config:config-item> \n";
            $rv .= "            </config:config-item-map-entry> \n";
        }
        $rv .= "          </config:config-item-map-named> \n";
        $rv .= "          <config:config-item config:name=\"ActiveTable\" config:type=\"string\">{$active_sheet}</config:config-item> \n";
        $rv .= "          <config:config-item config:name=\"HorizontalScrollbarWidth\" config:type=\"int\">270</config:config-item> \n";
        $rv .= "          <config:config-item config:name=\"ZoomType\" config:type=\"short\">0</config:config-item> \n";
        $rv .= "          <config:config-item config:name=\"ZoomValue\" config:type=\"int\">100</config:config-item> \n";
        $rv .= "          <config:config-item config:name=\"PageViewZoomValue\" config:type=\"int\">60</config:config-item> \n";
        $rv .= "          <config:config-item config:name=\"ShowPageBreakPreview\" config:type=\"boolean\">false</config:config-item> \n";
        $rv .= "          <config:config-item config:name=\"ShowZeroValues\" config:type=\"boolean\">true</config:config-item> \n";
        $rv .= "          <config:config-item config:name=\"ShowNotes\" config:type=\"boolean\">true</config:config-item> \n";
        $rv .= "          <config:config-item config:name=\"ShowGrid\" config:type=\"boolean\">true</config:config-item> \n";
        $rv .= "          <config:config-item config:name=\"GridColor\" config:type=\"long\">12632256</config:config-item> \n";
        $rv .= "          <config:config-item config:name=\"ShowPageBreaks\" config:type=\"boolean\">true</config:config-item> \n";
        $rv .= "          <config:config-item config:name=\"HasColumnRowHeaders\" config:type=\"boolean\">true</config:config-item> \n";
        $rv .= "          <config:config-item config:name=\"HasSheetTabs\" config:type=\"boolean\">true</config:config-item> \n";
        $rv .= "          <config:config-item config:name=\"IsOutlineSymbolsSet\" config:type=\"boolean\">true</config:config-item> \n";
        $rv .= "          <config:config-item config:name=\"IsSnapToRaster\" config:type=\"boolean\">false</config:config-item> \n";
        $rv .= "          <config:config-item config:name=\"RasterIsVisible\" config:type=\"boolean\">false</config:config-item> \n";
        $rv .= "          <config:config-item config:name=\"RasterResolutionX\" config:type=\"int\">1270</config:config-item> \n";
        $rv .= "          <config:config-item config:name=\"RasterResolutionY\" config:type=\"int\">1270</config:config-item> \n";
        $rv .= "          <config:config-item config:name=\"RasterSubdivisionX\" config:type=\"int\">1</config:config-item> \n";
        $rv .= "          <config:config-item config:name=\"RasterSubdivisionY\" config:type=\"int\">1</config:config-item> \n";
        $rv .= "          <config:config-item config:name=\"IsRasterAxisSynchronized\" config:type=\"boolean\">true</config:config-item> \n";
        $rv .= "        </config:config-item-map-entry> \n";
        $rv .= "      </config:config-item-map-indexed> \n";
        $rv .= "    </config:config-item-set> \n";
        $rv .= "    <config:config-item-set config:name=\"ooo:configuration-settings\"> \n";
        $rv .= "      <config:config-item config:name=\"AllowPrintJobCancel\" config:type=\"boolean\">true</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"IsKernAsianPunctuation\" config:type=\"boolean\">false</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"LinkUpdateMode\" config:type=\"short\">3</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"PrinterSetup\" config:type=\"base64Binary\" /> \n";
        $rv .= "      <config:config-item config:name=\"RasterSubdivisionX\" config:type=\"int\">1</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"ShowZeroValues\" config:type=\"boolean\">true</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"RasterSubdivisionY\" config:type=\"int\">1</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"ApplyUserData\" config:type=\"boolean\">true</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"HasSheetTabs\" config:type=\"boolean\">true</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"ShowPageBreaks\" config:type=\"boolean\">true</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"RasterResolutionX\" config:type=\"int\">1270</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"RasterResolutionY\" config:type=\"int\">1270</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"GridColor\" config:type=\"long\">12632256</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"LoadReadonly\" config:type=\"boolean\">false</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"ShowNotes\" config:type=\"boolean\">true</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"RasterIsVisible\" config:type=\"boolean\">false</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"IsSnapToRaster\" config:type=\"boolean\">false</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"PrinterName\" config:type=\"string\" /> \n";
        $rv .= "      <config:config-item config:name=\"ShowGrid\" config:type=\"boolean\">true</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"CharacterCompressionType\" config:type=\"short\">0</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"HasColumnRowHeaders\" config:type=\"boolean\">true</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"IsOutlineSymbolsSet\" config:type=\"boolean\">true</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"IsDocumentShared\" config:type=\"boolean\">false</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"IsRasterAxisSynchronized\" config:type=\"boolean\">true</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"AutoCalculate\" config:type=\"boolean\">true</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"SaveVersionOnClose\" config:type=\"boolean\">false</config:config-item> \n";
        $rv .= "      <config:config-item config:name=\"UpdateFromTemplate\" config:type=\"boolean\">true</config:config-item> \n";
        $rv .= "    </config:config-item-set> \n";
        $rv .= "  </office:settings> \n";
        $rv .= "</office:document-settings>";
        
        return $rv;
    }
}


