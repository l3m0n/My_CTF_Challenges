<?php
//error_reporting(E_ALL ^ E_NOTICE ^ E_WARNING);
//error_reporting(0);

session_start();

include "redis.php";
include "safe.php";
include "db.php";

$db = new DB;
//注意空白字符也被过滤
$url = addslashes(@$_GET['u']);
$s = addslashes(@$_GET['s']);

//检测非内网的访问是否带有session
// if (!check_inner_ip($_SERVER['HTTP_HOST'])) {
// 	if (@$_SESSION['check'] !== 1) {
// 		die('403: No Session!');
// 	}
// }

if ($_GET['a'] === 'c') {
	$sql = "select * from log where s='" . $s . "' limit 0,1";
	$row = $db->select($sql);
	if (exist_var($row)) {
		header("Location: " . $row[0]['url']);
		exit();
	} else {
		die('没有这个记录!(>_<)!');
	}
} elseif ($_GET['a'] === 's') {
	$result = array('status' => 0, 'msg' => '500: 无参数');
	if (exist_var($url)) {
		$m = safe_request_url($_GET['u']);
		if ($m === true) {
			$randmon = random_str();
			$sql = "insert into `log` (`url`,`s`) values ('" . $url . "','" . $randmon . "');";
			$db->insert($sql);
			$result['status'] = 1;
			$result['msg'] = '/s/' . $randmon;
		} else {
			$result['msg'] = $GLOBALS['msg'];
		}
	}

	header('Content-Type: application/json');
	echo json_encode($result);
}

function exist_var($v) {
	if (isset($v) && !empty($v)) {
		return true;
	} else {
		return false;
	}
}

function random_str() {
	$authnum = "";
	srand((double) microtime() * 1000000);
	$str_ = "0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z";
	$list = explode(",", $str_);
	for ($i = 0; $i < 6; $i++) {
		$randnum = rand(0, 61);
		$authnum .= $list[$randnum];
	}
	return $authnum;
}
