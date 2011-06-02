<?php
namespace Parameters;


class DateTime extends Base
{
    function form( $name, $additional_data, $value )
    {
        $v = ( preg_match( '/^\\d{4}-\\d{2}-\\d{2}/', $value ) ? $value : @$additional_data["default"] );
        
        $min = ( is_numeric(@$additional_data["Min"]) ? "min={$additional_data["Min"]}" : "" );
        $max = ( is_numeric(@$additional_data["Max"]) ? "max={$additional_data["Max"]}" : "" );
        
        $T = ( strlen(@$additional_data["title"]) > 0 ? $additional_data["title"] : $name );
        $D = ( strlen(@$additional_data["desc"])  > 0 ? $additional_data["desc"]  : $name );
        
        ?>
        <p>
            <label for="parameter_<?=$name?>"><?=$T?></label>
            <input type="date" <?=$min?> <?=$max?> name="<?=$name?>" id="parameter_<?=$name?>" value="<?=$v?>" title="<?=$D?>" />
        </p>
        <?php
    }
}


\QueryParameters::add_type( "DateTime", new DateTime() );


# Plak hier CSS of javascript, in de daartoe bestemde tags.
