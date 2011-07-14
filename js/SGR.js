
var SGR = {
    
    cache: {},
    timer: null,
    
    load: function( pers_id )
    {
        $("td.special_label ul").html('');
        $("td.special_label").addClass('loading');
        if ( SGR.timer ) { clearTimeout( SGR.timer ); }
        SGR.timer = setTimeout(function(){SGR.actually_load(pers_id);}, 500 );
    },
    
    actually_load: function( pers_id )
    {
        $.ajax({
            type: 'GET',
            url: '/bewerken/json/SGR',
            data: {'pers_id': pers_id},
            success: function(data)
            {
                SGR.cache[pers_id] = data;
                if ( pid() == pers_id ) { SGR.loaded(); }
            },
            dataType: 'json'
        });
    },
    loaded: function()
    {
        var sgr = SGR.cache[pid()];
        if ( ! sgr ) { alert( "Not found!" ); return; }
        
        var n = sgr["studies"].length;
        for ( var i = 0; i < n; i++ ) { SGR.append_studie( sgr["studies"][i] ); }
        n = sgr["groepen"].length;
        for ( var i = 0; i < n; i++ ) { SGR.Groep.append( sgr["groepen"][i] ); }
        n = sgr["relaties"].length;
        for ( var i = 0; i < n; i++ ) { SGR.append_relatie( sgr["relaties"][i] ); }
        
        $("td.special_label").removeClass('loading');
        
    },
    append_studie: function( studie )
    {
        var li = $('<li>'+studie["studienaam"]+
                   (studie["afgestudeerd"]>0?' (A)':'')+'</li>');
        li.click(function(){alert(studie);});
        $("#Studies ul").append(li);
    },
    append_relatie: function( relatie )
    {
        var opid = ( relatie["pers_id_1"] == pid() ? relatie["pers_id_2"] : relatie["pers_id_1"] );
        var li = '<li>';
        if ( relatie["relatie"] == 'partner' )
        {
            li += "Partner";
        }
        else if (( relatie["relatie"] == 'ouder' ) && ( pid() == relatie["pers_id_1"] ))
        {
            li += "Ouder";
        }
        else if (( relatie["relatie"] == 'ouder' ) && ( pid() == relatie["pers_id_2"] ))
        {
            li += "Kind";
        }
        else
        {
            li += "Kennis";
        }
        li = $(li + ' van ' + Naam.van(opid) + '</li>');
        li.click(function(){alert(relatie);});
        $("#Relaties ul").append(li);
    },
    
    
    Groep: {
        fmt: function( groep )
        {
            return groep["klasse"]+' : '+groep["groepsnaam"]+
                ' ('+groep["van"].substr(0,4)+'-'+
                (groep["tot"]?groep["tot"].substr(0,4):'heden')+')';
        },
        key: function( groep )
        {
            //alert( JSON.stringify(groep) );
            if ( groep == null ) { return null; }
            
            var zero = function( str )
            {
                if ( str == null ) { return null; }
                if ( typeof(str.length) == 'undefined' ) { return str; }
                if ( str.length == 0 ) { return null; }
                return str;
            }
            
            return {
                groep_id: groep.groep_id,
                van: zero( groep.van ),
                tot: zero( groep.tot )
            };
        },
        set_fields: function( groep )
        {
            $("#GroepenEditor #groep_id").val(groep["groep_id"]);
            $("#GroepenEditor #van").val(groep["van"]);
            $("#GroepenEditor #tot").val(groep["tot"]);
        }
    },
    
    save: function()
    {
        var trans = escape(JSON.stringify({
            groepen: SGR.Groep.transaction
        }));
        
        $.ajax({
            type: 'POST',
            url: '/bewerken/json/wijzig-SGR',
            data: {transaction: trans},
            success: function(data)
            {
                SGR.Groep.transaction = [];
            },
            dataType: 'html'
        });
    }
};

create_slc( "#Secties", SGR.Groep, "#GroepenEditor", "Bewerk groep" )

$(function()
{
    $("#GroepenEditor #save").click(function(){
        
        new_groep = {
            groep_id: $("#GroepenEditor #groep_id").val(),
            klasse: $("#GroepenEditor #groep_id :selected").attr('klasse'),
            groepsnaam: $("#GroepenEditor #groep_id :selected").html(),
            van: $("#GroepenEditor #van").val(),
            tot: $("#GroepenEditor #tot").val()
        };
        
        SGR.Groep.change( new_groep );
        
        $("#GroepenEditor").dialog("close");
    });
    
    // TODO: Later weer weghalen als Studies/relaties ook gebruik maken van SLC's
    $("#Studies").append('<ul />');
    $("#Relaties").append('<ul />');
});


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