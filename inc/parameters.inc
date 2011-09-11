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
   Base class for query inputs
*/


require_once( 'inc/db.inc' );
require_once( 'inc/parameters/base.inc' );

class QueryParameters
{
    static function protected_param_names()
    {
        return array('query');
    }
    
    
    private static $_types = array();
    static function add_type( $name, Parameters\Base $pc )
    {
        # TODO: Iets met duplicaten.
        QueryParameters::$_types[ $name ] = $pc;
    }
    
    
    
    static function process( Query $Q )
    {
        $params = $Q->Parameters;
        $ppn = QueryParameters::protected_param_names();
        
        if ( count($params) == 0 ) { return array(); }
        
        $rv = array();
        
        foreach ( $params as $name=>$paramspec )
        {
            if ( in_array($name,$ppn) )
            {
                throw new Exception( "Parameter name [{$name}] is reserved.  Query: " . $Q->file );
            }
            
            if ( array_key_exists($name, $_REQUEST) )
            {
                $rv[$name] = $_REQUEST[$name];
                continue;
            }
        }
        
        # Controleer of er genoeg informatie is
        if ( count($rv) == count($params) ) { return $rv; }
        
        # Zoniet, maak een mooi formuliertje voor elke parameter.
        ?><!DOCTYPE html>
<html>
    <head>
        <title><?=$Q->Title?></title>
        
        <link type="text/css" href="/css/admina.css" rel="stylesheet" />	
        <link type="text/css" href="/css/mock-grid.css" rel="stylesheet" />	
		<script type="text/javascript" src="/js/admina.js"></script>
<?php
        include('inc/parameters/string.inc');
        include('inc/parameters/integer.inc');
        include('inc/parameters/datetime.inc');
        include('inc/parameters/pick.inc');
?>
    </head>
    <body id="layout1280">
      <div id="page" class="width">
        <h1><?=$Q->Title?></h1>
        <br class="cl" />
        <form method="get" class="form" action="<?=$_SERVER["REQUEST_URI"]?>"
          <div class="mock_grid">

<?php
        foreach ( $params as $name=>$paramspec )
        {
            $handler = @QueryParameters::$_types[$paramspec['type']];
            if ( $handler )
            {
                $handler->form( $name, $paramspec, @$rv[$name] );
            }
            else
            {
                print( "<p>Ik heb een parameter [$name] gevonden van type [{$paramspec['type']}], ".
                       "en ik heb geen flauw idee wat ik er mee moet.</p>\n" );
            }
            print( "        <br class=\"cl\" />\n" );
        }
?>
            <p>
                <button id="submit" type="submit">Query uitvoeren</button>
                <script type="text/javascript">
                    $(function(){
                        $('#submit').click(function(){
                            $('form.form').submit();
                        });
                    });
                </script>
            </p>
          </div>
        </form>
      </div>
    </body>
</html><?php
        exit;
        
    }
}

