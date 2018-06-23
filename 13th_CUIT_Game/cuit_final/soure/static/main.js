
function show_success(msg){
  var content = ''+
  '<div class="alert alert-success alert-dismissable" role="alert">'+
  ' <button class="close" type="button" data-dismiss="alert">&times;</button>'+
  '  <strong>成功!</strong>'+
  '  你的链接:'+
  '  <a href="'+ msg +'" class="alert-link" target="_blank">'+ msg +'</a>'+
  '</div>';
  $("#show_result").append(content);
}

function show_error(msg){
  var content = ''+
  '<div class="alert alert-danger alert-dismissable" role="alert">'+
  ' <button class="close" type="button" data-dismiss="alert">&times;</button>'+
  '  <strong>错误!</strong>'+
  '  好像遇到了点问题... (-_-) , '+
  msg +
  '</div>';
  $("#show_result").append(content);
}

function submit() {
  var url = document.getElementById("url").value;
  $.ajax({
    type: "GET",
    url: './tools.php?a=s&u='+url,
    dataType: "json",
    headers: {
        Accept:"application/json",
        "Content-Type":"application/json"
    },
    success : function(data){
      if(data.status == 1){
        show_success(data.msg);
      }else{
        show_error(data.msg);
      }
    }
  });
}

