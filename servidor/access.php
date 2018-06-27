<?php
require_once 'nombres.php';
require_once 'conectar.php';
$hor = date('His',time());
$time = date('His',strtotime($hor));
	if($entrada = $enlace->query("SELECT * FROM " . $dbusuarios . " WHERE usuario='" . $_REQUEST['user'] . "'")){
		if($entrada->num_rows === 0){
			print("False");
		}else{
	        while($fila = $entrada->fetch_assoc()){ // La estructura foreach no funciona en la version PHP 5.5.54
				$desde=date('His',strtotime($fila['desde']));
				$hasta=date('His',strtotime($fila['hasta']));
				if(($time > $desde) && ($time < $hasta)){
					echo("OK");
				}
				else{
					echo("False");
				}
			}
		}
		$entrada->close();
	} else {
		die("Error al realizar la búsqueda");
	}

	$enlace->close();
?>