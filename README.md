# ESP32 Fake WiFi Prank

An ESP32-based prank access point. It broadcasts an open WiFi network, and when
someone connects and their phone/laptop opens the captive portal (or any page),
they get redirected to a page that plays a video.

## Connect with TechTadka360

- YouTube: [@techtadka360official](https://youtube.com/@techtadka360official?si=GdlIntZKv30kPgBk)
- Instagram: [@techtadka360official](https://www.instagram.com/techtadka360official?igsh=cWR4bnhjdWw1MHdh)
- Facebook: [TechTadka360](https://www.facebook.com/share/1EkKAJNLdB/)

## How it works

- ESP32 runs in Access Point mode, broadcasting an open network.
- A DNS server redirects every domain to the ESP32's own IP.
- A small async web server serves an HTML page with a `<video>` tag.
- The video file itself is stored on the ESP32's flash using LittleFS, and
  streamed to the browser on request.
- Common captive-portal detection URLs (`/generate_204`, `/hotspot-detect.html`,
  etc.) are redirected so most phones automatically pop up the page after
  connecting.

## Hardware / software required

- Any ESP32 dev board
- [Arduino IDE](https://www.arduino.cc/en/software) 2.x
- ESP32 board package installed (Tools > Board > Boards Manager > search "esp32")
- Libraries: `ESPAsyncWebServer` and `AsyncTCP` (install via Library Manager)
- The [arduino-littlefs-upload](https://github.com/earlephilhower/arduino-littlefs-upload)
  plugin — **required** to upload the video file to the board's filesystem

## Setup

1. **Add your video**
   Place a video file inside the `data/` folder, named exactly `video.mp4`.
   (Windows hides file extensions by default — turn on
   `View > Show > File name extensions` in File Explorer so you don't end up
   with a file accidentally named `video.mp4.mp4`.)

2. **Install the LittleFS upload plugin**
   Download the latest `.vsix` release from
   [earlephilhower/arduino-littlefs-upload](https://github.com/earlephilhower/arduino-littlefs-upload/releases),
   and place it in:
   ```
   C:\Users\<you>\.arduinoIDE\plugins\        (Windows)
   ~/.arduinoIDE/plugins/                     (macOS/Linux)
   ```
   Fully restart Arduino IDE afterward.

3. **Select your board settings**
   Tools > Board > ESP32 Dev Module (or your specific board)
   Tools > Partition Scheme > any scheme with SPIFFS/LittleFS space
   (e.g. "Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)")

4. **Upload the filesystem data**
   Open the Command Palette (`Ctrl+Shift+P` / `Cmd+Shift+P`), search for
   **"Upload LittleFS to Pico/ESP8266/ESP32"**, and run it.

5. **Upload the sketch**
   Click the normal Upload (→) button to flash `esp32-fake-wifi-prank.ino`.

6. **Check Serial Monitor**
   Open Serial Monitor at `115200` baud. On boot you should see a list of
   files found in LittleFS, and confirmation that `video.mp4` was found with
   its size.

7. **Test it**
   Connect a phone to the broadcast network. It should either auto-redirect
   to the video page, or open a browser and go to `192.168.4.1`.

## Customizing

- Change the network name: edit the `ssid` variable in the `.ino` file.
- Change the page text/styling: edit the `index_html` block.

## Troubleshooting

If the page loads but the video doesn't play, open Serial Monitor after boot —
it lists every file actually present in LittleFS along with its size. If
`video.mp4` isn't listed exactly as named, check for a hidden double
extension (`video.mp4.mp4`) or re-run the LittleFS upload step.

## Disclaimer

For harmless pranks among friends/family on networks you own or have
permission to use. Don't use this to intercept or redirect traffic on
networks or devices you don't have permission for.
