

function create_slc( container, CC, editor, title )
{
    CC.current = null;
    CC.transaction = [];
    CC.cache = [];
    
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
            width: 400
        });
        
        CC.current = [ groep, li ];
        if ( groep )
        {
            CC.set_fields( groep );
        }
        else
        {
            $(editor + " .resetable").val(null);
            $(editor + " .uncheckable").attr('checked',false);
        }
    };
    
    CC.change = function( new_groep )
    {
        var key = CC.key( CC.current[0] );
        var ng = CC.key( new_groep );
        var c = CC.cache[pid()];
        
        if ( duck_compare( key, ng ) ) { return; }
        
        CC.transaction.push( [pid(), key, ng] );
        CC.current[0] = new_groep;
        
        
        if ( key == null )
        {
            c.push( new_groep );
            CC.append( new_groep );
            return;
        }
        
        
        if ( ng == null )
        {
            // Remove the item from the cache
            for ( var i = 0; i < c.length; i++ )
            {
                if ( !duck_compare( key, c[i] ) ) { continue; }
                c.splice( i, 1 );
                break;
            }
            // Remove the item from the DOM
            if ( CC.current[1] )
            {
                CC.current[1].remove();
            }
        }
        else
        {
            // Modify the cache
            for ( var i = 0; i < c.length; i++ )
            {
                if ( !duck_compare( key, c[i] ) ) { continue; }
                c[i] = new_groep;
                break;
            }
            // Set the new text
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




function duck_compare( A, B )
{
    // Perform a shallow comparison of dict's A and B. Returns true if they're
    // ostensibly equal.
    
    if (( A == null ) && ( B == null )) { return true; }
    if (( A == null ) || ( B == null )) { return false; }
    
    for ( i in A )
    {
        if (( ! ( i in B ) ) || ( A[i] != B[i] )) { return false; }
    }
    return true;
}

