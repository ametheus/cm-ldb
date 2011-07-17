

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



