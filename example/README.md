# csv2xml

This application creates XML files from CSV files.

You can pick up a CSV file like this:

```
city,       temperature,    humidity
london,     21,             56
madrid,     35,             45
paris,      22,             48
```

And create an XML file like this:

```XML
<?xml version="1.0" encoding="UTF-8" ?>
<xml>
	<entry id="0">
		<field name="city">london</field>
		<field name="temperature">21</field>
		<field name="humidity">56</field>
	</entry>
	<entry id="1">
		<field name="city">madrid</field>
		<field name="temperature">35</field>
		<field name="humidity">45</field>
	</entry>
	<entry id="2">
		<field name="city">paris</field>
		<field name="temperature">22</field>
		<field name="humidity">48</field>
	</entry>
</xml>
```

With this commad:

```
$ csv2xml city-weather.csv city-weather.xml
```
