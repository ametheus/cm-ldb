

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




