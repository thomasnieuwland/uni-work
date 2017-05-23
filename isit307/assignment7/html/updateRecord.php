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
    $stmt = $conn->prepare("SELECT * FROM shoes WHERE id = ?");
    $stmt->bind_param('i',$_REQUEST["id"]);
    $stmt->execute();
    $stmt->bind_result($id,$brand,$colour,$size,$price,$stock);
    $stmt->fetch();

    ?>
  <div class="navbar-inverse">
    <div class="container-fluid">
      <div class="navbar-header">
        <a class="navbar-brand" href="./index.php" >Assignment 7 - Shoe Store</a>
      </div>
    </div>
  </div>
  <div class="jumbotron">
    <div class="container-fluid">
      <div class="row">
        <div class="col-md-10 col-md-offset-1">
          <h2>Updating Record #<?php echo $id;?></h2>
        </div>
      </div>
      <div class="row">
        <form action="./update.php">
          <input type="hidden" name="id" value="<?php echo $id;?>">
          <input type="hidden" name="refer" value="<?php echo $_SERVER['HTTP_REFERER'];?>">
        <div class="col-md-2 col-md-offset-2">
          <h3>Brand</h3>
          <div class="form-group">
            <input type="text" class="form-control" name="brand" value="<?php echo $brand?>">
          </div>
        </div>
        <div class="col-md-1">
          <h3>Colour</h3>
          <div class="form-group">
            <input type="text" class="form-control" name="colour" value="<?php echo $colour;?>">
          </div>
        </div>
        <div class="col-md-1">
          <h3>Stock</h3>
          <div class="form-group">
            <input type="number" class="form-control" name="stock" min="0" max="30" value="<?php echo $stock;?>">
          </div>
        </div>
        <div class="col-md-1">
          <h3>Size</h3>
          <div class="form-group">
            <input type="number" class="form-control" name="size" min="5" max="13" value="<?php echo $size;?>">
          </div>
        </div>
        <div class="col-md-2">
          <h3>Cost</h3>
          <div class="form-group">
            <label for="cost">$</label>
            <input type="number" id="cost" name="cost" min="15" max="10000" value="<?php echo $price;?>">
          </div>
          <br><button type="submit" class="btn btn-primary btn-md btn-block">Commit Update</button>
          </form>
          <a href="javascript:history.back()" class="btn btn-danger btn-md btn-block">Cancel Update</a>
        </div>
      </div>
    </div>
  </div>
</body>
