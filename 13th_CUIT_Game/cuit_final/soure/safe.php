<?php

$GLOBALS['msg'] = '';
function check_domain($url) {
	// $match_result = preg_match('/^(http|https)?:\/\/.*(\/)?.*$/', $url);
	// if (!$match_result) {
	// 	$GLOBALS['msg'] = '500: url fomat error';
	// 	return false;
	// }
	try
	{
		$url_parse = parse_url($url);
	} catch (Exception $e) {
		$GLOBALS['msg'] = '500: URL格式错误';
		return false;
	}

	if (!isset($url_parse['host']) && empty($url_parse['host'])) {
		$GLOBALS['msg'] = '500: URL格式错误';
		return false;
	} else {
		return $url_parse['host'];
	}
}

function check_inner_ip($hostname) {
	$ip = gethostbyname($hostname);
	$int_ip = ip2long($ip);
	return ip2long('127.0.0.0') >> 24 == $int_ip >> 24 || ip2long('10.0.0.0') >> 24 == $int_ip >> 24 || ip2long('172.16.0.0') >> 20 == $int_ip >> 20 || ip2long('192.168.0.0') >> 16 == $int_ip >> 16;
}

// function check_double_inner_ip($hostname) {
// 	$host_info = dns_get_record($hostname);
// 	$flag = false;
// 	foreach ($host_info as $k => $v) {
// 		if (check_inner_ip($v['ip']) === true) {
// 			$flag = true;
// 		}
// 	}
// 	return $flag;
// }

function safe_request_url($url) {
	$hostname = check_domain($url);
	if ($hostname === false) {
		return false;
	}

	//防止双绑定ip
	// if (check_double_inner_ip($hostname)) {
	// 	$GLOBALS['msg'] = '403: 禁止访问';
	// 	return false;
	// }

	if (check_inner_ip($hostname)) {
		$GLOBALS['msg'] = '403: 禁止访问';

		return false;
	} else {

		$ch = curl_init();
		curl_setopt($ch, CURLOPT_URL, $url);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		//设置超时2s
		curl_setopt($ch, CURLOPT_TIMEOUT, 2);
		//设置dns超时时间
		curl_setopt($ch, CURLOPT_DNS_USE_GLOBAL_CACHE, false);
		curl_setopt($ch, CURLOPT_DNS_CACHE_TIMEOUT, 0);
		curl_setopt($ch, CURLOPT_HEADER, 0);
		$output = curl_exec($ch);
		$result_info = curl_getinfo($ch);
		if ($result_info['redirect_url']) {
			safe_request_url($result_info['redirect_url']);
		}
		curl_close($ch);

		//print_r($output);
		return true;
	}
}

//$url = '';
//$url = 'http://127.0.0.1/';
safe_request_url($_GET['u']);
//safe_request_url('gopher://test.iamstudy.cn:83/_aaaaaa%0akkkkk');


?>
