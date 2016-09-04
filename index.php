<!doctype html>
<?php
session_start();
ini_set("display_errors", 1);
error_reporting(E_ERROR | E_WARNING);
if (!isset($_SESSION["prihlasen"])):
  header("location: login.php");
endif;
?>
<html>
	<head>
		<meta charset="utf-8">
		<script src="package/jquery-1.11.3.min.js" type="text/javascript"></script>
		<script src="package/jquery.idTabs.min.js" type="text/javascript"></script>
		<link rel="stylesheet" type="text/css" href="style/design.css">
		<title>MeteoStation</title>
    <?php 
    //mysql
    $username = "luka";
    $password = "gamepark";
    $hostname = "localhost"; 
    $dbname = "luka";

    $conn =  mysqli_connect($hostname, $username, $password, $dbname);
    $conn->query("SET NAMES UTF8"); 
    
    $sqlTeplotaVenku = "SELECT * FROM zaznamy where id_veliciny = 1 order by cas DESC LIMIT 1";
    $sqlTeplotaDoma = "SELECT * FROM zaznamy where id_veliciny = 3 order by cas DESC LIMIT 1";
    $sqlVlhkostDoma = "SELECT * FROM zaznamy where id_veliciny = 4 order by cas DESC LIMIT 1";
    ?>
	</head>
	<body>
    <div class="obaleni">
      <div class="hlavicka">
        <div class="budiky">
          <?php  
          function nastavBudik($conn, $sql, $text){              
            $result = $conn->query($sql);
            $row = $result->fetch_assoc();
            $cas = "";
            echo ' 
            <div class="budik">
              <div class="de">
                  <div class="den">
                    <div class="dene">
                      <div class="denem">
                        <div class="deneme">';
            echo number_format($row['hodnota'], 1); 
            $cas = $row['cas'];
            echo '<strong>&deg;</strong>
                        </div>
                      </div>
                    </div>
                  </div>
              </div>
            <p style="padding-top:1px;">';
            echo $text . $cas;
            echo '</p></div>';
          }
          $this.nastavBudik($conn, $sqlTeplotaVenku, "Teplota venku:"); 
          $this.nastavBudik($conn, $sqlTeplotaDoma, "Teplota doma:");
          $this.nastavBudik($conn, $sqlVlhkostDoma, "Vlhkost doma:"); 
          ?>          
      </div>       
      <div class="slunce">
      <?php        
        $dnes = date("j.n.Y", time());
        $sql1 = "select * from slunce where datum = '" . $dnes . "'";
        $result = $conn->query($sql1);
        $den = "";
        if ($result->num_rows > 0) {
            while($row = $result->fetch_assoc()) {              
              echo "<p>Dnes je " . $row["den"] . " " . $row["datum"] . " východ slunce: " . $row["vychod"] . " západ slunce: " . $row["zapad"] . "</p>";
            }
        } else {
            echo "0 results";
        } 
      ?>             
      </div>
    </div>
		<div id="usual1" class="usual">    
			<ul>    
        <?php
        if(!isset($_GET['zobrazit']) && !isset($_SESSION["posledniTab"])){ 
          echo '<li><a href="#tab1" class="selected">Dvě hodiny</a></li>';
        }else{
          echo '<li><a href="#tab1">Dvě hodiny</a></li>';
        }
        ?>
        <li><a href="#tab2">Čtyři hodiny</a></li> 
        <li><a href="#tab3">Den</a></li>
        <li><a href="#tab4">Týden</a></li>  
        <li><a href="#tab5">Měsíc</a></li>
        <li><a href="#tab6">Rok</a></li>         
        <?php
        if(!isset($_GET['zobrazit'])){
          echo '<li><a href="#tab7">Poslední záznamy</a></li>';
        }else{
          echo '<li><a href="#tab7" class="selected">Poslední záznamy</a></li>';
        }
        ?>
        <li><a href="#tab8">Kamery</a></li>
        <li><a href="#tab9">Slunce</a></li>
        <li><a href="logout.php">Odhlášení</a></li>   	
			</ul> 
			<div id="tab1" style="display: block; ">
        <p style="text-align:center"><img src="grafy/tempVenku_2hour.png" alt="tempVenku_2hour"></p>
				<p style="text-align:center"><img src="grafy/temp_2hour.png" alt="temp_2hour"></p>
				<p style="text-align:center"><img src="grafy/hum_2hour.png" alt="hum_2hour"></p> 
			</div> 

  		<div id="tab2" style="display: none; ">
        <p style="text-align:center"><img src="grafy/tempVenku_hourly.png" alt="tempVenku_hourly"></p>
        <p style="text-align:center"><img src="grafy/temp_hourly.png" alt="temp_hourly"></p>
        <p style="text-align:center"><img src="grafy/hum_hourly.png" alt="hum_hourly"></p>
      </div> 

			<div id="tab3" style="display: none; ">
        <p style="text-align:center"><img src="grafy/tempVenku_daily.png" alt="tempVenku_daily"></p>
				<p style="text-align:center"><img src="grafy/temp_daily.png" alt="temp_daily"></p>
				<p style="text-align:center"><img src="grafy/hum_daily.png" alt="hum_daily"></p>
  		</div> 

  		<div id="tab4" style="display: none; ">
        <p style="text-align:center"><img src="grafy/tempVenku_weekly.png" alt="tempVenku_weekly"></p>
        <p style="text-align:center"><img src="grafy/temp_weekly.png" alt="temp_weekly"></p>
        <p style="text-align:center"><img src="grafy/hum_weekly.png" alt="hum_weekly"></p>
  		</div> 

  		<div id="tab5" style="display: none; ">
        <p style="text-align:center"><img src="grafy/tempVenku_monthly.png" alt="tempVenku_monthly"></p>
        <p style="text-align:center"><img src="grafy/temp_monthly.png" alt="temp_monthly"></p>
        <p style="text-align:center"><img src="grafy/hum_monthly.png" alt="hum_monthly"></p>
  		</div> 

  		<div id="tab6" style="display: none; ">
        <p style="text-align:center"><img src="grafy/tempVenku_yearly.png" alt="tempVenku_yearly"></p>
    	  <p style="text-align:center"><img src="grafy/temp_yearly.png" alt="temp_yearly"></p>
        <p style="text-align:center"><img src="grafy/hum_yearly.png" alt="hum_yearly"></p>
  		</div> 
      
     <div id="tab7" style="display: none; ">
         <form method="get">
            <fieldset>
              <legend>Poslední záznamy</legend>
              <table>
              <tr>
              <td>
              <label for="id">Zadejte id_veliciny:</label>
              </td><td>
              <select class="id" id="id" name="id" size="1" style="width:150px;">
                <option value="1">Teplota venku
                <option value="3">Teplota doma              
                <option value="4">Vlhkost doma
              </select>
              </td></tr><tr><td>
              <label class="labelPocet" for="text">Počet posledních záznamů:</label>
              </td><td>
              <input class="text" id="text" type="text" name="pocet" style="width:146px;">
              </td></tr><tr><td>
              <input type="submit" name="zobrazit" value="Zobrazit" />
              </td></tr>
              <p style="text-align:center"> 
              <?php 
              if(isset($_GET['id']) && isset($_GET['pocet'])){
                if(trim($_GET['pocet']) != ''){
                  $sqlVypis = "SELECT * FROM zaznamy where id_veliciny = " . $_GET['id']. " order by cas DESC LIMIT " . $_GET['pocet']. "";
                  $result = $conn->query($sqlVypis);                  
                  echo '<table cellpadding="0" cellspacing="0" border="1" class="db-table">';
                  echo '
                  <tr>
                  <th>ID</th>
                  <th>Čas</th>
                  <th>Čidlo</th>
                  <th>ID_veličiny</th>
                  <th>Hodnota</th>
                  </tr>';
                  $i = 0;
                  while($row = $result->fetch_assoc()) {
                    if ($i%2==0){
                      echo "<tr bgcolor='#7a7a7a'>";
                    }else{
                      echo "<tr>";
                    }
                    $i++;
                        echo "<td style='padding-left:20px;'>" . $row["id_zaznamu"] . "</td>";
                        echo "<td align='center'>" . $row["cas"] . "</td>";
                        echo "<td align='center'>" . $row["cidlo"] . "</td>";
                        echo "<td align='center'>" . $row["id_veliciny"] . "</td>";
                        echo "<td align='center'>" . $row["hodnota"] . "</td>";
                      echo "<tr>";                 
                  }
                }          
              }  
              ?> 
              </p>
              </table> 
            </fieldset>
        </form> 				
  		</div>
      <div id="tab8" style="display: none; ">
        <?php
        $path1 = "cam/imago/mini/";
        $path2 = "cam/vivotek/mini/";
        
        function vratNazevSouboru($path){                
          $latest_ctime = 0;
          $latest_filename = '';
          $d = dir($path);
          while (false !== ($entry = $d->read())) {
            $filepath = "{$path}/{$entry}";
            // could do also other checks than just checking whether the entry is a file
            if (is_file($filepath) && filectime($filepath) > $latest_ctime) {
              $latest_ctime = filectime($filepath);
              $latest_filename = $entry;
            }
          }
          return $latest_filename;   
        }
                
        if(file_exists($path1) && file_exists($path2)){
          $nazevSouboru1 = $this.vratNazevSouboru($path1);
          $nazevSouboru2 = $this.vratNazevSouboru($path2);        
          echo "<p style='text-align:center'><a href='cam/imago/" . $nazevSouboru1 . "' target='_blank'><img src='" . $path1 . $nazevSouboru1 . "' alt='cam_imago'></a></p>" ;
          echo "<p style='text-align:center'><a href='cam/vivotek/" . $nazevSouboru2 . "' target='_blank'><img src='" . $path2 . $nazevSouboru2 . "' alt='cam_vivotek'></a></p>" ;
        }else{
          echo "<p style='text-align:center'>Nelze načíst miniatury z kamer.</p>";
        } 
        ?>
  		</div> 
      <div id="tab9" style="display: none;">
        <fieldset>
          <legend>Východy a západy Slunce</legend>            
          <p style='text-align:center'><img src='graf.jpeg' alt='graf'></p>          
          <table cellpadding="0" cellspacing="0" border="1" class="db-table2">
            <tr>
              <th>Datum</th>
              <th>Den</th>
              <th>Východ</th>
              <th>Západ</th>
              <th>Změna času</th>
            </tr>
          <?php
            $sqlVypis = "SELECT * FROM slunce";
            $result = $conn->query($sqlVypis);
            if ($result->num_rows > 0) {
              $i = 0;
              while($row = $result->fetch_assoc()) {
                if ($i%2==0){
                  echo "<tr bgcolor='#7a7a7a'>";
                }else{
                  echo "<tr>";
                }
                    $i++;
                    echo "<td style='padding-left:20px;'>" . $row["datum"] . "</td>";
                    echo "<td align='center'>" . $row["den"] . "</td>";
                    echo "<td align='center'>" . $row["vychod"] . "</td>";
                    echo "<td align='center'>" . $row["zapad"] . "</td>";
                    echo "<td align='center'>" . $row["prechod"] . "</td>";
                  echo "<tr>";                 
              }
            }
          ?>
          </table>
      </div>  
    </div> 
  	<script type="text/javascript">$("#usual1 ul").idTabs("tabs2");</script>
    </div>
	</body>
</html>