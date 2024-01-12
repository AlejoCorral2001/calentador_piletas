String GAS_ID = "AKfycbwaeVBgTBD3WW6BT__Bx5USla74LV2N0Gr9vFgGFcxYzNkbAKtF_n9QxNev5qDjdqkv"; //--> spreadsheet script ID
const char* host = "script.google.com"; // only google.com not https://google.com

void update_google_sheet()
{
    //Serial.print("connecting to ");
    //Serial.println(host);
  
    // Use WiFiClient class to create TCP connections
    WiFiClientSecure client;
    const int httpPort = 443; // 80 is for HTTP / 443 is for HTTPS!
    
    client.setInsecure(); // this is the magical line that makes everything work
    
    if (!client.connect(host, httpPort)) { //works!
      //Serial.println("connection failed");
      return;
    }
       
    //----------------------------------------Processing data and sending data
    String url = "/macros/s/" + GAS_ID + "/exec?temperature=";
   
    url += String(TEMP2);
    
    url += "&humidity=";
    url += String(temp_diff);
    
    //Serial.print("Requesting URL: ");
    //Serial.println(url);
  
    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
  
    //Serial.println();
    //Serial.println("closing connection");  
}
