<!DOCTYPE html>
<!--[if lt IE 7]>      <html class="no-js lt-ie9 lt-ie8 lt-ie7"> <![endif]-->
<!--[if IE 7]>         <html class="no-js lt-ie9 lt-ie8"> <![endif]-->
<!--[if IE 8]>         <html class="no-js lt-ie9"> <![endif]-->
<!--[if gt IE 8]><!--> <html class="no-js"> <!--<![endif]-->
<html>
<head>

<meta charset="utf-8">
<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
<meta name="description" content="">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<link rel="stylesheet" type="text/css" href="css/isotope.css" media="screen" />	
<link rel="stylesheet" href="js/fancybox/jquery.fancybox.css" type="text/css" media="screen" />
<link rel="stylesheet" href="css/bootstrap.css">
<link rel="stylesheet" href="css/bootstrap-theme.css">
<link rel="stylesheet" href="css/style.css">
<!-- skin -->
<link rel="stylesheet" href="skin/default.css">
<title> Bitslice Implementation </title>
</head>
<body>


<section id="section-content" class="section appear clearfix">
<div class="container">
   <div class="row mar-bot40">
      <div class="col-md-offset-3 col-md-6">
         <div class="section-header">
            <h2 class="section-heading animated" data-animation="bounceInUp">Bitslice</h2>
            <h2 class ="slogan">C++ Implementation</h2>
         </div>
      </div>
   </div>

   <div class="row">
      <div class="col-md-8 col-md-offset-2">
         <div class="cform" id="contact-form">
            <form action="." method="post">
               <div class="form-group">
                  <label for="sbox"> Fill in the following sbox:</label>
                  <input type="text" class="form-control" name="sbox" value="<?php
                     if( $_POST['sbox'] <> ""){
                        echo $_POST['sbox']; 
                     }else{
                        echo "14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7";
                     }
                   ?>" />
               </div>
               <button type="submit" class="btn btn-theme pull-left">Go!!!</button>
            </form>
        </div>
      </div>
   </div>
</section>

<section class="section appear clearfix">
   <div class="row">
      <div class="col-md-8 col-md-offset-2">
<?php
if($_POST['sbox'] <> ""){
   $str = preg_replace('/[^0-9 ]/', ' ', $_POST['sbox']);
   system("echo " . $str . " > /tmp/sbox");
   system("rm -rf /tmp/tmp1 /tmp/tmp2");
   system("./bitslice -s /tmp/sbox /tmp/tmp1 /tmp/tmp2 >/dev/null");

   if(file_exists("/tmp/tmp1")){
      echo "<p> Bitslice code: </p>";
      echo "<pre>";
      system("cat /tmp/tmp1");
      echo "</pre>";
   }else{
      echo "<p> Error in input!!!! </p>";
   }

   if(file_exists("/tmp/tmp2")){
      echo "<p> Array code: </p>";
      echo "<pre>";
      system("cat /tmp/tmp2");
      echo "</pre>";
   }
}
?>
      </div>
   </div>
</div>
</body>
</html>
