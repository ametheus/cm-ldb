<?php

class Auth
{
    static function deny_access( $why = "Verscheidene redenen" )
    {
        header( "HTTP/1.1 401 Unauthorized" );
        
        global $Why;
        $Why = $why;
        include( "error/auth.html" );
        exit;
    }
    
    /**
     * Check user privileges.
     * @return int Privilege level 1-4, where 4 is the highest.
     */
    static function check()
    {
        if ( ! isset($_SERVER["REMOTE_USER"]) )
        {
            Auth::deny_access( "No remote user set." );
        }
        
        // TODO: Implement layered privileges.
        return 3;
    }
}



