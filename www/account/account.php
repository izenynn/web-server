<?php
session_start();
include("login.class.php");
$login=new login();
$login->init();
?>

<html>
	<head>
		<title>account</title>
	</head>
	<body>
		<p>This is your account panel!</p>
	</body>
</html>
