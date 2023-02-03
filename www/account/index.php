<?php
session_start();
?>

<form name="frm_login" method="post" action="login.php">
	<p>User: </p><input type="text" size="10" name="user" /><br />
	<p>Password: </p><input type="text" size="10" name="pass" /><br /><br />
	<input type="submit" name="submit" value="Login" />
</form>

<?php
if (isset($_GET['error'])) {
	echo '<b>Wrong usename or password</b>';
}
?>
