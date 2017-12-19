var editor = ace.edit("code");
editor.setTheme("ace/theme/crimson_editor");
editor.getSession().setMode("ace/mode/xml");
editor.getSession().setUseWrapMode(true);
editor.focus();

$("#code").resize(function() {
   editor.resize(true);
   editor.focus();
});
$("#codebox" ).tabs({
   onSelect: function( title, index ) {
      editor.resize(true);
      editor.focus();
   }
});
$(document).mouseup(function (e){
    var container = $("#uploadmenu");
    if (!container.is(e.target) && container.has(e.target).length === 0){
        container.hide();
    }
});
$(document).ready(function(){
   var url = "upload_file.php";
   $('#ancFileUpload').click(function(){
      $('#uploadFile').trigger('click');
   });
   $('#ancFileUpload1').click(function(){
      $('#uploadFile1').trigger('click');
   });
   $('#uploadFile').change(function(e){
      var file = $('#uploadFile').val();
      var exts = ['xml', 'log', 'htm', 'html', 'csv', 'tex', 'asp', 'php', 'pl', 'py', 'rss', 'xhtml', 'aspx', 'csr', 'ini', 'c', 'cpp', 'sql', 'json','txt', 'js', 'java', 'javascript', 'text'];
      if( file.length <= 0   ) {
         alert("Please select a file from local drive.");
         $('#uploadFile').focus();
         return false;
      }
      var ext = file.split('.');
      ext = ext.reverse();
/*
      if ($.inArray ( ext[0].toLowerCase(), exts ) < 0 ){
         alert("Please select a text file with valid extension");
         $('#uploadFile').focus();
         return false;
      }
*/
      var inputs = new FormData();
      inputs.append('file', $("#uploadFile").prop("files")[0]);
      $("#loading").css({"visibility":"visible"});
      $.ajax({
         url: url,
         method:"POST",
         cache: false,
         data: inputs,
         processData: false,
         contentType: false
      }).done(function(msg) {
         $('#codebox').tabs('select', 0);
         editor.setValue(msg);
         editor.clearSelection();
         $('#uploadFile').val('');
         $("#loading").css({"visibility":"hidden"});
      });
   });

   $('#uploadFile1').change(function(e){
      var file = $('#uploadFile1').val();
      var exts = ['xml', 'sql', 'json','txt', 'js', 'java', 'javascript', 'text'];
      if( file.length <= 0   ) {
         alert("Please select a file from local drive.");
         $('#uploadFile1').focus();
         return false;
      }
      var ext = file.split('.');
      ext = ext.reverse();
      if ($.inArray ( ext[0].toLowerCase(), exts ) < 0 ){
         alert("Please select a text file with valid extension");
         $('#uploadFile').focus();
         return false;
      }
      var inputs = new FormData();
      inputs.append('file', $("#uploadFile1").prop("files")[0]);
      $("#loading").css({"visibility":"visible"});
      $.ajax({
         url: url,
         method:"POST",
         cache: false,
         data: inputs,
         processData: false,
         contentType: false
      }).done(function(msg) {
         $('#codebox').tabs('select', 1);
         xsdeditor.setValue(msg);
         xsdeditor.clearSelection();
         $('#uploadFile1').val('');
         $("#loading").css({"visibility":"hidden"});
      });
   });
   $('#textUrl').keypress(function(event){
      var keycode = (event.keyCode ? event.keyCode : event.which);
      if(keycode == '13'){
         getDataUrl();
      }
   });
   $('#textUrl1').keypress(function(event){
      var keycode = (event.keyCode ? event.keyCode : event.which);
      if(keycode == '13'){
         getDataUrl1();
      }
   });
   $('#btnGo').click(function() {
      getDataUrl();
   });
   $('#btnGo1').click(function() {
      getDataUrl1();
   });
   function getDataUrl(){
      if ($('#textUrl').val() == '') {
         alert('Please Enter URL');
         $('#textUrl').focus();
         return false;
      }
      var inputs = {
         "url": $('#textUrl').val(),
         "type":"url"
      };
      $.ajax({
         type: "POST",
         url: url,
         data: inputs,
         beforeSend: function() {
            $("#loading").css({"visibility":"visible"});
         },
         success: function(msg) {
            $("#loading").css({"visibility":"hidden"});
            if(msg == -1){
               alert('There is no content, please check url.');
               $('#textUrl').focus();
            } else {
               editor.setValue(msg);
               editor.clearSelection();
               $('#textUrl').val('');
            }
         }
      });
   };
  function getDataUrl1(){
      if ($('#textUrl1').val() == '') {
         alert('Please Enter URL');
         $('#textUrl1').focus();
         return false;
      }
      var inputs = {
         "url": $('#textUrl1').val(),
         "type":"url"
      };
      $.ajax({
         type: "POST",
         url: url,
         data: inputs,
         beforeSend: function() {
            $("#loading").css({"visibility":"visible"});
         },
         success: function(msg) {
            $("#loading").css({"visibility":"hidden"});
            if(msg == -1){
               alert('There is no content, please check url.');
               $('#textUrl1').focus();
            } else {
               xsdeditor.setValue(msg);
               xsdeditor.clearSelection();
               $('#textUrl1').val('');
            }
         }
      });
   };
   $("div#loading").css({"visibility":"hidden"});
   $("#cc").css({'visibility':'visible'});
});
$('#undo').click( function(){
   if( editor.getSession().getUndoManager().hasUndo() ){
      editor.getSession().getUndoManager().undo( false );
   }
});
$('#redo').click( function(){
   if( editor.getSession().getUndoManager().hasRedo() ){
      editor.getSession().getUndoManager().redo( false );
   }
});
var clipboard;
$('#cut').click( function(){
   var selectedRange = editor.getSelectionRange();
   if( editor.getSession().getTextRange(selectedRange) ){
      clipboard = editor.getSession().getTextRange(selectedRange);
      editor.getSession().remove(selectedRange);
   }
});
$('#delete').click( function(){
   var selectedRange = editor.getSelectionRange();
   if( editor.getSession().getTextRange(selectedRange) ){
      editor.getSession().remove(selectedRange);
   }
});
$('#copy').click( function(){
   var selectedRange = editor.getSelectionRange();
   if( editor.getSession().getTextRange(selectedRange) ){
      clipboard = editor.getSession().getTextRange(selectedRange);
   }
});
$('#paste').click( function(){
   var position = editor.getCursorPosition();
   editor.getSession().insert(position, clipboard);
});
$('#select').click( function(){
   editor.getSelection().selectAll();
});
$('#find').click( function(){
   editor.execCommand("find");
});
$('#findreplace').click( function(){
   editor.execCommand("replace");
});
function setEditorTheme( theme ){
   editor.setTheme("ace/theme/" + theme);
}
function setEditorSoftWrap( mode ){
   if( mode === "true" ){
      editor.getSession().setUseWrapMode(true);
   }else if (mode === "false" ){
      editor.getSession().setUseWrapMode(false);
   }else{
      editor.getSession().setUseWrapMode(true);
      editor.getSession().setWrapLimitRange(parseInt(mode), parseInt(mode));
   }
}

function setEditorFontSize( size ){
   editor_font_size = parseInt(size);
   editor.setFontSize(editor_font_size);
   $('#east').css('font-size', editor_font_size );
}
function setEditorTabSize( size ){
   editor_tab_size = parseInt(size);
   editor.getSession().setTabSize(editor_tab_size);
}
function setEditorInvisible( flag ){
   editor.setShowInvisibles(flag);
}
function setEditorGutter( flag ){
   editor.renderer.setShowGutter(flag);
}
code = editor.getValue();
editor.setValue( code );
$("div#loading").css({"visibility":"hidden"});
$("form#ff").css({"visibility": "visible"});

