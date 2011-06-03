

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
        for ( var i = 0; i < n; i++ ) { SGR.append_groep( sgr["groepen"][i] ); }
        n = sgr["relaties"].length;
        for ( var i = 0; i < n; i++ ) { SGR.append_relatie( sgr["relaties"][i] ); }
        
        $("td.special_label").removeClass('loading');
        
    },
    append_studie: function( studie )
    {
        var li = $('<li>'+studie["studienaam"]+
                   (studie["afgestudeerd"]>0?' (A)':'')+'</li>');
        li.click(function(){alert(studie);});
        $("#Studies").append(li);
    },
    append_groep: function( groep )
    {
        var li = $('<li>'+groep["klasse"]+' : '+groep["groepsnaam"]+
                   ' ('+groep["van"].substr(0,4)+'-'+
                   (groep["tot"]?groep["tot"].substr(0,4):'heden')+')</li>');
        li.click(function(){alert(groep["van"]);});
        $("#Secties").append(li);
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
        $("#Relaties").append(li);
    }
    
};
