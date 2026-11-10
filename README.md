# ESP32 Virtual LED Controller

Static HTML/CSS/JavaScript dashboard for controlling two ESP32 LEDs through Firebase Realtime Database. Designed for GitHub Pages; no build step is required.

## Firebase configuration

The dashboard uses:
- Realtime Database: `https://wokwi-f4741-default-rtdb.asia-southeast1.firebasedatabase.app`
- Device ID: `esp32_virtual_001`
- `/devices/esp32_virtual_001/led1`
- `/devices/esp32_virtual_001/led2`

The Firebase web API key in `index.html` identifies the Firebase project; it is not a database authorization mechanism. Database access is controlled by Realtime Database Rules.

## Important: database security

The supplied ESP32 sketch reads the database using an unauthenticated HTTP GET, and this web page writes using the Firebase Web SDK without signing in. For this prototype to work unchanged, the database rules must permit unauthenticated reads and writes to this device path. **This makes the controls publicly accessible to anyone who knows the database URL. Do not use this configuration for real lights, mains-powered equipment, or a production deployment.**

For a private deployment, implement Firebase Authentication and restrictive per-user/device rules, then update the ESP32 firmware to authenticate and send a valid Firebase ID token (or use a trusted backend). Never put a Firebase service-account credential or database secret in browser code.

## Run locally

Serve this directory from a local HTTP server (ES modules do not work reliably from `file://`):
```bash
python3 -m http.server 8000
```
Open `http://localhost:8000`.

## Deploy with GitHub Pages

1. Open **Settings → Pages** in this repository.
2. Under **Build and deployment**, select **Deploy from a branch**.
3. Select branch **main** and folder **/(root)**, then save.
4. Wait for the Pages deployment to finish. The published URL will appear in the Pages settings.

No Node.js, npm, bundler, or backend is required.
