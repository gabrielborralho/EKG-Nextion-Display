void setup () {  
  size(1260, 620);  // set the window size.
  println(Serial.list()); // List all the available serial ports  
  myPort = new Serial(this, Serial.list()[0], 9600); // Open whatever port is the one you're using.  
  myPort.bufferUntil('\n'); // don't generate a serialEvent() unless you get a newline character. 
  background(0xff);  // set inital background.
}

void draw () {
  // everything happens in the serialEvent()
}

void serialEvent (Serial myPort) {
  
  String inString = myPort.readStringUntil('\n'); 

  if (inString != null) { // get the ASCII string:  
  
    inString = trim(inString);  // trim off any whitespace: 
    
    if (inString.equals("!")) { // If leads off detection is true notify with blue line
      stroke(0, 0, 0xff); //Set stroke to blue ( R, G, B)
      inByte = 512;  // middle of the ADC range (Flat Line)
    }    
    else { // If the data is good let it through
      stroke(0xff, 0, 0); //Set stroke to red ( R, G, B)
      inByte = float(inString); 
    }    
     
    inByte = map(inByte, 0, 1023, 0, height); //Map and draw the line for new data point
    height_new = height - inByte; 
    line(xPos - 1, height_old, xPos, height_new);
    height_old = height_new;    
    
    if (xPos >= width) { // at the edge of the screen, go back to the beginning:
      xPos = 0;
      background(0xff);
    } 
    else {        
      xPos++; // increment the horizontal position:
    }    
  }
}
