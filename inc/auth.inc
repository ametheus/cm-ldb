<?php

class Auth
{
    static function deny_access( $why = "Verscheidene redenen" )
    {
        header( "HTTP/1.1 401 Unauthorized" );
        
        if ( parse_url($_SERVER['REQUEST_URI'],PHP_URL_PATH) != "/login" )
        {
            header( "Location: /login?continue=" . urlencode($_SERVER["REQUEST_URI"]) );
            
            # Softfail for if the UA displays the page anyway instead of redirecting.
            $_REQUEST["continue"] = $_GET["continue"] = $_SERVER["REQUEST_URI"];
        }
        
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
            Auth::deny_access( false );
        }
        
        // TODO: Implement layered privileges.
        return 3;
    }
}



