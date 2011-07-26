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
   Integer input
*/

namespace Parameters;


class Integer extends Base
{
    function form( $name, $additional_data, $value )
    {
        $v = ( is_numeric($value) ? $value : @$additional_data["default"] );
        $v = ( is_numeric($v) ? $v : 0 );
        
        $min = ( is_numeric(@$additional_data["Min"]) ? "min={$additional_data["Min"]}" : "" );
        $max = ( is_numeric(@$additional_data["Max"]) ? "max={$additional_data["Max"]}" : "" );
        
        $T = ( strlen(@$additional_data["Title"])       > 0 ? $additional_data["Title"] : $name );
        $D = ( strlen(@$additional_data["Description"]) > 0 ? $additional_data["Description"] : $name );
        
        ?>
            <p>
                <label for="parameter_<?=$name?>"><?=$T?></label>
                <input type="number" step="1" <?=$min?> <?=$max?> name="<?=$name?>" id="parameter_<?=$name?>" value="<?=$v?>" title="<?=$D?>" />
            </p>
<?php
    }
}


\QueryParameters::add_type( "Integer", new Integer() );


# Plak hier CSS of javascript, in de daartoe bestemde tags.
