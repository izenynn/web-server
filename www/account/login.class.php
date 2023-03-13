<?php
class login {
// Inicia sesion
public function init($time=3600, $user=NULL, $pass=NULL) { 
	if ($user==NULL && $pass==NULL) {
		// verify session
		if (isset($_SESSION['userid'])) {
			//echo "<p>Logged in, redirecting...</p>";
		} else {
			// verify cookie
			if (isset($_COOKIE['userid'])) {
				// restore session
				$_SESSION['userid']=$_COOKIE['userid'];
			} else {
				// if not session return to index
				header( "Location: index.php" );
			}
		}
	} else {
		$this->verify_user($time, $user, $pass);
	}
}

// verify login
private function verify_user($time, $user, $pass) {
	if ($user==="admin" && $pass==="password") {
		// Si la clave es correcta
		$userid=$this->hash_user($user);
		setcookie("userid", $userid, time()+$time);
		$_SESSION['userid']=$userid;
		header( "Location: account.php" );
	} else {
		// if user/password is wrong
		header( "Location: index.php?error=1" );
	}
}

// hash user
private function hash_user($user) {
	return md5($user);
}

}
?>
