<?php

class Auth
{
    private static function login_page()
    {
        return ( parse_url($_SERVER['REQUEST_URI'],PHP_URL_PATH) == "/login" );
    }
    
    static function deny_access( $why = false, $error_code = false )
    {
        header( "HTTP/1.1 401 Unauthorized" );
        
        if ( ! Auth::login_page() )
        {
            header( "Location: /login?continue=" . urlencode($_SERVER["REQUEST_URI"]) );
            
            # Softfail for if the UA displays the page anyway instead of redirecting.
            $_REQUEST["continue"] = $_GET["continue"] = $_SERVER["REQUEST_URI"];
        }
        
        global $Why, $errcode;
        $Why = $why;
        $errcode = ( $error_code === false ? 'me' : $error_code );
        include( "error/auth.html" );
        exit;
    }
    
    /**
     * Check user privileges.
     * @return int Privilege level 1-4, where 4 is the highest.
     */
    static function check()
    {
        if ( session_id() == "" )
        {
            Auth::deny_access( false );
        }
        else
        {
            session_start();
            
            // TODO: Verify session signature.
            
            
            // TODO: Implement layered, less hard-coded privileges.
            if ( $_SESSION['e-mail'] == 'nerdcie@collegiummusicum.nl' ) { return 4; }
            if ( $_SESSION['e-mail'] == 'bestuur@collegiummusicum.nl' ) { return 3; }
            if ( substr($_SESSION['e-mail'],-1*strlen('@collegiummusicum.nl')) == '@collegiummusicum.nl'  ) { return 1; }
        }
        
        return 0;
    }
}



