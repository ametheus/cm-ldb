//Admina Template
//Author: Chris Mooney (http://themeforest.net/user/ChrisMooney)

jQuery(document).ready(function($) {

// Theme Changer
   $("#themecolor .blue").click(function(){
      $("#theme").attr("href", "css/themes/blue.css");
      return false;
   });
   $("#themecolor .red").click(function(){
      $("#theme").attr("href", "css/themes/red.css");
      return false;
   });

   $("#themecolor .green").click(function(){
      $("#theme").attr("href", "css/themes/green.css");
      return false;
   });

// Dyanmic Width Feature
 $('#pagewidth a').click(function(){
 var lay_id = $(this).attr("class");
 $('body').attr("id",lay_id);
 $.cookie('layout', lay_id );
$('#pagewidth a').removeClass("active");
 $(this).addClass("active");
 drawChart();
 })

 var lay_cookie = $.cookie('layout');

 $("#pagewidth a[id="+ lay_cookie +"]").addClass("active");

 if (lay_cookie == 'layout100') {
 $('body').attr("id","layout100");
 };

 if (lay_cookie == 'layout90') {
 $('body').attr("id","layout90");
 };

 if (lay_cookie == 'layout75') {
 $('body').attr("id","layout75");
 };

 if (lay_cookie == 'layout980') {
 $('body').attr("id","layout980");
 };

 if (lay_cookie == 'layout1280') {
 $('body').attr("id","layout1280");
 };

 if (lay_cookie == 'layout1400') {
 $('body').attr("id","layout1400");
 };

 if (lay_cookie == 'layout1600') {
 $('body').attr("id","layout1600");
 };

//Sidebar Widgets
var setSelector = "#widgets";
var setCookieName = "widget-order";
var setCookieExpiry = 7;

function getOrder() {
 $.cookie(setCookieName, $(setSelector).sortable("toArray"), { expires: setCookieExpiry, path: "/" });
}
function restoreOrder() {
 var list = $(setSelector);
 if (list == null) return
 var cookie = $.cookie(setCookieName);
 if (!cookie) return;
 var IDs = cookie.split(",");
 var items = list.sortable("toArray");
 var rebuild = new Array();
 for ( var v=0, len=items.length; v<len; v++ ){
 rebuild[items[v]] = items[v];
 }
 for (var i = 0, n = IDs.length; i < n; i++) {
 var itemID = IDs[i];
 if (itemID in rebuild) {
 var item = rebuild[itemID];
 var child = $("ul.ui-sortable").children("#" + item);
 var savedOrd = $("ul.ui-sortable").children("#" + itemID);
 child.remove();
 $("ul.ui-sortable").filter(":first").append(savedOrd);
 }
 }
}
$(function() {
 $(setSelector).sortable({
	 placeholder: 'ui-state-highlight',
	 forcePlaceholderSize: 'true',
 axis: "y",
 cursor: "move",
 update: function() { getOrder(); }
 });
 restoreOrder();
}); 

// Sidebar close/open (with cookies)
 function close_sidebar() {
 $("body").addClass('sidebar-off');
 $("#open_sidebar").show();
 $("#close_sidebar").hide();
 $.cookie('sidebar', 'closed');
 if( $('body').hasClass('chart') ) {
    // redraw chart
	drawChart();
 }
 }
 function open_sidebar() {
 $("body").removeClass('sidebar-off');
 $("#open_sidebar").hide();
 $("#close_sidebar").show();
 $.cookie('sidebar', 'open');
 if( $('body').hasClass('chart') ) {
    // redraw chart
	drawChart();
}
}
 $('#close_sidebar').click(function(){
 close_sidebar();
 if($.browser.safari) {
 location.reload();
 }
 });
 $('#open_sidebar').click(function(){
 open_sidebar();
 if($.browser.safari) {
 location.reload();
 }
 });
 var sidebar = $.cookie('sidebar');

 if (sidebar == 'closed') {
 close_sidebar();
 };

 if (sidebar == 'open') {
 open_sidebar();
 };

// Datepicker Setup
$( ".datepicker" ).datepicker();

// Filetree Setup
$(".filetree").treeview({
		control: "#treecontrol"
	});
$("#filetree-add").click(function() {
		var branches = $("<li><span class='folder'>New Sublist</span><ul>" + 
			"<li><img alt='' src='./img/icons/16/page.png' /><a href='#'>New Item</a></li>" + 
			"<li><img alt='' src='./img/icons/16/page.png' /><a href='#'>New Item</a></li></ul></li>").appendTo("#browser");
		$("#browser").treeview({
			add: branches
		});
		branches = $("<li class='closed'><span class='folder'>New Sublist</span><ul><li><img alt='' src='./img/icons/16/page.png' /><a href='#'>New Item</a></li><li><img alt='' src='./img/icons/16/page.png' /><a href='#'>New Item</a></li></ul></li>").prependTo("#folder21");
		$("#browser").treeview({
			add: branches
		});
	});
	
// Gallery Setup
$('.gallery a').lightBox({
	fixedNavigation:true,
	overlayOpacity:0.5,
	imageLoading:'img/lightbox/lightbox-ico-loading.gif',
	imageBtnClose:'img/lightbox/lightbox-btn-close.gif',
	imageBtnPrev:'img/lightbox/lightbox-btn-prev.gif',
	imageBtnNext:'img/lightbox/lightbox-btn-next.gif',
	imageBlank:'img/lightbox/lightbox-blank.gif'
	});
	
// wysiwyg Setup
$('.wysiwyg').wysiwyg({
    controls: {
	      strikeThrough : { visible : false },
      underline     : { visible : false },
      
      separator00 : { visible : false },
      
      justifyLeft   : { visible : false },
      justifyCenter : { visible : false },
      justifyRight  : { visible : false },
      justifyFull   : { visible : false },
      
      separator01 : { visible : false },
      
      indent  : { visible : false },
      outdent : { visible : false },
      
      separator02 : { visible : false },
      
      subscript   : { visible : false },
      superscript : { visible : false },
      
      separator03 : { visible : false },
      
      undo : { visible : false },
      redo : { visible : false },
      
      separator04 : { visible : false },
      
      insertOrderedList    : { visible : false },
      insertUnorderedList  : { visible : false },
      insertHorizontalRule : { visible : false },
      
      separator07 : { visible : false },
      
      cut   : { visible : false },
      copy  : { visible : false },
      paste : { visible : false }	
 }
});

// Make objects collapsible
$(".collapsible-list span.toggle").click(function(){
$(this).closest('li').toggleClass('collapsed');
return false;
});
$("fieldset legend > a, .fieldset .legend > a").click(function(){
$(this).closest('fieldset, .fieldset').toggleClass('collapsed');
$("fieldset legend a").toggleClass('collapse');
return false;
});			

// Notification Animations
$('.notification').hide().append('<span class="close" title="Dismiss"></span>').fadeIn('slow');
$('.notification .close').hover(
function() { $(this).addClass('hover'); },
function() { $(this).removeClass('hover'); }
);
$('.notification .close').click(function() {
$(this).parent().slideUp('slow', function() { $(this).remove(); });
}); 


// Tabs
$("#tabs").tabs();
$("#dashtabs").tabs();
$("#demotabs").tabs();
$("#formtabs").tabs();


 // Check all checkboxes when the one in a table head is checked:

 $(function () { // this line makes sure this code runs on page load
 	$('.check-all').click(function () {
 		$(this).parents('table:eq(0)').find(':checkbox').attr('checked', this.checked);
 	});
 });


// Tipsy Tooltips
$('.tooltip').tipsy({fade: true});
$('.tooltip.north').tipsy({fade: true, gravity: 's'});
$('.tooltip.east').tipsy({fade: true, gravity: 'w'});
$('.tooltip.west').tipsy({fade: true, gravity: 'e'});
// Form Tooltips
$('form [title]').tipsy({fade: true, trigger: 'focus', gravity: 'w'});
// wysiwyg Toolbar Tooltips
$('.wysiwyg a').tipsy({fade: true, gravity: 's'});

// Form Switches
  $(".switch-enable").click(function(){
        var parent = $(this).parents('.switch-wrapper');
        $('.switch-disable',parent).removeClass('selected');
        $(this).addClass('selected');
        $('.checkbox',parent).attr('checked', true);
    });
    $(".switch-disable").click(function(){
        var parent = $(this).parents('.switch-wrapper');
        $('.switch-enable',parent).removeClass('selected');
        $(this).addClass('selected');
        $('.checkbox',parent).attr('checked', false);
    });





}); 
