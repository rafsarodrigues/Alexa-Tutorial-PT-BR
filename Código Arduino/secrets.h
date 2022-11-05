#include <pgmspace.h>
 
#define SECRET
#define THINGNAME "Altere aqui"                         //change this
const char WIFI_SSID[] = "Altere aqui";               //change this
const char WIFI_PASSWORD[] = "Altere aqui";           //change this
const char AWS_IOT_ENDPOINT[] = "Altere aqui";       //change this
 
// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
Altere aqui
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                                               //change this
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
Altere aqui
-----END CERTIFICATE-----
)KEY";
 
// Device Private Key                                               //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
Altere aqui
-----END RSA PRIVATE KEY-----
)KEY";
