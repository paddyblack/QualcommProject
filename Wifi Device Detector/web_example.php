
<!DOCTYPE html>
<html>
<head>
<style>
table {
    font-size:25px;
    font-family:Arial;
}
</style>
</head>

<body>

<?php
    
    $username = "your MySQL username here";     // Type your MySQL username inside " "
    $password = "your MySQL password here";     // Type your MySQL password inside " "
    $hostname = "your MySQL hostname here";     // Type your MySQL hostname inside " "
    
    //connection to the database
    $dbhandle = mysql_connect($hostname, $username, $password)
    or die("Unable to connect to MySQL");
    echo "<p> <font size='50pt'>WIFI Presence Database</font></p><br>";
    echo "<br>";
    //select a database to work with
    $selected = mysql_select_db("comp2010",$dbhandle)
    or die("Could not select examples");
    
    //execute the SQL query and return records
    $result = mysql_query("SELECT * FROM your MySQL table name here"); // Type your MySQL table name after FROM
    
    $fields_num = mysql_num_fields($result);
    
    echo "<table border='1'><tr>";
    // printing table headers
    for($i=0; $i<$fields_num; $i++)
    {
        $field = mysql_fetch_field($result);
        echo "<td align='center'>{$field->name}</td>";
    }
    echo "</tr>\n";
    
    
    while($row = mysql_fetch_array($result))
    {
        echo "<tr>";
        echo "<td align='center'>$row[0]</td>";
        echo "<td align='center'>$row[1]</td>";
        echo "</tr>\n";
    }
    echo "</table>";
    
    
    mysql_free_result($result);
    
    
    ?>
</body>
</html>
