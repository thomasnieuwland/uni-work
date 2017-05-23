<head>
  <meta charset="utf-8">
  <title>Assignment 7</title>

  <!-- Bootstrap Inclusions -->
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" integrity="sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u" crossorigin="anonymous">
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css" integrity="sha384-rHyoN1iRsVXV4nD0JutlnGaslCJuC7uwjduW9SVrLvRYooPp2bWYgmgJQIXwl/Sp" crossorigin="anonymous">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
  <?php
    $servername = "localhost";
    $username = "root";
    $password = "";
    $dbname = "assignment7";
    $conn = new mysqli($servername,$username,$password,$dbname);
    if ($conn->connect_error){
      die("Connection Failed: ". $conn->connect_error);
    }
   ?>
  <div class="navbar-inverse">
    <div class="container-fluid">
      <div class="navbar-header">
        <a class="navbar-brand" href="./index.php" >Assignment 7 - Shoe Store</a>
      </div>
      <div class="nav navbar-nav navbar-right">
        <div class="btn-group">
          <form action="./addRecord.php">
              <button class="btn btn-warning navbar-btn" type="submit">Add Record</button>&nbsp;&nbsp;&nbsp;
          </form>
          <form action="./populateDB.php">
              <button class="btn btn-success navbar-btn" type="submit">Generate New Data</button>&nbsp;&nbsp;&nbsp;
          </form>
        </div>
      </div>
    </div>
  </div>
  <div class="jumbotron">
    <div class="container-fluid">
      <div class="row">
        <div class="col-md-10 col-md-offset-1">
          <h2>Find a shoe..</h2>
        </div>
      </div>
      <div class="row">
        <form action="<?php echo $_SERVER['PHP_SELF']; ?>">
        <div class="col-md-2 col-md-offset-2">
          <h3>Brand</h3>
          <?php
            $sql = "SELECT DISTINCT brand FROM shoes";
            $result = $conn->query($sql);

            if ($result->num_rows > 0) {
              while($row = $result->fetch_assoc()) {
                  echo "<div class=\"checkbox\">";
                  echo "<label><input type=\"checkbox\" name=\"brand[]\" value=\"".$row["brand"]."\"";
                  if(in_array($row["brand"],$_REQUEST[brand])){echo "checked=\"checked\"";}
                  echo ">".$row["brand"]."</label>";
                  echo "</div>";
              }
            }
           ?>
        </div>
        <div class="col-md-2">
          <h3>Colour</h3>
          <div class="row">
            <div class="col-md-6">
              <?php
                $sql = "SELECT DISTINCT colour FROM shoes";
                $result = $conn->query($sql);

                if ($result->num_rows > 0) {
                  $cnt = 0;
                  while($row = $result->fetch_assoc()) {
                    if ($cnt == 10){
                      echo "</div>";
                      echo "<div class=\"col-md-6\">";
                    }
                      echo "<div class=\"checkbox\">";
                      echo "<label><input type=\"checkbox\" name=\"colour[]\" value=\"".$row["colour"]."\"";
                      if(in_array($row["colour"],$_REQUEST[colour])){echo "checked=\"checked\"";}
                      echo ">".$row["colour"]."</label>";
                      echo "</div>";
                      $cnt++;
                  }
                }
               ?>
            </div>
          </div>
        </div>
        <div class="col-md-1">
          <h3>Size</h3>
          <?php
            $sql = "SELECT DISTINCT size FROM shoes";
            $result = $conn->query($sql);

            if ($result->num_rows > 0) {
              while($row = $result->fetch_assoc()) {
                  echo "<div class=\"checkbox\">";
                  echo "<label><input type=\"checkbox\" name=\"size[]\" value=\"".$row["size"]."\"";
                  if(in_array($row["size"],$_REQUEST[size])){echo "checked=\"checked\"";}
                  echo ">".$row["size"]."</label>";
                  echo "</div>";
              }
            }
           ?>
        </div>
        <div class="col-md-2">
          <h3>Cost</h3>
          <div class="form-group">
            <span>Less than or equal to $</span></br>
            <label for="cost">$</label>
            <input type="number" id="cost" name="cost" min="15" max="10000" value=
            <?php
              if(isset($_REQUEST['cost'])){
                echo "\"".$_REQUEST['cost']."\"";
              }

              else{
                echo "10000";
              }
            ?>>
          </div>
          <br><button type="submit" class="btn btn-primary btn-md btn-block">Search</button>
          </form>
          <form action="./index.php">
            <br><button type="submit" class="btn btn-danger btn-md btn-block">Reset Form</button>
          </form>
          <br>
        </div>
      </div>
    </div>
    <div class="row">
      <div class="col-md-7 col-md-offset-2">
        <br><br><br><br><br>
        <?php
          if(!empty($_REQUEST)){
            $brands = $_REQUEST['brand'];
            $colours = $_REQUEST['colour'];
            $sizes = $_REQUEST['size'];
            $cost = $_REQUEST['cost'];
            $stmt = $conn->prepare("TRUNCATE TABLE searched");
            $stmt->execute();
            $stmt = $conn->prepare("INSERT INTO searched(id,brand,colour,size,price,stock) SELECT id,brand,colour,size,price,stock FROM shoes");
            $stmt->execute();
            if(!empty($brands)){
              $stmt = $conn->prepare("UPDATE searched SET keep = 1 WHERE brand =?");
              $stmt->bind_param('s',$brand);
              foreach($brands as $brand){
                $stmt->execute();
              }

              $stmt = $conn->prepare("DELETE FROM searched WHERE keep = 0");
              $stmt->execute();
              $stmt = $conn->prepare("UPDATE searched SET keep = 0");
              $stmt->execute();
            }

            if(!empty($colours)){
              $stmt = $conn->prepare("UPDATE searched SET keep = 1 WHERE colour =?");
              $stmt->bind_param('s',$colour);
              foreach($colours as $colour){
                $stmt->execute();
              }

              $stmt = $conn->prepare("DELETE FROM searched WHERE keep = 0");
              $stmt->execute();
              $stmt = $conn->prepare("UPDATE searched SET keep = 0");
              $stmt->execute();
            }

            if(!empty($sizes)){
              $stmt = $conn->prepare("UPDATE searched SET keep = 1 WHERE size =?");
              $stmt->bind_param('i',$size);
              foreach($sizes as $size){
                $stmt->execute();
              }

              $stmt = $conn->prepare("DELETE FROM searched WHERE keep = 0");
              $stmt->execute();
              $stmt = $conn->prepare("UPDATE searched SET keep = 0");
              $stmt->execute();
            }


            $stmt = $conn->prepare("DELETE FROM searched WHERE price >= ?");
            $stmt->bind_param('d',$cost);
            $stmt->execute();

            $sql = "SELECT * FROM searched ORDER BY price ASC";
            $result = mysqli_query($conn, $sql);
            if (mysqli_num_rows($result) > 0) {
              if(mysqli_num_rows($result) > 1){
                echo "<h4>".mysqli_num_rows($result)." results found!</h4>";
              }
              else{
                echo "<h4>".mysqli_num_rows($result)." result found!</h4>";
              }
              echo "<div class=\"table-responsive t\">";
              echo "<table class=\"table table-bordered table-striped\">";
              echo "<tr><th>ID</th><th>Brand</th><th>Colour</th><th>Size</th><th>Price</th><th>Stock</th><th>Options</th></tr>";
              while($row = mysqli_fetch_assoc($result)) {
                echo "<tr><td>".$row["id"]."</td><td>".$row["brand"]."</td><td>".$row["colour"]."</td><td>".$row["size"]."</td><td>".$row["price"]."</td><td>".$row["stock"]."</td><td>";
                echo "<div class=\"btn-group\">";
                echo "<form action=\"./updateRecord.php\"><input type=\"hidden\" name=\"id\" value=\"".$row["id"]."\"><input type=\"submit\" value=\"Update Record\" class=\"btn btn-success btn-sm\"></form>";
                echo "&nbsp&nbsp&nbsp";
                echo "<form action=\"./removeRecord.php\"><input type=\"hidden\" name=\"id\" value=\"".$row["id"]."\"><input type=\"submit\" value=\"Delete Record\" class=\"btn btn-danger btn-sm\"></form>";
                echo "</div></td></tr>";
              }
            }
            else {
              echo "<h4>0 results found</h4>";
            }
          }

          $conn->close();
        ?>
      </div>
    </div>
  </div>
</body>
