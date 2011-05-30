<?php

require_once( "inc/db.inc" );

class Query
{
    function __construct( $query_file=false )
    {
        if ( file_exists( $query_file ) )
        {
            $this->load_from_xml( $query_file );
        }
        else
        {
            throw new Exception( "File [$query_file] does not exist." );
        }
    }
    private $_file = "";
    
    private $_texts;
    private $_parameters;
    
    private $_Title = 'Naamloze query';
    private $_Description = 'Een query zonder naam. Geen idee wat hij doet.';
    private $_Author = 'J.S. Bach';
    private $_Category = 'Overig';
    
    function execute()
    {
        $comm = new Command( $this->_texts, DB::ro(), array_keys($this->_parameters) );
        
        #TODO: Iets met parameters.
        return $comm->execute(array());
    }
    
    
    private function load_from_xml( $file )
    {
        $this->_file = $file;
        $doc = new DOMDocument;
        $doc->load( $file );
        
        $this->load_header_from_xml( AP::attr_node($doc,"Header") );
        $this->load_parameters_from_xml( AP::attr_node($doc,"Parameters") );
        $this->load_body_from_xml( AP::attr_node($doc,"Body") );
    }
    private function load_header_from_xml( $header_node )
    {
        if ( ! $header_node ) { return; }
        
        $title_node    = AP::attr_node( $header_node, "Title" );
        $desc_node     = AP::attr_node( $header_node, "Description" );
        $author_node   = AP::attr_node( $header_node, "Author" );
        $category_node = AP::attr_node( $header_node, "Category" );
        
        if ( $title_node )    { $this->_Title       = $title_node->textContent; }
        if ( $desc_node )     { $this->_Description = $desc_node->textContent; }
        if ( $author_node )   { $this->_Author      = $author_node->textContent; }
        if ( $category_node ) { $this->_Category    = $category_node->textContent; }
    }
    private function load_parameters_from_xml( $parameters_node )
    {
        $this->_parameters = array();
        if ( ! $parameters_node ) { return; }
        foreach ( $parameters_node->childNodes as $param )
        {
            if ( ! is_a( $param, 'DOMElement' ) ) { continue; }
            
            $paramtype = AP::first_child( $param );
            if ( ! $paramtype ) { throw new Exception( "Parameter ".$param->getAttribute('name')." heeft geen type." ); }
            $P = array( 'type' => $paramtype->nodeName );
            
            $this->_parameters[$param->getAttribute('Name')] = $P;
        }
    }
    private function load_body_from_xml( $body_node )
    {
        if ( ! $body_node ) { throw new Exception( "Invalid Query: Must have a body." ); }
        
        foreach ( $body_node->childNodes as $cn )
        {
            if ( ! is_a($cn,"DOMElement") ) { continue; }
            if ( $cn->nodeName != "Text" ) { continue; }
            if ( $cn->hasAttribute("Name") )
            {
                $this->_texts[$cn->getAttribute("Name")] = $cn->textContent;
            }
        }
    }
    
    function __get( $name )
    {
        switch ( $name )
        {
            case "Title":
                return $this->_Title;
            case "Description":
                return $this->_Description;
            case "Author":
                return $this->_Author;
            case "Category":
                return $this->_Category;
            case "file":
                return $this->_file;
            case "Parameters":
                return $this->_parameters;
        }
        throw new Exception( "Onbekend lid [{$name}]." );
    }
    
    
    static function queries_in_dir( $dir )
    {
        $dh = opendir( $dir );
        if ( ! $dh ) { return array(); }
        
        $rv = array();
        
        while ( $fn = readdir($dh) )
        {
            if (( strlen($fn) > 6 ) && ( substr($fn, -6) == '.query' ))
            {
                try
                {
                    $nq = new Query( $dir . "/" . $fn );
                    $rv[] = $nq;
                }
                catch(Exception $e) {}
            }
        }
        
        return $rv;
    }
    static function sort_query_list( $list )
    {
        function cmp($a,$b)
        {
            return strcmp($a->Title, $b->Title);
        }
        usort($list,"cmp");
        $rv = array();
        foreach ( $list as $q )
        {
            if ( ! isset($rv[$q->Category]) )
            {
                $rv[$q->Category] = array( $q );
            }
            else
            {
                $rv[$q->Category][] = $q;
            }
        }
        return $rv;
    }
    static function query_from_file()
    {
        if ( !isset($_GET["query"]) ) { return false; }
        $A = explode( '/', $_GET["query"] );
        if ( count($A) != 2 ) { return false; }
        switch ( $A[0] )
        {
            case Config::$query_dir:
            case 'standaard':
                if ( file_exists( Config::$query_dir . "/" . $A[1]) )
                {
                    return new Query( Config::$query_dir . "/" . $A[1] );
                }
                return false;
            case 'persoonlijk':
                throw new Exception( "Niet geïmplementeerd." );
        }
        throw new Exception( "Ongeldig pad." );
    }
}





class AP
{
    # Short for "Antipatterns"
    static function attr_node( DOMNode $doc, $nodename )
    {
        $rv = $doc->getElementsByTagName( $nodename );
        if ( $rv->length == 0 )
        {
            return false;
        }
        if ( $rv->length > 1 )
        {
            if ( ini_get("display_errors") > 0 )
            {
                print( "NOTICE: Found multiple [$nodename]s!" );
            }
        }
        return $rv->item(0);
    }
    static function first_child( DOMElement $elt )
    {
        foreach ( $elt->childNodes as $cn )
        {
            if ( ! is_a( $cn, 'DOMElement' ) ) { continue; }
            return $cn;
        }
        return null;
    }
}




