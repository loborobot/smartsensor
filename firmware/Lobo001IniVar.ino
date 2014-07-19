/*void startVal()
{
 for (_i=0;_i<7;_i++)
    {
      pinMode(_led[_i], OUTPUT);//define leds tipe OUTPUT
    } 
}*/

void startSim900()
{
 if(sim900gprs.getCurrentStatusGSM()==GSMOFFLINE) {
    // Si el estatus es OFFLINE intentar detectar
    // el modulo por dos maneras
    Serial.write("Trying to detect SIM900 module\r\n");
    // Esperando a Call ready
    if(sim900gprs.waitForString("Call Ready",5000)) {
      Serial.write("GSM Module detected. Current status: ONLINE\r\n");
      sim900gprs.setCurrentStatusGSM(GSMONLINE);
      // O verificando mediante comandos AT
    } else if(sim900gprs.checkForSIM900()) {
      Serial.write("GSM Module already connected. Changing status to: ONLINE\r\n");
      sim900gprs.setCurrentStatusGSM(GSMONLINE);
    }
  }
  if(sim900gprs.getCurrentStatusGSM()==GSMONLINE) {
    // Si esta ONLINE procesara el comando de la interrupcion
    if(sim900gprs.getSendMessage()) {
      // La primera vez que reciba la interrupcion intentara conectar
      // Este codigo ignorara el envio del mensaje
      if(sim900gprs.getCurrentStatusGPRS()==GPRSDISCONNECTED) {
        Serial.print("GPRS is disconnected. Starting up GPRS data.\r\n");
        Serial.print("Current request is going to be ignored.\r\n");
        // try to SETUP GPRS before connect
        sim900gprs.setupGPRS();
        sim900gprs.setSendMessage(false);
      // Una vez este conectado
      // Intentara realizar la conexion
      } else if(sim900gprs.getCurrentStatusGPRS()==GPRSCONNECTED) {
        Serial.print("DATA Interrupt received\r\n");
        sim900gprs.GET();
        sim900gprs.setSendMessage(false);
      }
    }
    sim900gprs.bridge();
  }
  if(sim900gprs.getStartShutDown()) {
    Serial.print("Shutdown interrupt received\r\n");
    sim900gprs.shutDown();
    sim900gprs.setStartShutDown(false);
    sim900gprs.setCurrentStatusGSM(GSMOFFLINE);
    sim900gprs.setCurrentStatusGPRS(GPRSDISCONNECTED);
  }
}
