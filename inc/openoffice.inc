<?php

require_once( "inc/db.inc" );

class Openoffice
{
    static function SpreadSheet( $result )
    {
        header("Content-type: text/plain" );
        print( "Een echt .ods-bestand gaat nu even niet lukken.\n".
            "In plaats daarvan krijg je een bestandsnaam: \n\n".
            Openoffice::create_zip( $result ) );
    }
    
    private static function create_zip( $result, $filename=false )
    {
        if ( $filename === false ) { $filename = tempnam( '/tmp', 'LDB_ods_' ); }
        
        // TODO: Load zip file with data.
        
        return $filename;
    }
    
    private static function manifest_file( $result )
    {
        $rv = "";
        
        $rv .= "<manifest:manifest>\n";
        $rv .= "\t<manifest:file-entry manifest:media-type=\"application/vnd.oasis.opendocument.spreadsheet\" manifest:version=\"1.2\" manifest:full-path=\"/\"/>\n";
        $rv .= "\t<manifest:file-entry manifest:media-type=\"text/xml\" manifest:full-path=\"content.xml\"/>\n";
        //$rv .= "\t<manifest:file-entry manifest:media-type=\"text/xml\" manifest:full-path=\"styles.xml\"/>\n";
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
        $rv .= "\t\t<meta:initial-creator>"    .    'De database'    .    "</meta:initial-creator>\n";
        $rv .= "\t\t<dc:creator>"   .               'De database'    .    "</dc:creator>\n";
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
    
    private static function content_file( $result )
    {
        $rv = "";
        
        $rv .= "<office:document-content xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\" xmlns:rpt=\"http://openoffice.org/2005/report\" grddl:transformation=\"http://docs.oasis-open.org/office/1.2/xslt/odf2rdf.xsl\" xmlns:math=\"http://www.w3.org/1998/Math/MathML\" xmlns:field=\"urn:openoffice:names:experimental:ooo-ms-interop:xmlns:field:1.0\" xmlns:ooo=\"http://openoffice.org/2004/office\" xmlns:form=\"urn:oasis:names:tc:opendocument:xmlns:form:1.0\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:of=\"urn:oasis:names:tc:opendocument:xmlns:of:1.2\" xmlns:dom=\"http://www.w3.org/2001/xml-events\" xmlns:oooc=\"http://openoffice.org/2004/calc\" xmlns:style=\"urn:oasis:names:tc:opendocument:xmlns:style:1.0\" xmlns:script=\"urn:oasis:names:tc:opendocument:xmlns:script:1.0\" xmlns:presentation=\"urn:oasis:names:tc:opendocument:xmlns:presentation:1.0\" xmlns:tableooo=\"http://openoffice.org/2009/table\" xmlns:css3t=\"http://www.w3.org/TR/css3-text/\" xmlns:grddl=\"http://www.w3.org/2003/g/data-view#\" xmlns:dr3d=\"urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0\" xmlns:text=\"urn:oasis:names:tc:opendocument:xmlns:text:1.0\" office:version=\"1.2\" xmlns:number=\"urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0\" xmlns:ooow=\"http://openoffice.org/2004/writer\" xmlns:xforms=\"http://www.w3.org/2002/xforms\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns:formx=\"urn:openoffice:names:experimental:ooxml-odf-interop:xmlns:form:1.0\" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\" xmlns:xhtml=\"http://www.w3.org/1999/xhtml\" xmlns:draw=\"urn:oasis:names:tc:opendocument:xmlns:drawing:1.0\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:meta=\"urn:oasis:names:tc:opendocument:xmlns:meta:1.0\" xmlns:svg=\"urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0\" xmlns:fo=\"urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0\" xmlns:chart=\"urn:oasis:names:tc:opendocument:xmlns:chart:1.0\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\"> \n";
        $rv .= "  <office:scripts /> \n";
        $rv .= "  <office:font-face-decls> \n";
        $rv .= "    <style:font-face svg:font-family=\"Arial\" style:font-family-generic=\"swiss\" style:font-pitch=\"variable\" style:name=\"Arial\" /> \n";
        $rv .= "    <style:font-face svg:font-family=\"&apos;DejaVu Sans&apos;\" style:font-family-generic=\"system\" style:font-pitch=\"variable\" style:name=\"DejaVu Sans\" /> \n";
        $rv .= "    <style:font-face svg:font-family=\"&apos;Droid Sans Fallback&apos;\" style:font-family-generic=\"system\" style:font-pitch=\"variable\" style:name=\"Droid Sans Fallback\" /> \n";
        $rv .= "    <style:font-face svg:font-family=\"&apos;Lohit Hindi&apos;\" style:font-family-generic=\"system\" style:font-pitch=\"variable\" style:name=\"Lohit Hindi\" /> \n";
        $rv .= "  </office:font-face-decls> \n";
        $rv .= "  <office:automatic-styles> \n";
        $rv .= "    <style:style style:name=\"co1\" style:family=\"table-column\"> \n";
        $rv .= "      <style:table-column-properties fo:break-before=\"auto\" style:column-width=\"0.8925in\" /> \n";
        $rv .= "    </style:style> \n";
        $rv .= "    <style:style style:name=\"ro1\" style:family=\"table-row\"> \n";
        $rv .= "      <style:table-row-properties fo:break-before=\"auto\" style:use-optimal-row-height=\"true\" style:row-height=\"0.178in\" /> \n";
        $rv .= "    </style:style> \n";
        $rv .= "    <style:style style:name=\"ro2\" style:family=\"table-row\"> \n";
        $rv .= "      <style:table-row-properties fo:break-before=\"auto\" style:use-optimal-row-height=\"true\" style:row-height=\"0.178in\" /> \n";
        $rv .= "      <style:text-properties fo:font-weight=\"bold\" style:font-weight-asian=\"bold\" style:font-weight-complex=\"bold\"/>\n";
        $rv .= "    </style:style> \n";
        $rv .= "    <style:style style:master-page-name=\"Default\" style:name=\"ta1\" style:family=\"table\"> \n";
        $rv .= "      <style:table-properties table:display=\"true\" style:writing-mode=\"lr-tb\" /> \n";
        $rv .= "    </style:style> \n";
        $rv .= "  </office:automatic-styles> \n";
        $rv .= "  <office:body> \n";
        $rv .= "    <office:spreadsheet> \n";
        
        foreach ( $result as $name => $sheet )
        {
            $rv .= "      <table:table table:style-name=\"ta1\" table:print=\"false\" table:name=\"{$name}\"> \n";
            
            $columns = ( count($sheet) > 0 ? array_keys($sheet[0]) : array() );
            
            $rv .= "        <table:table-column table:style-name=\"co1\" table:default-cell-style-name=\"Default\" table:number-columns-repeated=\"".count($columns)."\" /> \n";
            
            # Eerste rij met kolomnamen
            $rv .= "        <table:table-row table:style-name=\"ro2\"> \n";
            foreach ( $columns as $col )
            {
                $c = htmlspecialchars( $col );
                $rv .= "          <table:table-cell office:value-type=\"string\"> \n";
                $rv .= "            <text:p>{$c}</text:p> \n";
                $rv .= "          </table:table-cell> \n";
            }
            $rv .= "        </table:table-row> \n";
            
            foreach ( $sheet as $row )
            {
                $rv .= "        <table:table-row table:style-name=\"ro1\"> \n";
                foreach ( $row as $v )
                {
                    $v = htmlspecialchars( $v );
                    if ( strlen($v) > 0 )
                    {
                        $rv .= "          <table:table-cell office:value-type=\"string\"> \n";
                        $rv .= "            <text:p>{$v}</text:p> \n";
                        $rv .= "          </table:table-cell> \n";
                    }
                    else
                    {
                        $rv .= "          <table:table-cell />\n";
                    }
                }
                $rv .= "        </table:table-row> \n";
            }
            $rv .= "      </table:table> \n";
        }
        
        $rv .= "    </office:spreadsheet> \n";
        $rv .= "  </office:body> \n";
        $rv .= "</office:document-content> ";
        
        return $rv;
    }
}


