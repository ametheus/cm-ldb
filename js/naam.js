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
   Finds names.
*/


var Naam = {
    
    cache: {},
    
    van: function( pers_id )
    {
        if ( pers_id in Naam.cache ) { return Naam.cache[pers_id]; }
        
        $.ajax({
            type: 'GET',
            url: '/bewerken/json/namen',
            data: {'pers_id': pers_id},
            success: function(data)
            {
                for ( var i = 0; i < data.length; i++ )
                {
                    Naam.cache[ data[i]["pers_id"] ] = data[i]["Naam"];
                }
                $("span.naam_ontbreekt").each(Naam.terugzetten);
            },
            dataType: 'json'
        });
        
        return '<span class="naam_ontbreekt" id="'+pers_id+'">J.S. Bach</span>'
    },
    
    
    terugzetten: function( index )
    {
        var id = $(this).attr('id');
        if ( !( id in Naam.cache )) { return; }
        
        $(this).html(Naam.cache[id]);
        $(this).removeClass("naam_ontbreekt");
    }
};




