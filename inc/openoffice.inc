<?php

class Openoffice
{
    static function SpreadSheet( $result )
    {
        header("Content-type: text/plain" );
        print( "Een echt .ods-bestand gaat nu even niet lukken.\n".
            "In plaats daarvan krijg je een lijstje tabellen: \n\n".
            print_r(array_keys($result),true));
    }
}


