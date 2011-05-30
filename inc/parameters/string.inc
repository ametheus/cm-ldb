<?php


namespace Parameters;


class String extends Base
{
    function form( $name, $additional_data, $value )
    {
        $value_to_display = ( $value ? $value : @$additional_data['default'] );
        ?>
        <p>
            <label for="parameter_<?=$name?>"><?=$name?></label>
            <input type="text" name="<?=$name?>" value="<?=$value_to_display?>" />
        </p>
        <?php
    }
}



\QueryParameters::add_type( "String", new String() );



