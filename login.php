<!doctype html>
<?php
session_start();
?>
<html>
<head>
  <meta charset="utf-8">
  <title>Přihlášení</title>
  <link rel="stylesheet" href="normalize.css">
  <link rel="stylesheet" href="style/style.css">
</head>
<body> 
  <section class="loginform cf">
    <form name="login" action="login.php" method="post">
    	<label for="password">Zadejte heslo:</label>
      <input type="password" name="heslo" placeholder="password" required>
      <input type="submit" name="odeslat" value="Přihlásit">
      <?php
        if (isset($_POST["heslo"])){    
          if ($_POST["heslo"]=="gamepark"){
            $_SESSION["prihlasen"]="ano"; 
            header("location: index.php");
          }else{
           echo"Špatné heslo!";
          }
        }    
      ?>
    </form>
  </section>
</body>
</html>