

function create_slc( container, CC, editor, title )
{
    CC.append = function( groep )
    {
        var li = $('<li>'+CC.fmt(groep)+'</li>');
        
        li.click(function(){
            CC.edit( groep, $(this) );
        });
        
        $(container).append(li);
    };
    
    CC.current = null;
    
    CC.edit = function( groep, li )
    {
        $(editor).dialog({
            title: "Bewerk groep",
            modal: true,
            width: 350
        });
        
        CC.current = [ groep, li ];
        if ( groep )
        {
            CC.set_fields( groep );
        }
        else
        {
            $(editor)(".resetable").val(null);
        }
    };
    
    CC.change = function( new_groep )
    {
        var key = CC.key( CC.current[0] );
        var ng = CC.key( new_groep );
        
        if ( duck_compare( key, ng ) ) { return; }
        
        CC.transaction.push( [pid(), key, ng] );
        
        CC.current[0] = new_groep;
    };
    
    CC.transaction = [];
    
    if ( !( "fmt" in CC ) || !( "key" in CC ) || !( "set_fields" in CC ) )
    {
        throw "Code container does not have required functions" +
            "fmt(), key(), and set_fields().";
    }
}



