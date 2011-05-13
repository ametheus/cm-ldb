<?php

require_once( "auth.inc" );

if ( ! Auth::check() )
{
    Auth::deny_access( "Authorisatie ging grandioos mis." );
    exit;
}

