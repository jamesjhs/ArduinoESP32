
<html>
<head>
<title>Diesel's Data Logger</title>
</head>
<body>
<H1>Diesel's Data Logger</h1>
<p>Data logger started</p>
<p><a href="resetdata.php">Reset</a></php>
<p><a href="killpid.php">Kill process</a></p>
</body>
</html>

<?php
// This file is called by index.php and simply sends the unix command to start python and the datalogger.py file
// The python exec is created under the apache user, and so are any files created as a result of it, 
// but these are all within the var/www/html directory of the apache server and are therefore accessible from outside via http,
// and crucially are therefore deletable by the apache server when calld to do so via the killpid.php file generated by the python script.

exec("python datalogger.py"); 

?>