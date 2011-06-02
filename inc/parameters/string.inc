<?php


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



