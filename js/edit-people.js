
var selectedIndex = 0;
var IDs = null;
var Changes = {};
var Details = {};

$(function()
{
    $('#EditPeople').addClass('loading');
    
    $('#IDNav-fist').click(function(){select_index(0);});
    $('#IDNav-back').click(function(){select_index(selectedIndex-1);});
    $('#IDNav-next').click(function(){select_index(selectedIndex+1);});
    $('#IDNav-last').click(function(){select_index(IDs.length);});
    
    $('#IDNav-save').click(save);
    
    $('#EditPeople input').change(function()
    {
        Details[pid()] = get_new_details();
        Changes[pid()] = true;
        $('#EditPeople').addClass('modified');
    });
    
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
    select_this();
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
    
    if ( pid() in Details )
    {
        $('#EditPeople').removeClass('loading');
        update_fields();
    }
    else
    {
        $('#EditPeople').addClass('loading');
        load_details( pid() );
    }
    if ( pid() in Changes )
    {
        $('#EditPeople').addClass('modified');
    }
    else
    {
        $('#EditPeople').removeClass('modified');
    }
}

function load_details( pers_id )
{
    $.ajax({
        type: 'POST',
        url: '/bewerken/json/details',
        data: {'pers_id': pers_id},
        success: function(data)
        {
            //var pid;
            var update = !( pid() in Details );
            for ( var i = 0; i < data.length; i++ )
            {
                r = data[i];
                p = r['pers_id'];
                if ( ! ( p in Details ))
                {
                    Details[p] = r;
                }
            }
            if (( update ) && ( pid() in Details ))
            {
                update_fields();
            }
        },
        dataType: 'json'
    });
}

function update_fields()
{
    if ( ! ( pid() in Details )) { alert("not anymore."); return; }
    var d = Details[pid()];
    
    $('#Field-Achternaam')       .val(d['achternaam']);
    $('#Field-Aangetrouwdenaam') .val(d['aangetrouwdenaam']);
    $('#Field-Voorletters')      .val(d['voorletters']);
    $('#Field-Voornaam')         .val(d['voornaam']);
    $('#Field-Tussenvoegsel')    .val(d['tussenvoegsel']);
    $('#Field-Titel')            .val(d['titel']);
    $('#Field-Geslacht')         .val(d['geslacht']);
    $('#Field-Adres')            .val(d['adres']);
    $('#Field-Postcode')         .val(d['postcode']);
    $('#Field-Plaats')           .val(d['plaats']);
    $('#Field-Land')             .val(d['land']);
    $('#Field-Post')             .val(d['post']);
    $('#Field-Telefoon')         .val(d['telefoon']);
    $('#Field-Mobiel')           .val(d['mobiel']);
    $('#Field-Email')            .val(d['email']);
    $('#Field-Geboortedatum')    .val(d['geboortedatum']);
    $('#Field-Sterftedatum')     .val(d['sterftedatum']);
    $('#Field-Banknummer')       .val(d['banknummer']);
    $('#Field-Tenaamstelling')   .val(d['tenaamstelling']);
    $('#Field-Nationaliteit')    .val(d['nationaliteit']);
    $('#Field-Voorkeurstaal')    .val(d['voorkeurstaal']);
    $('#Field-Opm')              .val(d['opm']);
    
    $('#EditPeople').removeClass('loading');
}

function get_new_details()
{
    var d = {};
    d['pers_id']            = pid();
    d['achternaam']         = $('#Field-Achternaam').val();
    d['aangetrouwdenaam']   = $('#Field-Aangetrouwdenaam').val();
    d['voorletters']        = $('#Field-Voorletters').val();
    d['voornaam']           = $('#Field-Voornaam').val();
    d['tussenvoegsel']      = $('#Field-Tussenvoegsel').val();
    d['titel']              = $('#Field-Titel').val();
    d['geslacht']           = $('#Field-Geslacht').val();
    d['adres']              = $('#Field-Adres').val();
    d['postcode']           = $('#Field-Postcode').val();
    d['plaats']             = $('#Field-Plaats').val();
    d['land']               = $('#Field-Land').val();
    d['post']               = $('#Field-Post').val();
    d['telefoon']           = $('#Field-Telefoon').val();
    d['mobiel']             = $('#Field-Mobiel').val();
    d['email']              = $('#Field-Email').val();
    d['geboortedatum']      = $('#Field-Geboortedatum').val();
    d['sterftedatum']       = $('#Field-Sterftedatum').val();
    d['banknummer']         = $('#Field-Banknummer').val();
    d['tenaamstelling']     = $('#Field-Tenaamstelling').val();
    d['nationaliteit']      = $('#Field-Nationaliteit').val();
    d['voorkeurstaal']      = $('#Field-Voorkeurstaal').val();
    d['opm']                = $('#Field-Opm').val();
    
    return d;
}


function save()
{
    for ( i in Changes )
    {
        $.ajax({
            type: 'POST',
            url: '/bewerken/json/wijzig',
            data: Details[i],
            success: function(data)
            {
                delete Changes[i];
                alert(data);
                if ( pid() == i )
                {
                    $('#EditPeople').removeClass('modified');
                }
            },
            dataType: 'html'
        });
    }
}

