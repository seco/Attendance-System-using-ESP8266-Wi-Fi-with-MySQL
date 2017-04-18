<?php
class rfid{
 public $link='';
 function __construct($allow, $id){
  $this->connect();
  $this->storeInDB($allow, $id);
 }
 
 function connect(){
  $this->link = mysqli_connect('localhost','root','Deligence@1') or die('Cannot connect to the DB');
  mysqli_select_db($this->link,'rfidesp') or die('Cannot select the DB');
 }
 
 function storeInDB($allow, $id){
  $query = "insert into rfid set rfid='".$id."', allow='".$allow."'";
  $result = mysqli_query($this->link,$query) or die('Errant query:  '.$query);
 }
 
}
if($_GET['allow'] != '' and  $_GET['id'] != ''){
 $rfid=new rfid($_GET['allow'],$_GET['id']);
}


?>

