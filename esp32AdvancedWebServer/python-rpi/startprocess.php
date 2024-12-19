
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
<?php exec("python datalogger.py"); ?>