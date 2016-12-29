<?php
error_reporting(E_ALL);
ini_set('display_errors', 1);
if (isset($_GET["serial"]) && isset($_GET["id_veliciny"]) && isset($_GET["hodnota"])){ 
	$cidlo = $_GET["serial"];
	$id_veliciny = $_GET["id_veliciny"];
	$hodnota = $_GET["hodnota"];

	if($hodnota < 45 && $hodnota > -30){
	    if($id_veliciny == 1) {	
			$xml = file_get_contents("http://reddriver.tmep.cz/?temp=" . $hodnota);
			$xmlText = "https://script.google.com/macros/s/AKfycbx0HXK9ciYqRig6dqfV2_JSCoDO9g4OBdM8ye1q_764Xxra7Us/exec?teplota=" . $hodnota;
			echo $xmlText;
			$xml2 = file_get_contents($xmlText);
	    }
	    if($id_veliciny == 3) {
			$xml = file_get_contents("http://reddriver.tmep.cz/?temp2=" . $hodnota);
	    }    
	}	

  if($hodnota < 95 && $hodnota > 30){
  	if($id_veliciny == 4) {
		$xml = file_get_contents("http://reddriver.tmep.cz/?humV=" . $hodnota);
	}
  }   
}else{
	echo "Neni predan nektery z parametru metodou GET!<br>";
}

$urlTeplotaVenku = "https://tmep.cz/vystup-json.php?id=423&export_key=byrl2kn1vq"; 
$urlTeplotaObyvak = "https://tmep.cz/vystup-json.php?id=467&export_key=r9uqxo5znf";
$json1 = file_get_contents($urlTeplotaVenku);
$json2 = file_get_contents($urlTeplotaObyvak);
$teplotaVenku = json_decode($json1);
$teplotaObyvak = json_decode($json2);

echo "Předávám: ";
if(is_numeric($teplotaVenku->teplota)){
  echo "%" . round($teplotaVenku->teplota, 1) . ";" . round($teplotaObyvak->teplota, 1) . "*";
}
?>
