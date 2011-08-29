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
   Creates a PersonPicker.
*/


var PersonPicker = {
    
    cache: {},
    
    create: function( elt, cb )
    {
        $(elt).autocomplete({
            
            minLength: 2,
            
            source: function( request, response )
            {
                var term = request.term;
                if ( term in PersonPicker.cache )
                {
                    response( PersonPicker.cache[ term ] );
                    return;
                }
                
                lastXhr = $.getJSON(
                    "/bewerken/json/personpick",
                    { q: term },
                    function( data, status, xhr )
                    {
                        PersonPicker.cache[ term ] = data;
                        if ( xhr === lastXhr )
                        {
                            response( data );
                        }
                    }
                );
            },
            
            select: function( event, ui )
            {
                cb( ui.item.id );
            }
            
        });
    }
    
}



