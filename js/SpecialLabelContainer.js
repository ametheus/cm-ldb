

function create_slc( container, CC, editor, title )
{
    CC.current = null;
    CC.transaction = [];
    
    CC.append = function( groep )
    {
        var li = $('<li>'+CC.fmt(groep)+'</li>');
        
        li.click(function(){
            CC.edit( groep, $(this) );
        });
        
        $(container + " ul").append(li);
    };
    
    CC.addnew = function()
    {
        CC.edit( null, null );
    };
    
    CC.edit = function( groep, li )
    {
        $(editor).dialog({
            title: title,
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
            $(editor + " .resetable").val(null);
        }
    };
    
    CC.change = function( new_groep )
    {
        var key = CC.key( CC.current[0] );
        var ng = CC.key( new_groep );
        
        if ( duck_compare( key, ng ) ) { return; }
        
        CC.transaction.push( [pid(), key, ng] );
        
        CC.current[0] = new_groep;
        if ( key == null )
        {
            CC.append( new_groep );
        }
        else
        {
            CC.current[1].html(CC.fmt(CC.current[0]));
        }
    };
    
    if ( !( "fmt" in CC ) || !( "key" in CC ) || !( "set_fields" in CC ) )
    {
        throw "Code container does not have required functions" +
            "fmt(), key(), and set_fields().";
    }
    
    
    $(function(){
        var buttons = $('<div class="buttons" />');
        $(container).append(buttons);
        
        buttons.append("<div>add</div>").click(CC.addnew);
        
        $(container).append('<ul />');
    });
}



