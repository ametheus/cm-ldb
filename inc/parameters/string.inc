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
   String input.
*/

namespace Parameters;


class String extends Base
{
    function form( $name, $additional_data, $value )
    {
        $value_to_display = ( $value ? $value : @$additional_data['default'] );
        
        $T = ( strlen(@$additional_data["Title"])       > 0 ? $additional_data["Title"] : $name );
        $D = ( strlen(@$additional_data["Description"]) > 0 ? $additional_data["Description"] : $name );
        
        ?>
            <p>
                <label for="parameter_<?=$name?>"><?=$T?></label>
                <input type="text" name="<?=$name?>" value="<?=$value_to_display?>" title="<?=$D?>" />
            </p>
<?php
    }
}



\QueryParameters::add_type( "String", new String() );



