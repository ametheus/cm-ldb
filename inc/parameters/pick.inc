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
   Combobox input.
*/

namespace Parameters;


class Pick extends Base
{
    function form( $name, $additional_data, $value )
    {
        $value = ( $value ? $value : @$additional_data["default"] );
        
        $T = ( strlen(@$additional_data["Title"])       > 0 ? $additional_data["Title"] : $name );
        $D = ( strlen(@$additional_data["Description"]) > 0 ? $additional_data["Description"] : $name );
        
        $Options = $this->get_options( $additional_data );
        
        ?>
            <p>
                <label for="parameter_<?=$name?>"><?=$T?></label>
                <select name="<?=$name?>" id="parameter_<?=$name?>" title="<?=$D?>">
                    <option disabled="disabled" />
<?php
        foreach ( $Options as $k=>$v )
        {
            $k = htmlentities($k); $v = htmlentities($v);
            print( "                    <option value=\"{$k}\"" );
            if ( $value == $k ) { print( " selected=\"selected\"" ); }
            print( ">{$v}</option>\n" );
        }
?>
                </select>
            </p>
<?php
    }
    
    private function get_options( $additional_data )
    {
        $Options = array();
        if ( isset($additional_data['Subquery']) )
        {
            $C = new \Command( $additional_data['Subquery'], \DB::ro() );
            $O = $C->execute();
            if (( count($O) == 0 ) || ( count(array_keys($O[0])) < 2 ))
            {
                throw new Exception( "Invalid Subquery for pick [{$name}]." );
            }
            list($k,$v) = array_keys($O[0]);
            foreach ( $O as $r )
            {
                $Options[$r[$k]] = $r[$v];
            }
        }
        return $Options;
    }
}


\QueryParameters::add_type( "Pick", new Pick() );


# Plak hier CSS of javascript, in de daartoe bestemde tags.
