const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
   <div class="LED">
        <h2>ESP Web Server</h2>
        %BUTTONPLACEHOLDER%
        <script>function toggleCheckbox(element) {
        var xhr = new XMLHttpRequest();
        if(element.checked){ xhr.open("GET", "/update?state=1", true); }
        else { xhr.open("GET", "/update?state=0", true); }
        xhr.send();
        }

        setInterval(function ( ) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
            var inputChecked;
            var outputStateM;
            if( this.responseText == 1){ 
                inputChecked = true;
                outputStateM = "On";
            }
            else { 
                inputChecked = false;
                outputStateM = "Off";
            }
            document.getElementById("output").checked = inputChecked;
            document.getElementById("outputState").innerHTML = outputStateM;
            }
        };
        xhttp.open("GET", "/state", true);
        xhttp.send();
        }, 1000 ) ;
        </script>
    </div>
     <div class="DHT">
        <h2>ESP8266 DHT Server</h2>
        <p>
            <i class="fas fa-tint" style="color:#00add6;"></i> 
            <span class="dht-labels">Humidity</span>
            <span id="humidity">%HUMIDITY%</span>
            <sup class="units">%</sup>
         </p>

     </div>       
     <div class="DHT">
        <h2>ESP8266 DHT Server</h2>
        <p>
            <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
            <span class="dht-labels">Temperature</span> 
            <span id="temperature">%TEMPERATURE%</span>
            <sup class="units">&deg;C</sup>
        </p>
     </div>    
 
           <div class="DHT">
        <h2>ESP8266 SOIL </h2>
         <p>
            <i class="fas fa-tint" style="color:#00add6;"></i> 
            <span class="dht-labels">Soil_Moisture</span>
            <span id="soil_moisture">%SOIL_MOISTURE%</span>
            <sup class="units">%</sup>
         </p>
     </div>   
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("soil_moisture").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/soil_moisture", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";  


