<html>
<head>
    <meta http-equiv='refresh' content='1'/>
    <title>Diesel the Hamster</title>
    <style>
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }
    </style>
</head>
<body>
<h1>Diesel the Hamster</h1>
<p>These are the voyages of Diesel, the Russian Dwarf hamster...</p>

<?php 
// This is the ROWSON server file pointed to through the firewall from JHS, references the ESP32.
// floor(microtime(true) * 1000) = number of milliseconds since the Unix epoch (0:00:00 January 1,1970 GMT)

$avespeed = file_get_contents("http://192.168.1.98/d/avespeed");
$maxspeed = file_get_contents("http://192.168.1.98/d/maxspeed");
$distance = file_get_contents("http://192.168.1.98/d/distance");
$millisnow = file_get_contents("http://192.168.1.98/d/millisnow");
$motioncount = file_get_contents("http://192.168.1.98/d/motioncount");
$lastwheelmillis = floor(microtime(true) * 1000) - file_get_contents("http://192.168.1.98/d/lastwheelmillis");
$lastmotionmillis = floor(microtime(true) * 1000) - file_get_contents("http://192.168.1.98/d/lastmotionmillis");

echo "<pre>";
echo "\n Average speed: " . $avespeed . " m/sec"; 
echo "\n Maximum speed: " . $maxspeed . " m/sec"; 
echo "\n Distance travelled: " . $distance . " m"; 
echo "\n Motion sensor triggers: " . $motioncount; 
echo "\n Last wheel turn: " . date("Y-m-d H:i:s", substr($lastwheelmillis, 0, 10)); 
echo "\n Last motion detected: " . date("Y-m-d H:i:s", substr($lastmotionmillis, 0, 10)); 

echo "</pre>";

?>

<p><img src="https://lh3.googleusercontent.com/pw/AP1GczP97wGZhFkKZHHAtFzpYpCeU5V939t1-REGrQr3tn9aCMPBZxeMHQ0Ck2OD414CHK4quMgtUPJ0FS2v_zxX-pwtxkirRNXtywkIz_P1OpLyCu8oKLlWpHcU5Xf05nWmpH2l_fJPjy7arR-ilxFk3rb2pA=w1920-h865-s-no-gm" height="300"><hr><img src='data:image/svg+xml;base64,<?php echo base64_encode(file_get_contents("http://192.168.1.98/test.svg")); ?>'></p>
<p>(Only a placeholder for a SVG-based data logging graph, one day it'll show x=time and y=distance travelled, or speed, or both...)</p>


</body>
</html>