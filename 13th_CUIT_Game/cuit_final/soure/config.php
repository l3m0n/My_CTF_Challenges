<?php
//error_reporting(0);

$redis_config = array();
$redis_config['host'] = $_ENV['REDIS_HOST'];
$redis_config['port'] = 6379;
$redis_config['redis_key'] = 'syc_ip';
