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

	function select($sql) {
		$r = array();
		$n = 0;
		try {
			$this->result = $this->db_conn->query($sql);
		} catch (Exception $e) {
			die('错误');
		}
		while ($row = $this->result->fetch_assoc()) {
			$r[$n] = $row;
			$n++;
		}
		return $r;
	}

	function insert($sql) {
		try {
			$this->db_conn->query($sql);
		} catch (Exception $e) {
			die('错误');
		}
		return true;
	}

	function __destruct() {
		if (isset($this->result)) {
			$this->result->free();
		}
		$this->db_conn->close();
	}
}

// var_dump($db->select('select * from log where s=\'' . $_GET['s'] . '\' limit 0,1'));
