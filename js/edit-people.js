
var selectedIndex = 0;
var IDs = null;

$(function()
{
    $.ajax({
        url: '/bewerken/json/IDs',
        dataType: 'json',
        success: function(data)
        {
            IDs = data;
            if ( location.hash.length > 1 )
            {
                select_id( location.hash.substr(1) );
            }
            else
            {
                select_index( IDs.length-1 );
            }
        }
    });
})


function pid()
{
    return IDs[selectedIndex];
}

function select_index( index )
{
    if ( ! IDs ) { return; }
    if ( IDs.length == 0 ) { return; }
    if ( index >= IDs.length ) { index = IDs.length - 1; }
    if ( index < 0 ) { index = 0; }
    
    selectedIndex = index;
}

function select_id( pers_id )
{
    if ( ! IDs ) { return; }
    for ( var i = 0; i < IDs.length; i++ )
    {
        if ( IDs[i] == pers_id )
        {
            selectedIndex = i;
            select_this();
            return;
        }
    }
    select_index( IDs.length-1 );
}

function select_this()
{
    location.hash = pid();
    $('#IDNavigator input').val(pid());
}


