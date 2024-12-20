<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>

<meta name="keywords" content="hamster, russian dwarf, monitor, pets, remote access, php, python, Visual Studio, esp32, expressif, arduino, SOC, system-on-a-chip, IDE, html, coding, design, backend, server, raspberry pi, motion sensor, reed switch, magnets, project, DIY, geek, how-to" />
<meta name="description" content="A webpage entirely detailing the movements and activity of our Russian Dwarf Hamster called Diesel, using an ESP32 system-on-a-chip (SOC) and a Raspberry Pi">

<meta property="og:url" content="https://www.jameshovercraft.co.uk/diesel.php" />
<meta property="og:type" content="website" />
<meta property="og:title" content="The adventures of Diesel the Hamster" />
<meta property="og:description" content="A webpage entirely detailing the movements and activity of our Russian Dwarf Hamster called Diesel, using an ESP32 system-on-a-chip (SOC) and a Raspberry Pi" />
<meta property="og:image" content="https://lh3.googleusercontent.com/pw/AP1GczP97wGZhFkKZHHAtFzpYpCeU5V939t1-REGrQr3tn9aCMPBZxeMHQ0Ck2OD414CHK4quMgtUPJ0FS2v_zxX-pwtxkirRNXtywkIz_P1OpLyCu8oKLlWpHcU5Xf05nWmpH2l_fJPjy7arR-ilxFk3rb2pA=w1920-h865-s-no-gm" />


<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>The adventures of Diesel the Hamster</title>
    <style>
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }
    </style>
</head>
<body>
<h1>Diesel the Hamster</h1>
<p>These are the voyages of Diesel, the Russian Dwarf hamster...</p>
<p>Absolutely no hamsters were harmed in the making of this project, as it is entirely done via remote sensing! As far as we can tell, 
Diesel's privacy has not been invaded; he still pees in a secret corner and buries other waste products - and he has so far to lodge a complaint 
to the house council despite being given ample notice and chances to do so. A plus side of monitoring him is that, perhaps in theory, his activity level
may even be a proxy for his health and happiness - hamsters do love a good spin on their wheels! I suspect that mounting a webcam on the side of his cage
may well be just too much for the little guy's anxiety levels, so such flagrant disregard for personal space is unlikely to occur.</p>
<p><img src="https://lh3.googleusercontent.com/pw/AP1GczP97wGZhFkKZHHAtFzpYpCeU5V939t1-REGrQr3tn9aCMPBZxeMHQ0Ck2OD414CHK4quMgtUPJ0FS2v_zxX-pwtxkirRNXtywkIz_P1OpLyCu8oKLlWpHcU5Xf05nWmpH2l_fJPjy7arR-ilxFk3rb2pA=w1920-h865-s-no-gm" height="300">

<hr>
<h2>Basic data</h2>
<?php 
// This is the server file pointed to from the public-facing website jameshovercraft.co.uk/diesel.php, referencing the ESP32 whose IP address is defined below.
// floor(microtime(true) * 1000) = number of milliseconds since the Unix epoch (0:00:00 January 1,1970 GMT)

$esp32ip = "192.168.1.98";
$piip = "192.168.1.72";

$avespeed = file_get_contents("http://".$esp32ip."/d/avespeed");
$maxspeed = file_get_contents("http://".$esp32ip."/d/maxspeed");
$distance = file_get_contents("http://".$esp32ip."/d/distance");
$millisnow = file_get_contents("http://".$esp32ip."/d/millisnow");
$motioncount = file_get_contents("http://".$esp32ip."/d/motioncount");
$lastwheelmillis = floor(microtime(true) * 1000) - file_get_contents("http://".$esp32ip."/d/lastwheelmillis");
$lastmotionmillis = floor(microtime(true) * 1000) - file_get_contents("http://".$esp32ip."/d/lastmotionmillis");

echo "<pre>";
echo "\n Average speed: " . $avespeed . " m/sec (not quite working yet)"; 
echo "\n Maximum speed: " . $maxspeed . " m/sec"; 
echo "\n Distance travelled: " . $distance . " m"; 
echo "\n Motion sensor triggers: " . $motioncount; 
echo "\n Last wheel turn: " . date("Y-m-d H:i:s", substr($lastwheelmillis, 0, 10)); 
echo "\n Last motion detected: " . date("Y-m-d H:i:s", substr($lastmotionmillis, 0, 10)); 

// The following checks that the Raspberry Pi polling information from the esp32 device is running its Python script and outputting data.

$pidurl = "http://".$piip."/diesellogger/pid.php"; // php file dumping the python script process ID - this is deleted when the process ends (but admittedly not if the system reboots)
$headers = @get_headers($pidurl); // calling HTTP headers to check this exists or not (200 is yes, otherwise no, i.e., log not running)
if($headers && strpos( $headers[0], '200')) { 
    echo "\n\n Process ID: " . file_get_contents("http://".$piip."/diesellogger/pid.php");
} 
else { 
    $pidfilestatus = "\n\n Log not running"; 
} 
echo($pidfilestatus); 

echo "</pre>";

?>
<hr>
<p><h2>Data timeline</h2></p>
<p>(Only a placeholder for a SVG-based data logging graph, one day it'll show x=time and y=distance travelled, or speed, or both...)</p>

<?php 

$csvfile = "http://".$piip."/diesellogger/dieseldata.csv";

$fileData=fopen($csvfile,'r');
while (($line = fgetcsv($fileData)) !== FALSE) {
   $data[] = $line;
}
$dataLength = sizeof($data);

// SVG file generation:
$rectWidth = 700; $rectHeight = 300; ?>

<svg xmlns="http://www.w3.org/2000/svg" version="1.1" width="<?php echo $rectWidth;?>" height="<?php echo $rectHeight; ?>">
<rect width="<?php echo $rectWidth;?>" height="<?php echo $rectHeight; ?>" fill="rgb(250, 230, 210)" stroke-width="2" stroke="rgb(0, 0, 50)" />
<g stroke="black">


<?php 

	echo '<line x1="0" y1="0" '; // assuming starting at 0,0
	
for ($i; $i<$rectWidth/10; $i++) {	
	$x = $i * 10; 
	$y = $rectHeight/2 * (1-sin(deg2rad($x))); 

	echo 'x2="' . $x . '" y2="' . $y . '" stroke-width="1" /> '."\n".' <line x1="' . $x . '" y1="' . $y . '" ';

};
	echo 'x2="' . $rectWidth . '" y2="' . $rectHeight/2 * (1-sin(deg2rad($rectWidth))). '" stroke-width="1" /> '."\n";

?>

</g>
</svg>
<p><h2>Data table</h2></p><?php 
echo "<p>Number of rows: " . $dataLength . "</p>";

// DATA TABLE GENERATION: ?>  

<table border="1" cellspacing="2" cellpadding="2">
  <tr>
    <th scope="col" width="25">Row</th>
    <th scope="col" width="200">Date/Time</th>
    <th scope="col" width="150">Total Distance (m)</th>
    <th scope="col" width="150">Total Triggers</th>
  </tr>

<?php 

for ($row = 1; $row <$dataLength; $row++) { //doing it row by row instead of a foreach allows referencing between rows
    
    echo "<tr>";
    echo "<td align='center'>". $row . "</td>";
    echo "<td align='center'>". gmdate("Y-m-d H:i:s", $data[$row][0]) . "</td>";
    echo "<td align='center'>". $data[$row][1] . "</td>";
    echo "<td align='center'>". $data[$row][2] . "</td>";
    echo "</tr>";

} ?> </table>



</body>
</html>