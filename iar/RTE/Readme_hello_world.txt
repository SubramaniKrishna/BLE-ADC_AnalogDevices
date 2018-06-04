            Analog Devices, Inc. EV-COG-AD3029LZ Application Example


Project Name: hello_world

Note: This example has not been ported to IAR Embedded Workbench

Description:  Use the EV-COG-AD3029LZ on board Bluetooth Radio to exchange a string with the host application using the custom data exchange profile. 
 

Overview:
=========
    The single threaded, non-RTOS program rotates between sending "Hello World" and "Bye World" to the Android host application via Bluetooth. This example uses the custom Bluetooth profile "Data exchange", which allows for data to be sent and received via Bluetooth. 


User Configuration Macros:
==========================
    None. 


Hardware Setup:
===============   
    None.
 
Host App Setup:
===============
    Please refer to section "Installation" in the Android_Application_Users_Guide.pdf for details on
    how to install the Android app.


External connections:
=====================
    Connect the EV-COG-AD3029LZ to the host PC using the USB cable.


How to build and run:
=====================
     

    To run the example:
        1. Build and run the demo. 
        2. On the phone/table open the Android application ADIotNode.
        3. Press the "SCAN" button.
        4. Click on "ADI_BLE_HELLOWORLD" once it becomes available on the list of devices. This will create a connection to the EV-COG-AD3029LZ board.
        5. Click "DISCONNECT" to disconnect from the EV-COG-AD3029LZ board.


Expected Result:
=====================
    
	Starting DataExchange HelloWorld Example.

	Waiting for connection. Initiate connection on central device please.

	Connected!

	GAP mode changed.

	Unexpected event received.

	Connection interval updated.

	Connection interval updated.

	Error sending the data. (This occationally will happen on the first data packet)

	Data sent!

	Disconnected!


References:
===========
    EV-COG-AD3029LZ Schematic.
    EV-COG-AD3029LZ_Users_Guide.pdf
    IoT_Node_Users_Guide.pdf
