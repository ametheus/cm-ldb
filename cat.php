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
    Concatenate files
*/



$files = array(
    'js/admina' => array(
        "js/jquery-1.5.1.min.js",
        "js/jquery-ui-1.8.1.min.js",
        "js/jquery.tipsy.js",
        "js/jquery.treeview.min.js",
        "js/jquery.cookie.js",
        "js/jquery.lightbox-0.5.min.js",
        "js/jquery.wysiwyg.js",
        "js/functions.js",
        
        "js/PersonPicker.js"
    ),
    'css/admina' => array(
        # CSS - Setup
        "css/style.css",
        "css/grid.css",
        "css/jquery-ui.base.css",
        # CSS - Styles
        "css/base.css",
        "css/forms.css",
        "css/lists.css",
        "css/calendar.css",
        "css/extensions.css"
    ),
    
    'js/bewerken' => array(
        //"js/jquery.js",
        //"js/jquery-ui.js",
        "js/naam.js",
        "js/SpecialLabelContainer.js",
        //"js/PersonPicker.js",
        "js/SGR.js",
        "js/edit-people.js",
        "js/json2-min.js"
    )
);




$mime = array(
    'js' => 'application/javascript',
    'css' => 'text/css'
);

if ( ! preg_match( '/^\/(.*)(-[0-9]+)?\.(\w{2,5})$/', $_SERVER["REQUEST_URI"], $A ) )
{
    header( "Content-type: text/plain" );
    die( "/* Unable to concatenate files */" );
}

if ( isset( $mime[$A[3]] ) )
{
    header( "Content-type: " . $mime[$A[3]] );
}
else
{
    header( "Content-type: text/plain" );
}



$cat = @$files[$A[1]];
if ( ! $cat ) { $cat = array(); }

foreach ( $cat as $file )
{
    if ( file_exists( $file ) )
    {
        readfile( $file );
        print( str_repeat("\n",10) );
    }
}

