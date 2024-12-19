<?php $rectWidth = 400; $rectHeight = 150;?>

<svg xmlns="http://www.w3.org/2000/svg" version="1.1" width="400" height="150">
<rect width="<?php echo $rectWidth;?>" height="<?php echo $rectHeight; ?>" fill="rgb(250, 230, 210)" stroke-width="1" stroke="rgb(0, 0, 0)" />
<g stroke="black">

<?php 

// <line x1="50" y1="20" x2="60" y2="100" stroke-width="1" /> 
/*
for ($s=0; $s<36; $s++) {
	
	$y=sin($s);
	echo $y. "\n";
}*/



	echo '<line x1="0" y1="0" '; // assuming starting at 0,0
	
for ($i; $i<$rectWidth/10; $i++) {	
	$x = $i * 10; 
	$y =  $rectHeight/2 * (1-sin(deg2rad($x))); 

	echo 'x2="' . $x . '" y2="' . $y . '" stroke-width="1" /> '."\n".' <line x1="' . $x . '" y1="' . $y . '" ';

};
	echo 'x2="' . $rectWidth . '" y2="' . $rectHeight/2 * (1-sin(deg2rad($rectWidth))). '" stroke-width="1" /> '."\n";

?>

</g>
</svg>