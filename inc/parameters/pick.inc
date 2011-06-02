<?php
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
