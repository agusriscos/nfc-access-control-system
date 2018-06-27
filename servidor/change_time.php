<?php

	$saltar = 0;
	require_once 'permiso.php';
	$no_query = 0; //si las comprobaciones fallan entonces no_query igual a uno.
	if($no_query !== 1){
		if($saltar === 0){
		if($entrada = $enlace->query("SELECT * FROM " . $dbusuarios . " WHERE usuario='" . $_POST['usuario'] . "'")){
			if($entrada->num_rows === 0){
				echo "<p><div id='error'>El usuario no existe</div></p>";
			}else{
				try{
					$desde = $_POST['desde_f'];
					$hasta = $_POST['hasta_f'];
					$enlace->query("UPDATE " . $dbusuarios . " SET desde='" . $desde . "',hasta='" . $hasta . "' WHERE usuario='" . $_POST['usuario'] . "'");
					echo "<p><div id='ok'>Se ha cambiado correctamente el horario</div></p>";
				}catch(Exception $error){
					die("Error al actualizar: " . $error->getMessage());
				}
			$entrada->close();
			}
		}else {
			die("Error al realizar la búsqueda");
		}
		}
	
	}
	
?>