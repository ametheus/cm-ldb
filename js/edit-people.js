
var IDs = null;

$(function()
{
    $.ajax({
        url: '/bewerken/json/IDs',
        dataType: 'json',
        success: function(data)
        {
            IDs = data;
            location.hash = IDs[IDs.length-1];
        }
    });
})



