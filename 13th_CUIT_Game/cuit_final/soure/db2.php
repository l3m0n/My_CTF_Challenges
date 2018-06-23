<?php
include_once 'config.php';

class DB {
	public $host;
	public $user;
	public $pass;
	public $dbname;
	public $db_conn;
	public $result;

	function __construct() {
		$this->host = $_ENV['MYSQL_DB_HOST'];
		$this->user = $_ENV['MYSQL_DB_USER'];
		$this->pass = $_ENV['MYSQL_DB_PASS'];
		$this->dbname = $_ENV['MYSQL_DB_NAME'];
		$this->db_conn = new MySQLi($this->host, $this->user, $this->pass, $this->dbname);
		if (mysqli_connect_errno()) {
			die("error connecting");
		}
		$this->db_conn->set_charset('utf8');
	}

	function select($s) {
		$r = array();
		$n = 0;
		$sql = "select * from log where s='?' limit 0,1";
		$this->result = $this->db_conn->prepare($sql);
		$this->result->bind_param('s', $s);
		if ($this->result->execute()) {
			$this->result->store_result();
			$this->result->bind_result($url);
			while ($this->result->fetch()) {
				$r[$n] = $url;
				$n++;
			}
		}
		return $r;
	}

	function insert($url, $randmon) {
		$sql = "insert into `log` (`url`,`s`) values (?,?)";
		$this->result = $this->db_conn->prepare($sql);
		$this->result->bind_param('ss', $url, $randmon);
		try {
			$this->result->execute();
		} catch (Exception $e) {
			die('错误');
		}
		return true;
	}

	function __destruct() {
		if (isset($this->result)) {
			$this->result->close();
		}
		$this->db_conn->close();
	}
}
