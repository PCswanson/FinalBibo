#include "SparkFunPhant.h"


int count = 0; //This triggers a post and print on the first time through the loop

////////////PHANT STUFF//////////////////////////////////////////////////////////////////
const char server[] = "data.sparkfun.com";
const char publicKey[] = "9bgvRmrzzxFn9ZMRKm87";
const char privateKey[] = "xvRGdweVVXt1Kq8ve0nY";
Phant phant(server, publicKey, privateKey);
/////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
    Serial.begin(9600);   // open serial over USB

    postToPhant();
}
//---------------------------------------------------------------
void loop()
{
    //Rather than use a delay, keeping track of a counter allows the photon to
    // still take readings and do work in between printing out data.
    count++;
    //alter this number to change the amount of time between each reading
    //this number is low for troubleshooting purposes. Be sure to change it
    //once you deploy so as not to send too much data.
    if(count == 10000)
    {
       postToPhant();
       count = 0;
    }
}

int postToPhant()
{
    phant.add("today_date", 13);

    TCPClient client;
    char response[512];
    int i = 0;
    int retVal = 0;

    if (client.connect(server, 80))
    {
        Serial.println("Posting!");
        client.print(phant.post());
        delay(1000);
        while (client.available())
        {
            char c = client.read();
            //Serial.print(c);
            if (i < 512)
                response[i++] = c;
        }
        if (strstr(response, "200 OK"))
        {
            Serial.println("Post success!");
            retVal = 1;
        }
        else if (strstr(response, "400 Bad Request"))
        {
            Serial.println("Bad request");
            retVal = -1;
        }
        else
        {
            retVal = -2;
        }
    }
    else
    {
        Serial.println("connection failed");
        retVal = -3;
    }
    client.stop();
    return retVal;

}
