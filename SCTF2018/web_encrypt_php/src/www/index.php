<?php
session_start();
$lemon_flag = 1;
welcome_to_sctf2018(1);
$file = @$_GET['f'];
if (empty($file)) {
	header("Location: ./?f=login.php");
}
if (strpos($file, 'tmp') !== false || strpos($file, 'upload_7788') !== false) {
	die('NoNoNo.');
} else {
	include_once $file;
}
?>
LaLaLaLaLaLa

<!-- 感谢SCTF2018友情支持,XD -->