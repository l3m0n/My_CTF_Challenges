<?php
include_once 'config.php';

//限制ip访问

$redis = new Redis();
$redis->connect($redis_config['host'], $redis_config['port']);
$redis->auth($redis_config['redis_key']);
//这个key记录该ip的访问次数 也可改成用户id
$key = get_real_ip();

//限制次数为2
$limit = 3;

$check = $redis->exists($key);
if ($check) {
	$redis->incr($key);
	$count = $redis->get($key);
	if ($count > $limit) {
		$result = array('status' => 0, 'msg' => '');
		$result['msg'] = '请求太频繁啦,请稍后再试!';
		header('Content-Type: application/json');
		echo json_encode($result);
		exit();
	}
} else {
	$redis->incr($key);
	//限制时间为60秒
	$redis->expire($key, 15);
}

$count = $redis->get($key);

//获取客户端真实ip地址
function get_real_ip() {
	static $realip;
	if (isset($_SERVER)) {
		// if (isset($_SERVER['HTTP_X_FORWARDED_FOR'])) {
		// 	$realip = $_SERVER['HTTP_X_FORWARDED_FOR'];
		// } else if (isset($_SERVER['HTTP_CLIENT_IP'])) {
		// 	$realip = $_SERVER['HTTP_CLIENT_IP'];
		// } else {
		// 	$realip = $_SERVER['REMOTE_ADDR'];
		// }
		$realip = $_SERVER['REMOTE_ADDR'];
	} else {
		// if (getenv('HTTP_X_FORWARDED_FOR')) {
		// 	$realip = getenv('HTTP_X_FORWARDED_FOR');
		// } else if (getenv('HTTP_CLIENT_IP')) {
		// 	$realip = getenv('HTTP_CLIENT_IP');
		// } else {
		// 	$realip = getenv('REMOTE_ADDR');
		// }
		$realip = getenv('REMOTE_ADDR');
	}
	return $realip;
}
?>