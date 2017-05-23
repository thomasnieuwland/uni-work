<head>
  <meta charset="utf-8">
  <title>Assignment 7</title>

  <!-- Bootstrap Inclusions -->
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" integrity="sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u" crossorigin="anonymous">
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css" integrity="sha384-rHyoN1iRsVXV4nD0JutlnGaslCJuC7uwjduW9SVrLvRYooPp2bWYgmgJQIXwl/Sp" crossorigin="anonymous">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
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
          <h2>Adding Record</h2>
        </div>
      </div>
      <div class="row">
        <form action="./add.php">
        <div class="col-md-2 col-md-offset-2">
          <h3>Brand</h3>
          <div class="form-group">
            <input type="text" class="form-control" name="brand" value="" required>
          </div>
        </div>
        <div class="col-md-1">
          <h3>Colour</h3>
          <div class="form-group">
            <input type="text" class="form-control" name="colour" value="" required>
          </div>
        </div>
        <div class="col-md-1">
          <h3>Stock</h3>
          <div class="form-group">
            <input type="number" class="form-control" name="stock" min="0" max="30" value="" required>
          </div>
        </div>
        <div class="col-md-1">
          <h3>Size</h3>
          <div class="form-group">
            <input type="number" class="form-control" name="size" min="5" max="13" value="" required>
          </div>
        </div>
        <div class="col-md-2">
          <h3>Cost</h3>
          <div class="form-group">
            <label for="cost">$</label>
            <input type="number" id="cost" name="cost" min="15" max="10000" value="" required>
          </div>
          <br><button type="submit" class="btn btn-primary btn-md btn-block">Add Record</button>
          </form>
          <a href="javascript:history.back()" class="btn btn-danger btn-md btn-block">Cancel Addition</a>
        </div>
      </div>
    </div>
  </div>
</body>
