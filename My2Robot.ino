#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <MPU6050_tockn.h>
#include <DHT.h>

const char* ssid     = "Departments Bosses";
const char* password = "040302012003";

ESP8266WebServer server(80);
MPU6050 mpu(Wire);

#define IN1 D5
#define IN2 D6
#define IN3 D7
#define IN4 D8   // تم نقل IN4 من D0 إلى D8 حتى نستخدم D0 لحساس DHT
#define TRIG D3
#define ECHO D4

#define DHTPIN D0       // حساس الحرارة والرطوبة على D0
#define DHTTYPE DHT22   // إذا حساسك DHT22 غيّرها إلى DHT22
DHT dht(DHTPIN, DHTTYPE);

#define GAS_PIN A0      // حساس الغاز على المدخل التماثلي A0

// ─── متغيرات المسار ───────────────────────────────────────────
#define MAX_PATH 200          // أقصى عدد نقاط نحفظها
float pathX[MAX_PATH];
float pathY[MAX_PATH];
int   pathLen   = 0;
float posX      = 0;
float posY      = 0;
float headingDeg = 0;         // 0=أمام، 90=يمين، 180=خلف، 270=يسار
const float STEP_CM = 5.0;    // المسافة التقريبية لكل خطوة (اضبطها حسب روبوتك)

float angleX, angleY, distanceCM;
float temperatureC = 0;
float humidityRH = 0;
int gasValue = 0;       // قراءة حساس الغاز من 0 إلى 1023
String warning = "SAFE";

unsigned long lastSerialPrint = 0;

// ─── تسجيل نقطة في المسار ─────────────────────────────────────
void recordPosition() {
  if (pathLen < MAX_PATH) {
    pathX[pathLen] = posX;
    pathY[pathLen] = posY;
    pathLen++;
  }
}

// ─── حركات الموتور ────────────────────────────────────────────
void forward() {
  digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH); digitalWrite(IN4,LOW);
  


  float rad = headingDeg * PI / 180.0;
  posX += STEP_CM * sin(rad);
  posY -= STEP_CM * cos(rad);
  recordPosition();
}

void backward() {
  digitalWrite(IN1,LOW); digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW); digitalWrite(IN4,HIGH);

  float rad = headingDeg * PI / 180.0;
  posX -= STEP_CM * sin(rad);
  posY += STEP_CM * cos(rad);
  recordPosition();
}

void left() {
  digitalWrite(IN1,LOW);  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH); digitalWrite(IN4,LOW);
  headingDeg = fmod(headingDeg - 15 + 360, 360);
  recordPosition();
}

void right() {
  digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);  digitalWrite(IN4,HIGH);
  headingDeg = fmod(headingDeg + 15, 360);
  recordPosition();
}

void stopCar() {
  digitalWrite(IN1,LOW); digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW); digitalWrite(IN4,LOW);
}

// ─── قياس المسافة ─────────────────────────────────────────────
float readDistance() {
  digitalWrite(TRIG,LOW); delayMicroseconds(2);
  digitalWrite(TRIG,HIGH); delayMicroseconds(10);
  digitalWrite(TRIG,LOW);
  long dur = pulseIn(ECHO,HIGH,30000);
  return dur * 0.034 / 2;
}

// ─── بيانات المسار ─────────────────────────────────────────────
void handlePath() {
  String json = "{\"len\":" + String(pathLen) + ",\"cx\":" + String(posX) + ",\"cy\":" + String(posY) + ",\"hd\":" + String(headingDeg) + ",\"pts\":[";
  for (int i = 0; i < pathLen; i++) {
    if (i) json += ",";
    json += "[" + String(pathX[i], 1) + "," + String(pathY[i], 1) + "]";
  }
  json += "]}";
  server.send(200,"application/json",json);
}

void handleClearPath() {
  pathLen = 0; posX = 0; posY = 0; headingDeg = 0;
  pathX[0] = 0; pathY[0] = 0; pathLen = 1;
  server.send(200,"text/plain","cleared");
}

// ─── تحديث بيانات الحساسات ─────────────────────────────────────
void updateSensorValues() {
  mpu.update();
  angleX     = mpu.getAngleX();
  angleY     = mpu.getAngleY();
  distanceCM = readDistance();

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (!isnan(h)) humidityRH = h;
  if (!isnan(t)) temperatureC = t;

  gasValue = analogRead(GAS_PIN);

  if (abs(angleX) > 40 || abs(angleY) > 40) { warning = "DANGER"; stopCar(); }
  else warning = "SAFE";
}

// ─── بيانات الحساسات للموقع ───────────────────────────────────
void handleData() {
  updateSensorValues();

  String json = "{\"ax\":" + String(angleX,1) +
                ",\"ay\":" + String(angleY,1) +
                ",\"dist\":" + String(distanceCM,1) +
                ",\"temp\":" + String(temperatureC,1) +
                ",\"hum\":" + String(humidityRH,1) +
                ",\"gas\":" + String(gasValue) +
                ",\"warn\":\"" + warning + "\"}";
  server.send(200,"application/json",json);
}

// ─── صفحة HTML ─────────────────────────────────────────────────
String getHTML() {
  return R"=====(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Smart Robot Dashboard</title>
<style>
* { box-sizing: border-box; margin: 0; padding: 0; }
body {
  font-family: Arial, sans-serif;
  background: radial-gradient(circle at top, #202945 0%, #0b0f18 45%, #07090f 100%);
  color: #eee;
  text-align: center;
  min-height: 100vh;
}
h2 { padding: 14px 0 8px; font-size: 20px; letter-spacing: .5px; }
.controls { display: flex; flex-wrap: wrap; justify-content: center; gap: 6px; padding: 8px; }
button {
  padding: 12px 20px;
  font-size: 16px;
  border: none;
  border-radius: 8px;
  cursor: pointer;
  box-shadow: 0 7px 18px rgba(0,0,0,.25);
}
button:active { transform: scale(.96); }
.f { background: #2a9d4e; color: #fff; }
.b { background: #c0392b; color: #fff; }
.l { background: #2471a3; color: #fff; }
.r { background: #d68910; color: #fff; }
.s { background: #555;    color: #fff; }
.info-row { display: flex; flex-wrap: wrap; justify-content: center; gap: 8px; padding: 8px; }
.box {
  background: rgba(30,30,30,.82);
  border: 1px solid rgba(255,255,255,.07);
  padding: 10px 16px;
  border-radius: 10px;
  font-size: 14px;
  backdrop-filter: blur(6px);
}
.warn { color: #ff4d4d; font-size: 20px; font-weight: bold; margin: 6px 0; text-shadow: 0 0 12px rgba(255,0,0,.5); }
.safe { color: #2ecc71; font-size: 20px; font-weight: bold; margin: 6px 0; text-shadow: 0 0 12px rgba(46,204,113,.35); }

/* خريطة المسار */
#pathSection { padding: 10px; }
#pathSection h3 { font-size: 16px; margin-bottom: 6px; color: #c9d6ff; }
.map-wrap {
  width: min(94vw, 520px);
  margin: 0 auto;
  padding: 10px;
  border-radius: 18px;
  background: linear-gradient(145deg, rgba(255,255,255,.08), rgba(255,255,255,.015));
  border: 1px solid rgba(255,255,255,.1);
  box-shadow: inset 0 0 25px rgba(255,255,255,.03), 0 18px 40px rgba(0,0,0,.35);
}
#pathCanvas {
  width: 100%;
  height: auto;
  max-width: 500px;
  background: #111a2d;
  border: 1px solid #33405d;
  border-radius: 14px;
  display: block;
  margin: 0 auto;
  box-shadow: inset 0 0 35px rgba(0,0,0,.6);
}
.path-controls { display: flex; flex-wrap: wrap; justify-content: center; gap: 8px; margin-top: 8px; }
.pc-btn { padding: 8px 14px; font-size: 13px; border: none; border-radius: 6px; cursor: pointer; }
.pc-clr { background: #7b241c; color: #fff; }
.pc-csv { background: #1a5276; color: #fff; }
.pc-ctr { background: #1e8449; color: #fff; }
#pathInfo { font-size: 12px; color: #aab7d4; margin-top: 5px; }
.tip { font-size: 11px; color: #74839f; margin-top: 4px; }
</style>
</head>
<body>

<h2>🤖 SMART TRACKED ROBOT</h2>

<div class="controls">
  <button class='f' onclick="cmd('/f')">▲ Forward</button>
  <button class='l' onclick="cmd('/l')">◄ Left</button>
  <button class='s' onclick="cmd('/s')">■ Stop</button>
  <button class='r' onclick="cmd('/r')">► Right</button>
  <button class='b' onclick="cmd('/b')">▼ Backward</button>
</div>

<div class="info-row">
  <div class='box'>Angle X: <b id='ax'>0</b>°</div>
  <div class='box'>Angle Y: <b id='ay'>0</b>°</div>
  <div class='box'>Distance: <b id='d'>0</b> cm</div>
  <div class='box'>Temp: <b id='t'>0</b> °C</div>
  <div class='box'>Humidity: <b id='h'>0</b> %</div>
  <div class='box'>Gas: <b id='g'>0</b></div>
</div>
<div id='w' class='safe'>SAFE</div>

<div id="pathSection">
  <h3>🗺️ مسار الروبوت 3D View</h3>
  <div class="map-wrap">
    <canvas id="pathCanvas" width="500" height="500"></canvas>
  </div>
  <div class="path-controls">
    <button class="pc-btn pc-clr" onclick="clearPath()">✕ مسح المسار</button>
    <button class="pc-btn pc-ctr" onclick="autoCenter()">⊙ مركز</button>
    <button class="pc-btn pc-csv" onclick="downloadCSV()">⬇ CSV</button>
  </div>
  <div id="pathInfo">النقاط: 0 | X: 0 cm | Y: 0 cm | اتجاه: 0°</div>
  <div class="tip">استخدم عجلة الماوس للتكبير والتصغير، واسحب الخريطة بالماوس للتحريك.</div>
</div>

<script>
function cmd(url){ fetch(url); }

function updateSensors(){
  fetch('/data').then(r=>r.json()).then(d=>{
    document.getElementById('ax').innerText = d.ax.toFixed(1);
    document.getElementById('ay').innerText = d.ay.toFixed(1);
    document.getElementById('d').innerText  = d.dist.toFixed(1);
    document.getElementById('t').innerText  = d.temp.toFixed(1);
    document.getElementById('h').innerText  = d.hum.toFixed(1);
    document.getElementById('g').innerText  = d.gas;
    let w = document.getElementById('w');
    w.innerText = d.warn;
    w.className = (d.warn==="DANGER") ? "warn" : "safe";
  });
}
setInterval(updateSensors, 500);

const canvas = document.getElementById('pathCanvas');
const ctx = canvas.getContext('2d');

let pathData = { pts: [], cx: 0, cy: 0, hd: 0, len: 0 };
let viewOX = canvas.width / 2;
let viewOY = canvas.height / 2;
let scale = 3.0;

function worldToScreen(x, y) {
  // إسقاط بسيط يعطي إحساس 3D: X أفقي، Y مائل
  return {
    x: viewOX + x * scale + y * scale * 0.33,
    y: viewOY + y * scale * 0.72
  };
}

function drawGrid3D(W, H) {
  ctx.save();
  ctx.fillStyle = '#101829';
  ctx.fillRect(0, 0, W, H);

  // تدرج خفيف بالأرضية
  const grad = ctx.createLinearGradient(0, 0, 0, H);
  grad.addColorStop(0, 'rgba(70,110,180,.16)');
  grad.addColorStop(1, 'rgba(0,0,0,.08)');
  ctx.fillStyle = grad;
  ctx.fillRect(0, 0, W, H);

  // شبكة 3D مرتبة
  ctx.lineWidth = 1;
  for (let i = -40; i <= 40; i++) {
    let a = worldToScreen(i * 10, -400);
    let b = worldToScreen(i * 10,  400);
    ctx.strokeStyle = (i === 0) ? 'rgba(100,180,255,.45)' : 'rgba(100,150,210,.13)';
    ctx.beginPath(); ctx.moveTo(a.x, a.y); ctx.lineTo(b.x, b.y); ctx.stroke();

    let c = worldToScreen(-400, i * 10);
    let d = worldToScreen( 400, i * 10);
    ctx.strokeStyle = (i === 0) ? 'rgba(255,180,80,.42)' : 'rgba(100,150,210,.13)';
    ctx.beginPath(); ctx.moveTo(c.x, c.y); ctx.lineTo(d.x, d.y); ctx.stroke();
  }
  ctx.restore();
}

function drawSmoothPath(pts) {
  if (!pts || pts.length < 1) return;

  // ظل المسار
  ctx.save();
  ctx.lineJoin = 'round';
  ctx.lineCap = 'round';

  ctx.beginPath();
  let p0 = worldToScreen(pts[0][0], pts[0][1]);
  ctx.moveTo(p0.x, p0.y);
  for (let i = 1; i < pts.length; i++) {
    let p = worldToScreen(pts[i][0], pts[i][1]);
    ctx.lineTo(p.x, p.y);
  }
  ctx.strokeStyle = 'rgba(0,0,0,.55)';
  ctx.lineWidth = 9;
  ctx.stroke();

  // المسار الأساسي
  ctx.beginPath();
  ctx.moveTo(p0.x, p0.y);
  for (let i = 1; i < pts.length; i++) {
    let p = worldToScreen(pts[i][0], pts[i][1]);
    ctx.lineTo(p.x, p.y);
  }
  const g = ctx.createLinearGradient(0, 0, canvas.width, canvas.height);
  g.addColorStop(0, '#00e676');
  g.addColorStop(.55, '#00bcd4');
  g.addColorStop(1, '#5e9cff');
  ctx.strokeStyle = g;
  ctx.lineWidth = 5;
  ctx.stroke();

  // نقاط خفيفة فقط كل عدة نقاط حتى لا يكون الشكل مزعج
  for (let i = 0; i < pts.length; i += 4) {
    const p = worldToScreen(pts[i][0], pts[i][1]);
    ctx.beginPath();
    ctx.arc(p.x, p.y, 3, 0, Math.PI * 2);
    ctx.fillStyle = 'rgba(255,255,255,.75)';
    ctx.fill();
  }
  ctx.restore();
}

function drawStartPoint() {
  const s = worldToScreen(0, 0);
  ctx.save();
  ctx.beginPath();
  ctx.arc(s.x, s.y, 10, 0, Math.PI*2);
  ctx.fillStyle = '#ffb020';
  ctx.fill();
  ctx.strokeStyle = '#fff0b8';
  ctx.lineWidth = 2;
  ctx.stroke();
  ctx.fillStyle = '#111';
  ctx.font = 'bold 11px Arial';
  ctx.textAlign = 'center';
  ctx.fillText('S', s.x, s.y + 4);
  ctx.restore();
}

function drawRobot3D(x, y, headingDeg) {
  const p = worldToScreen(x, y);
  const ang = headingDeg * Math.PI / 180;

  ctx.save();
  ctx.translate(p.x, p.y);
  ctx.rotate(ang);

  // ظل
  ctx.beginPath();
  ctx.ellipse(0, 18, 28, 11, 0, 0, Math.PI*2);
  ctx.fillStyle = 'rgba(0,0,0,.45)';
  ctx.fill();

  // الجنزير اليسار
  ctx.fillStyle = '#252b33';
  roundRect(ctx, -26, -18, 12, 38, 5, true, false);
  ctx.fillStyle = '#0e1117';
  roundRect(ctx, -23, -14, 6, 30, 3, true, false);

  // الجنزير اليمين
  ctx.fillStyle = '#252b33';
  roundRect(ctx, 14, -18, 12, 38, 5, true, false);
  ctx.fillStyle = '#0e1117';
  roundRect(ctx, 17, -14, 6, 30, 3, true, false);

  // جسم الروبوت
  const bodyGrad = ctx.createLinearGradient(-18, -22, 22, 20);
  bodyGrad.addColorStop(0, '#5ed4ff');
  bodyGrad.addColorStop(.45, '#2075d6');
  bodyGrad.addColorStop(1, '#12335e');
  ctx.fillStyle = bodyGrad;
  roundRect(ctx, -18, -24, 36, 42, 8, true, false);

  // غطاء علوي يعطي إحساس 3D
  ctx.fillStyle = 'rgba(255,255,255,.22)';
  roundRect(ctx, -12, -18, 24, 13, 5, true, false);

  // حساس أمامي
  ctx.fillStyle = '#101820';
  roundRect(ctx, -11, -31, 22, 10, 4, true, false);
  ctx.fillStyle = '#38ff9c';
  ctx.beginPath(); ctx.arc(-5, -26, 2.5, 0, Math.PI*2); ctx.fill();
  ctx.beginPath(); ctx.arc(5, -26, 2.5, 0, Math.PI*2); ctx.fill();

  // سهم اتجاه صغير على جسم الروبوت
  ctx.beginPath();
  ctx.moveTo(0, -17);
  ctx.lineTo(7, -4);
  ctx.lineTo(-7, -4);
  ctx.closePath();
  ctx.fillStyle = '#ffffff';
  ctx.fill();

  // تحديد خارجي
  ctx.strokeStyle = 'rgba(255,255,255,.75)';
  ctx.lineWidth = 1.4;
  roundRect(ctx, -18, -24, 36, 42, 8, false, true);

  ctx.restore();
}

function roundRect(ctx, x, y, w, h, r, fill, stroke) {
  if (w < 2 * r) r = w / 2;
  if (h < 2 * r) r = h / 2;
  ctx.beginPath();
  ctx.moveTo(x+r, y);
  ctx.arcTo(x+w, y, x+w, y+h, r);
  ctx.arcTo(x+w, y+h, x, y+h, r);
  ctx.arcTo(x, y+h, x, y, r);
  ctx.arcTo(x, y, x+w, y, r);
  ctx.closePath();
  if (fill) ctx.fill();
  if (stroke) ctx.stroke();
}

function drawPath() {
  const W = canvas.width, H = canvas.height;
  ctx.clearRect(0, 0, W, H);

  drawGrid3D(W, H);

  const pts = pathData.pts || [];
  drawSmoothPath(pts);
  drawStartPoint();
  drawRobot3D(pathData.cx, pathData.cy, pathData.hd);

  document.getElementById('pathInfo').innerText =
    'النقاط: ' + pathData.len +
    ' | X: ' + Number(pathData.cx).toFixed(1) + ' cm' +
    ' | Y: ' + Number(pathData.cy).toFixed(1) + ' cm' +
    ' | اتجاه: ' + Number(pathData.hd).toFixed(0) + '°';
}

function updatePath() {
  fetch('/path').then(r=>r.json()).then(d=>{
    pathData = d;
    drawPath();
  });
}
setInterval(updatePath, 500);

function clearPath() {
  fetch('/clearpath').then(()=>{
    pathData={pts:[[0,0]],cx:0,cy:0,hd:0,len:1};
    autoCenter();
  });
}

function autoCenter() {
  const p = worldToScreen(pathData.cx, pathData.cy);
  viewOX += canvas.width/2 - p.x;
  viewOY += canvas.height/2 - p.y;
  drawPath();
}

function downloadCSV() {
  let csv = 'index,X_cm,Y_cm\n';
  (pathData.pts || []).forEach((p,i) => { csv += i+','+Number(p[0]).toFixed(2)+','+Number(p[1]).toFixed(2)+'\n'; });
  const a = document.createElement('a');
  a.href = 'data:text/csv;charset=utf-8,' + encodeURIComponent(csv);
  a.download = 'robot_path.csv';
  a.click();
}

canvas.addEventListener('wheel', e=>{
  e.preventDefault();
  const factor = e.deltaY < 0 ? 1.15 : 0.87;
  scale = Math.min(Math.max(scale*factor, 0.5), 20);
  drawPath();
}, { passive: false });

let drag=false, lastX=0, lastY=0;
canvas.addEventListener('mousedown', e=>{ drag=true; lastX=e.offsetX; lastY=e.offsetY; });
canvas.addEventListener('mousemove', e=>{
  if(!drag) return;
  viewOX += e.offsetX - lastX;
  viewOY += e.offsetY - lastY;
  lastX=e.offsetX; lastY=e.offsetY;
  drawPath();
});
canvas.addEventListener('mouseup',   ()=>drag=false);
canvas.addEventListener('mouseleave',()=>drag=false);

drawPath();
</script>
</body>
</html>
)=====";
}

// ─── الإعداد ───────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  Wire.begin(D2, D1);
  mpu.begin();
  mpu.calcGyroOffsets(true);
  dht.begin();

  pinMode(IN1,OUTPUT); pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT); pinMode(IN4,OUTPUT);
  pinMode(TRIG,OUTPUT); pinMode(ECHO,INPUT);
  stopCar();

  // نقطة البداية
  pathX[0] = 0; pathY[0] = 0; pathLen = 1;
  // ─── إعدادات الـ IP الثابت ─────────────────────────────────────
  IPAddress staticIP(192,168,68,30);   // الـ IP الذي تريده للـ ESP
  IPAddress gateway(192, 168, 1, 1);      // عادةً هذا هو IP الراوتر
  IPAddress subnet(255, 255, 255, 0);     // Subnet Mask
  IPAddress dns(8, 8, 8, 8);             // DNS (اختياري)

  WiFi.config(staticIP, gateway, subnet, dns);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nWiFi Connected: " + WiFi.localIP().toString());

  server.on("/", [](){server.send(200, "text/html; charset=utf-8", getHTML());});
  server.on("/f",        [](){ forward();  server.send(200,"text/plain","ok"); });
  server.on("/b",        [](){ backward(); server.send(200,"text/plain","ok"); });
  server.on("/l",        [](){ left();     server.send(200,"text/plain","ok"); });
  server.on("/r",        [](){ right();    server.send(200,"text/plain","ok"); });
  server.on("/s",        [](){ stopCar();  server.send(200,"text/plain","ok"); });
  server.on("/data",     handleData);
  server.on("/path",     handlePath);
  server.on("/clearpath",handleClearPath);

  server.begin();
  Serial.println("Server Started");
}

void loop() {
  server.handleClient();
}
