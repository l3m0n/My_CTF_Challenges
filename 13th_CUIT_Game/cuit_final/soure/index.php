<?php
session_start();
$_SESSION['check'] = 1;
?>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html;charset=utf-8">
  <title>Index</title>
  <link rel="stylesheet" href="static/css/bootstrap.min.css">
  <link rel="stylesheet" type="text/css" href="static/main.css">
  <script src="static/jquery.min.js"></script>
  <script src="static/js/bootstrap.min.js"></script>
  <script src="static/main.js"></script>
</head>
<body>

<div id="l-body">

  <div id="nav">
    <nav class="navbar navbar-inverse" role="navigation">
      <div class="container-fluid">
      <div class="navbar-header">
        <a class="navbar-brand" href="#">SYC工具</a>
      </div>
      <div>
        <ul class="nav navbar-nav">
          <li class="active"><a href="index.php">首页</a></li>
          <li><a href="index.php">短域名</a></li>
          <li><a href="about.php">关于</a></li>
        </ul>
      </div>
      </div>
    </nav>
  </div>

  <div id="content-box" class="container">
    <div id="content" class="row">
      <div class="col-md-6 col-md-push-3 build_content">
        <div class="title">短域名工具</div>
        <div class="build_content">来简化一下你的URL吧 &#40;&#62;&#95;&#60;&#41; </div>
        <div style="padding: 50px 100px 10px;">
          <form class="bs-example bs-example-form" role="form">
            <div class="input-group">
              <span class="input-group-addon">URL</span>
              <input type="text" class="form-control" placeholder="http://www.sycsec.com/hello" id="url">
              <span class="input-group-addon glyphicon glyphicon-search" onclick="submit()"></span>
            </div>
          </form>
        </div>

        <div id="show_result"></div>

      </div>
    </div>
  </div>

  <div class="footer">
    <p>Hello,Syclover!</p>
    <p>Copyright &#169; 2017 short.sycsec.com </p>
  </div>

</div>
</body>
</html>