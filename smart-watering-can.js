// Constants
var JAR_NAME = "jar_1/";


// Sets up Firebase
var firebase = require("firebase-admin");
var serviceAccount = require("./smart-watering-can-credential.json");
firebase.initializeApp({
  credential: firebase.credential.cert(serviceAccount),
  databaseURL: "https://smart-watering-can.firebaseio.com"
});
var database = firebase.database().ref(JAR_NAME);                       // Creates a database reference in DEVICE_NAME

// Sets up Johnny-Five
var five = require("johnny-five");
board = new five.Board();


board.on("ready", function() {
  
  // Hardware pins
  var right_button = new five.Button({pin: 7, invert: false});
  var manual_button = new five.Button({pin: 9, invert: false});
  var left_button = new five.Button({pin: 8, invert: false});
  var humidity_sensor = new five.Sensor("A0");
  
  // Variables
  var releState = 0;
  var setpoint = 50;
  var EPSILON = 10;
  var manualButtonState;

  // Reads the database and updates the necessity states
  database.on("value", function(snapshot) {
  	
    setpoint = snapshot.val().setpoint;

    if (manualButtonState != snapshot.val().manual || manualButtonState == null) {
      if (snapshot.val().humidity - setpoint < EPSILON && manualButtonState != null) {
        releState = 1;
        console.log('Regando...')
      }
      manualButtonState = snapshot.val().manual;
    }

  });

  // When manual button is pressed
  manual_button.on('hit', function() {
  	console.log('Manual button pressed: '.concat(manualButtonState));
    if (manualButtonState <= 0) 
      database.update({'manual': 1});
    else
      database.update({'manual': 0});
  });

  // When left button is pressed
  left_button.on('hold', function() {
    if (setpoint > 0) setpoint -= 1;                        // Decreases setpoint by 1
    database.update({"setpoint": setpoint});                // Updates the database
  	console.log(setpoint);                                  // Prints message for debugging
  });

  // When right button is pressed
  right_button.on('hold', function() {
    if (setpoint < 100) setpoint += 1;                     // Increases setpoint by 1
    database.update({"setpoint": setpoint});               // Updates the database
  	console.log(setpoint);                                 // Prints message for debugging
  });

  // When there is a change in humidity
  humidity_sensor.on('change', function(humidade) {
    humidade = parseInt(humidade/10.23)
    database.update({"humidity": humidade});               // TODO: Update only a few minutes from each other
    
    if (releState == 0 && setpoint - humidade > 0) {
      console.log('Regando...')
      releState = 1;
    }
    if (releState == 1 && humidade - setpoint > EPSILON) {
      console.log('Parando de regar...')
      releState = 0;
    }
  });

});

// TODO: Funcao MAP