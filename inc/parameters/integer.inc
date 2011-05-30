<?php
namespace Parameters;


class Integer extends Base
{
    function form( $name, $additional_data, $value )
    {
        $v = ( is_numeric($value) ? $value : @$additional_data["default"] );
        $v = ( is_numeric($v) ? $v : 0 );
        
        $min = ( is_numeric(@$additional_data["Min"]) ? "min={$additional_data["Min"]}" : "" );
        $max = ( is_numeric(@$additional_data["Max"]) ? "max={$additional_data["Max"]}" : "" );
        
        ?>
        <p>
            <label for="parameter_<?=$name?>"><?=$name?></label>
            <input type="number" step="1" <?=$min?> <?=$max?> name="<?=$name?>" id="parameter_<?=$name?>" value="<?=$v?>" />
        </p>
        <?php
    }
}


\QueryParameters::add_type( "Integer", new Integer() );


# Plak hier CSS of javascript, in de daartoe bestemde tags.
