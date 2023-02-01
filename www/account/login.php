<?php
session_start();
include("login.class.php");
$login=new login();
$login->inicia(3600, $_POST['user'], $_POST['pass']);
?>
