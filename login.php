<?php

require_once( "inc/auth.inc" );

if ( Auth::check() )
{
    Auth::redirect_login_page();
}

die( "<a href=\"/\">Interessant. Deze boodschap hoor je nooit te kunnen zien.</a>" );


