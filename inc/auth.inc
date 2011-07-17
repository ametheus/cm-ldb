<?php

class Auth
{
    private static function login_page()
    {
        return ( parse_url($_SERVER['REQUEST_URI'],PHP_URL_PATH) == "/login" );
    }
    private static function logout_page()
    {
        return ( parse_url($_SERVER['REQUEST_URI'],PHP_URL_PATH) == "/logout" );
    }
    
    static function deny_access( $why = false, $error_code = false, $send_header = true )
    {
        if ( $send_header )
        {
            header( "HTTP/1.1 401 Unauthorized" );
        }
        
        if (( ! Auth::login_page() ) && $send_header )
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
     * @return int Privilege level 1-4, where 4 is the highest. Or 0.
     */
    static function check()
    {
        session_start();
        
        if ( Auth::logout_page() )
        {
            session_destroy();
            header( 'HTTP/1.1 302 Found' );
            header( 'Location: /' );
            
            Auth::deny_access( false, false, false );
        }
        elseif ( ! isset($_SESSION['e-mail']) )
        {
            if ( isset($_POST['username']) && isset($_POST['password']) )
            {
                $em = Auth::enticate( $_POST['username'], $_POST['password'] );
                if ( $em !== false )
                {
                    $_SESSION["e-mail"] = $em;
                    
                    // TODO: Generate session signature.
                    
                    //session_write_close();
                    
                    if ( Auth::login_page() && isset($_REQUEST["continue"]) )
                    {
                        header( "HTTP/1.1 302 Found" );
                        header( "Location: " . str_replace("\n","",$_REQUEST["continue"]) );
                    }
                    
                    return Auth::priv_level( $em );
                }
                else
                {
                    Auth::deny_access( "Ongeldige gebruikersnaam <br /> en/of wachtwoord." );
                    exit;
                }
            }
            
            Auth::deny_access( false );
        }
        else
        {
            // TODO: Verify session signature.
            
            
            return Auth::priv_level( $_SESSION['e-mail'] );
        }
        
        return 0;
    }
    
    private static function enticate( $username, $password )
    {
        require_once( "Net/SMTP.php" );
        
        $D = 'collegiummusicum.nl';
        
        if ( substr($username,-1*strlen("@$D")) == "@$D" )
        {
            $username = substr($username,0,-1*strlen("@$D"));
        }
        
        
        $SMTP = new Net_SMTP( 'ssl://smtp.gmail.com', 465 );
        
        $r = $SMTP->connect(3);
        if ( is_a( $r, "PEAR_Error" ) )
        {
            Auth::deny_access( "Verbinden met Google mislukt." );
            return false;
        }
        
        $r = $SMTP->auth( "{$username}@{$D}", $password );
        if ( is_a( $r, "PEAR_Error" ) )
        {
            Auth::deny_access( "Ongeldige gebruikersnaam <br /> en/of wachtwoord." );
            return false;
        }
        
        return "{$username}@{$D}";
    }
    
    /**
     * Check privileges of $email.
     * @return int Privilege level 1-4, where 4 is the highest. Or 0.
     */
    private static function priv_level( $email )
    {
        // TODO: Implement layered, less hard-coded privileges.
        $D = '@collegiummusicum.nl';
        
        if ( $email == 'nerdcie'.$D ) { return 4; }
        if ( $email == 'bestuur'.$D ) { return 3; }
        if ( substr($email,-1*strlen($D)) == $D  ) { return 1; }
        
        return 0;
    }
}



