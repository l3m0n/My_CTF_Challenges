<?php
$redis_ip = gethostbyname('redis');
putenv("REDIS_HOST=" . $redis_ip);
phpinfo();