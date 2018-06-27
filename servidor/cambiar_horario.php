<!doctype html>
<html>
<head>
	<meta charset="utf-8">
	<title>Cambiar horario</title>
	<link rel='stylesheet' type='text/css' href='../normal.css' media='screen'>
</head>
<body>
	<table id='centrar'><tr><td><header>
	<a href='../index.php'><img src='../logopequeno.png' height=25% width=25%></img></a></header>
	<p>Cambiar horario:</p>
	<?php
		require_once '../nombres.php';

		$enlace = new mysqli($dbhost, $dbuser, $dbpsswd, $dbname);

		if($enlace->connect_errno){
			die("Failed connection");
		}

		if($_REQUEST['change'] === 'yes')
			require_once 'change_time.php';

		echo "
		<form action='cambiar_horario.php' method='post'>
		<fieldset>
		<input type='hidden' name='change' value='yes' />
		<p>El usuario:</p>
		<p><label>Usuario</label><br />
		<input type='text' name='admin' /></p>
		<p><label>Contraseña</label><br />
		<input type='password' name='adcont' /></p>
		<p>Da permiso para que se cambie el horario del usuario:</p>
		<p><label>Usuario</label><br />
		<input type='text' name='usuario' /></p>
		<p>Nuevo horario:</p>
		<p><label>Desde</label><br />
		<input type='time' name='desde_f' /></p>
		<p><label>Hasta</label><br />
		<input type='time' name='hasta_f' /></p>
		<input type='submit' value='Enviar'></p>
		</fieldset></form>";
		$enlace->close();
	?>
	</td></tr></table>
</body>
</html>
