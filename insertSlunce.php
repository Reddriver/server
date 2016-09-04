<!doctype html>
<html>
	<head>
		<meta charset="utf-8">
  </head>
<?php 
//data z http://wall.cz/vychod-a-zapad-slunce-graf

error_reporting(E_ALL);
ini_set('display_errors', 1);

$datum = "";
$den = "";
$vychod = "";
$zapad = "";
$row = 1;
         
//mysql
$username = "luka";
$password = "gamepark";
$hostname = "localhost"; 
$dbname = "luka";
$conn =  mysqli_connect($hostname, $username, $password, $dbname);

header("Content-type: text/html; charset=utf-8");
mysqli_set_charset( $conn, 'utf-8');

$conn->query("SET NAMES UTF8"); 

$dnes = date("j.n.Y", time());
$sql1 = "select * from slunce where datum = '" . $dnes . "'";
$result = $conn->query($sql1);
$den = "";
if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
      switch ($row["den"]) {
        case "Po":
          $den = "Pondělí"; 
          break;
        case "Út":
          $den = "Úterý"; 
          break;
        case "St":
          $den = "Středa"; 
          break;
        case "Čt":
          $den = "Čtvrtek"; 
          break;
        case "Pá":
          $den = "Pátek"; 
          break;
        case "So":
          $den = "Sobota"; 
          break;
        case "Ne":
          $den = "Neděle"; 
          break;
      } 
      echo "datum: " . $row["datum"] . " den: " . $den . " východ: " . $row["vychod"] . " západ: " . $row["zapad"];
    }
} else {
    echo "0 results";
}

// if (($handle = fopen("dataCSV.csv", "r")) !== FALSE) {
//     while (($data = fgetcsv($handle, 1000, ";")) !== FALSE) {
//         $num = count($data);
//         $row++; 
//         for ($c=0; $c < $num; $c++) {
//             switch ($c) {
//               case 0:
//                 $datum = $data[$c]; 
//                 break;
//               case 1:
//                 $den = $data[$c];
//                 break;
//               case 2:
//                 $vychod = $data[$c];
//                 break;
//               case 3:
//                 $zapad = $data[$c];
//                 break;
//             }       
//         }
//         $sql = "INSERT INTO slunce (datum, den, vychod, zapad) VALUES ('" . $datum . "', '" . $den . "', '" . $vychod .  "', '" . $zapad . "')";
//         echo $sql;
//         if ($conn->query($sql) === TRUE) {
// 		      //echo " Zaznam do mysql ulozen!";
// 	     } else {
// 		      echo "Error: " . $sql . "<br>" . $conn->error;
// 	     }
//         echo "<br />\n";
//     }
//     fclose($handle);
// }

?>
</html>