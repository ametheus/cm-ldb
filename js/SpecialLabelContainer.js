

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
        else if ( ng == null )
        {
            if ( CC.current[1] )
            {
                CC.current[1].remove();
            }
        }
        else
        {
            CC.current[1].html(CC.fmt(CC.current[0]));
        }
    };
    
    if ( !( "fmt" in CC ) || !( "key" in CC ) ||
        !( "get_fields" in CC ) || !( "set_fields" in CC ) )
    {
        throw "Code container does not have required functions" +
            "fmt(), key(), get_fields(), and set_fields().";
    }
    
    
    $(function(){
        
        // Create buttons, place everything in a nested <div>, and generate
        // an <ul> below.
        var div = $('<div />');
        div.html($(container).html());
        $(container).html('').append(div);
        
        var buttons = $('<div class="buttons" />');
        div.append(buttons);
        
        buttons.append("<div>add</div>").click(CC.addnew);
        
        $(container).append('<ul />');
        
        
        // Set correct click events on the editor dialog.
        $(editor + " #save").click(function(){
            
            new_groep = CC.get_fields( editor );
            CC.change( new_groep );
            
            $(editor).dialog("close");
        });
        
        $(editor + " #delete").click(function(){
            if ( confirm( "Weet je zeker dat je dit item wilt verwijderen?\n" +
                          "Dit is waarschijnlijk niet wat je wil.") )
            {
                CC.change( null );
                
                $(editor).dialog("close");
            }
        });
    });
}



