<?php
session_start();
?>

<form name="frm_login" method="post" action="login.php">
	<p>Usuario: </p><input type="text" size="10" name="user" /><br />
	<p>Clave: </p><input type="text" size="10" name="pass" />
	<input type="submit" name="submit" value="Login" />
</form>

<?php
if (isset($_GET['error'])) {
	echo '<b>Usuario o clave incorrecta</b>';
}
?>
