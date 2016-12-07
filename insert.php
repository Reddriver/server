<?php
error_reporting(E_ALL);
ini_set('display_errors', 1);
if (isset($_GET["serial"]) && isset($_GET["id_veliciny"]) && isset($_GET["hodnota"])){ 
	$cidlo = $_GET["serial"];
	$id_veliciny = $_GET["id_veliciny"];
	$hodnota = $_GET["hodnota"];

	//mysql
	$username = "luka";
	$password = "gamepark";
	$hostname = "localhost"; 
	$dbname = "luka";

  if($hodnota < 45 && $hodnota > -30){
    if($id_veliciny == 1) {	
	$retVal = rrd_update("/var/www/html/teplotyVenku.rrd", (array)"N:$hodnota");	    
	echo "Ulozeno do RRD = " . $retVal;	
	$xml = file_get_contents("http://reddriver.tmep.cz/?temp=" . $hodnota);
    }
    if($id_veliciny == 3) {
	$retVal = rrd_update("/var/www/html/teploty.rrd", (array)"N:$hodnota");
	$xml = file_get_contents("http://reddriver.tmep.cz/?temp2=" . $hodnota);
      echo "Ulozeno do RRD = " . $retVal;
    }    
	}	

  if($hodnota < 95 && $hodnota > 30){
  	if($id_veliciny == 4) {
		$retVal = rrd_update("/var/www/html/vlhkosti.rrd", (array)"N:$hodnota");
      		echo "Ulozeno do RRD = " . $retVal;
		$xml = file_get_contents("http://reddriver.tmep.cz/?humV=" . $hodnota);
	 }
  }  
  
  echo "<br>";
  
	$conn =  mysqli_connect($hostname, $username, $password, $dbname);
	$sql = "INSERT INTO zaznamy (cidlo, id_veliciny, hodnota) VALUES ('" . $cidlo . "', " . $id_veliciny . ", " . $hodnota . ")";
	$sql2 = "select hodnota from zaznamy where id_veliciny = 1 order by cas DESC LIMIT 1";
	$sql3 = "select hodnota from zaznamy where id_veliciny = 5 order by cas DESC LIMIT 1";
	
	if ($conn->query($sql) === TRUE) {
		echo " Zaznam do mysql ulozen!";
	} else {
		echo "Error: " . $sql . "<br>" . $conn->error;
	}
	
	
	$result = $conn->query($sql2);
	$result2 = $conn->query($sql3);
	$predani = "";

	if ($result->num_rows > 0) {
	    while($row = $result->fetch_assoc()) {
	        $predani = $predani . number_format($row["hodnota"],2);
	    }
	} else {
    	echo "0 results";
	}
	$predani = $predani . ";";
	if ($result2->num_rows > 0) {
		while($row2 = $result2->fetch_assoc()) {
			 $predani = $predani . number_format($row2["hodnota"],2);
		}
	} else {
		echo "0 results";
	}
	echo "<br> Predan zaznam pro domaciMeteo: %" . $predani . "*";

	$conn->close();
}else{
	echo "Neni predan nektery z parametru metodou GET!";
}
?>
