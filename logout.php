<!doctype html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Odhlášení</title>
    <style type="text/css">
       BODY { background: url("wood_pattern.png")}
    </style>
  </head>
  <body>
    <?php
    session_start();
    session_destroy();
    header( "refresh:5;url=login.php" );
    ?>
    <h3>Uživatel odhlášen!</h3>  
  </body>
</html>