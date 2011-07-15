
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
        for ( var i = 0; i < n; i++ ) { SGR.Studie.append( sgr["studies"][i] ); }
        n = sgr["groepen"].length;
        for ( var i = 0; i < n; i++ ) { SGR.Groep.append( sgr["groepen"][i] ); }
        n = sgr["relaties"].length;
        for ( var i = 0; i < n; i++ ) { SGR.append_relatie( sgr["relaties"][i] ); }
        
        $("td.special_label").removeClass('loading');
        
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
    
    
    Studie: {
        fmt: function( studie )
        {
            return studie["studienaam"] + ( studie["afgestudeerd"] > 0 ? ' (A)' : '' );
        },
        key: function( studie )
        {
            if ( studie == null ) { return null; }
            return {
                studie_id: studie["studie_id"],
                afgestudeerd: studie["afgestudeerd"]
            };
        },
        get_fields: function()
        {
            return {
                studie_id: $("#StudieEditor #studie_id").val(),
                studienaam: $("#StudieEditor #studie_id :selected").html(),
                afgestudeerd: ( $("#StudieEditor #afgestudeerd").attr('checked') ? 1 : 0 )
            };
        },
        set_fields: function( studie )
        {
            $("#StudieEditor #studie_id").val(studie["studie_id"]);
            $("#StudieEditor #afgestudeerd").attr('checked', ( studie["afgestudeerd"] > 0 ) );
        }
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
                if ( str.substr(0,4) == "0000" ) { return null; }
                return str;
            }
            
            return {
                groep_id: groep.groep_id,
                van: zero( groep.van ),
                tot: zero( groep.tot )
            };
        },
        get_fields: function()
        {
            return {
                groep_id: $("#GroepenEditor #groep_id").val(),
                klasse: $("#GroepenEditor #groep_id :selected").attr('klasse'),
                groepsnaam: $("#GroepenEditor #groep_id :selected").html(),
                van: $("#GroepenEditor #van").val(),
                tot: $("#GroepenEditor #tot").val()
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
            studies: SGR.Studie.transaction,
            groepen: SGR.Groep.transaction
        }));
        
        $.ajax({
            type: 'POST',
            url: '/bewerken/json/wijzig-SGR',
            data: {transaction: trans},
            success: function(data)
            {
                SGR.Studie.transaction = [];
                SGR.Groep.transaction = [];
            },
            dataType: 'html'
        });
    }
};

create_slc( "#Secties", SGR.Groep, "#GroepenEditor", "Bewerk groep" )
create_slc( "#Studies", SGR.Studie, "#StudieEditor", "Bewerk studie" )

$(function()
{
    // TODO: Later weer weghalen als Studies/relaties ook gebruik maken van SLC's
    $("#Relaties").append('<ul />');
});

