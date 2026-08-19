#include <WiFi.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

const char* ssid = "Free_5G_WiFi_Click_Me";
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Free 5G WiFi</title>
<style>
body{margin:0;background:#000;display:flex;justify-content:center;align-items:center;height:100vh;flex-direction:column;font-family:sans-serif;color:#fff}
video{width:100%;max-width:480px}
p{margin-top:12px}
</style>
</head>
<body>
<video controls autoplay playsinline muted>
<source src="/video.mp4" type="video/mp4">
</video>
<p>TechTadka360</p>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  delay(500);

  // Mount LittleFS. "true" formats it automatically if mounting fails
  // the first time (only happens once, safe to leave in).
  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS mount FAILED");
  } else {
    Serial.println("LittleFS mount OK");

    Serial.println("---- Listing ALL files in LittleFS ----");
    File root = LittleFS.open("/");
    File file = root.openNextFile();
    int count = 0;
    while (file) {
      Serial.print("  FOUND: ");
      Serial.print(file.name());
      Serial.print("   size: ");
      Serial.println(file.size());
      count++;
      file = root.openNextFile();
    }
    if (count == 0) {
      Serial.println("  (no files at all - partition is empty)");
    }
    Serial.println("----------------------------------------");

    if (LittleFS.exists("/video.mp4.mp4")) {
      File f = LittleFS.open("/video.mp4.mp4", "r");
      Serial.print("video.mp4.mp4 found, size: ");
      Serial.println(f.size());
      f.close();
    } else {
      Serial.println("video.mp4.mp4 NOT FOUND in LittleFS - re-upload data folder");
    }
  }

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  dnsServer.start(DNS_PORT, "*", apIP);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  server.on("/video.mp4", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (LittleFS.exists("/video.mp4.mp4")) {
      request->send(LittleFS, "/video.mp4.mp4", "video/mp4");
    } else {
      request->send(404, "text/plain", "video not found");
    }
  });

  server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->redirect("/");
  });
  server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->redirect("/");
  });
  server.on("/ncsi.txt", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->redirect("/");
  });
  server.on("/connecttest.txt", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->redirect("/");
  });

  server.onNotFound([](AsyncWebServerRequest *request) {
    request->redirect("/");
  });

  server.begin();
}

void loop() {
  dnsServer.processNextRequest();
}