/*
    Copyright (C) 2011 Thijs van Dijk
    
    This file is part of CM-LDB.

    CM-LDB is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CM-LDB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CM-LDB.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
   JS for the edit page.
*/

var selectedIndex = 0;
var IDs = null;
var Changes = {};
var Details = {};

var searchTerm = '';
var searchIndex = 0;
var searches = {};

$(function()
{
    $('#EditPeople').addClass('loading');
    
    $('#IDNav-fist').click(function(){select_index(0);});
    $('#IDNav-back').click(function(){select_index(selectedIndex-1);});
    $('#IDNav-next').click(function(){select_index(selectedIndex+1);});
    $('#IDNav-last').click(function(){select_index(IDs.length);});
    
    PersonPicker.create( '#sidebar-pick', function( id ){
        select_id(id);
        $('#sidebar-pick').val('');
    });
    PersonPicker.create( '#RelatieEditor #pers_id_b', function(id){
        SGR.Relatie.actual_actual_pers_id = id;
    } );
    
    $('#sgr-accordion').accordion({ collapsible: true }).accordion( "activate", false );
    
    
    $('#IDNav-insert').click(insert_person);
    $('#IDNav-delete').click(delete_person);
    
    $('#IDNav-save').click(save);
    
    $('#EditPeople table input').keyup(function()
    {
        if ( ! any_changes() ) { return; }
        Details[pid()] = get_new_details();
        Changes[pid()] = true;
        $('#EditPeople').addClass('modified');
    });
    $('#IDNav-id').keyup(function()
    {
        select_id($('#IDNav-id').val());
    });
    
    reload();
});



function reload()
{
    $('#EditPeople').addClass('loading');
    IDs = null;
    
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
}



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
}

function select_this()
{
    location.hash = pid();
    $('#IDNav-id').val(pid());
    
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
    $('#Field-Geslacht')         .val(ucfirst(d['geslacht']));
    $('#Field-Adres')            .val(d['adres']);
    $('#Field-Postcode')         .val(d['postcode']);
    $('#Field-Plaats')           .val(d['plaats']);
    $('#Field-Land')             .val(d['land']);
    
    $('#Field-Post')             .attr('checked', (d['post'] == 'ja'));
    $('#Field-Post').change(); // HACK: Dit zou niet expliciet moeten!
    
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
    
    SGR.load( pid() );
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
    d['geboortedatum']      = nul($('#Field-Geboortedatum').val());
    d['sterftedatum']       = nul($('#Field-Sterftedatum').val());
    d['banknummer']         = $('#Field-Banknummer').val();
    d['tenaamstelling']     = $('#Field-Tenaamstelling').val();
    d['nationaliteit']      = $('#Field-Nationaliteit').val();
    d['voorkeurstaal']      = $('#Field-Voorkeurstaal').val();
    d['opm']                = $('#Field-Opm').val();
    
    return d;
}

function nul( x )
{
    if ( x == null ) { return null; }
    if ( (""+x).length == 0 ) { return null; }
    if ( (""+x).substr(0,4) == "0000" ) { return null; }
    
    return x;
}
function ucfirst( s )
{
    if ( s.length < 2 ) { return s; }
    return s.substr(0,1).toUpperCase() + s.substr(1).toLowerCase();
}


function any_changes()
{
    var ould = Details[pid()];
    var nehw = get_new_details();
    for ( i in nehw )
    {
        if ( ould[i] != nehw[i] ) { return true; }
    }
    return false;
}


function save()
{
    SGR.save();
    for ( i in Changes )
    {
        $.ajax({
            type: 'POST',
            url: '/bewerken/json/wijzig',
            data: Details[i],
            success: function(data)
            {
                delete Changes[i];
                //alert(data);
                if ( pid() == i )
                {
                    $('#EditPeople').removeClass('modified');
                }
            },
            dataType: 'html'
        });
    }
}



function insert_person()
{
    if ( !IDs ) { return; }
    $.ajax({
        type: 'POST',
        url: '/bewerken/json/invoegen',
        data: {'failsafe': 'ja'},
        success: function(data)
        {
            if ( ! data ) { alert("Invoegen ging mis. Sorry."); return; }
            //alert(data);
            IDs[IDs.length] = data;
            select_index(IDs.length-1);
        },
        dataType: 'json'
    });
}
function delete_person()
{
    if ( !confirm('Weet je het zeker?') ) { return; }
    if ( !confirm('Het verwijderen van personen is doorgaans niet de bedoeling. \n' + 
                  'Waarschijnlijk volstaat het om deze persoon uit te schrijven bij alle groepen.\n' +
                  'Weet je echt zeker dat je deze persoon COMPLEET WILT WISSEN?') ) { return; }
    if ( !confirm('Ik vraag het nog één keer. \n'+
                  'Weet je het echt echt zeker?') ) { return; }
    $.ajax({
        type: 'POST',
        url: '/bewerken/json/verwijderen',
        data: {'pers_id': pid(), 'weetjehetzeker': 'ja'},
        success: function(data)
        {
            if (data)
            {
                location.hash = '';
                reload();
            }
        },
        dataType: 'json'
    });
}



