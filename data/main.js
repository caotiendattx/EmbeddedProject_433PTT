const navItems = document.querySelectorAll("nav li");

let globalRFData = null;
let globalScanData = null;
// one page renderring in navbar
navItems.forEach((item) => {
   item.addEventListener('click', function () {
      console.log(item)
      const target = this.getAttribute('data-target');
      const sections = document.querySelectorAll('section');
      sections.forEach(section => section.classList.remove('active'));
      const targetSection = document.getElementById(target);
      targetSection.classList.add('active')
      navItems.forEach(item => {
         item.querySelector('a').classList.remove('active');
      });
      this.querySelector('a').classList.add('active');
   })
})

function sendDataToServer(url, data) {
   fetch(url, {
      method: "POST",
      headers: {
         "Content-Type": "application/json"
      },
      body: JSON.stringify(data)
   })
      .then(response => {
         if (!response.ok) {
            throw new Error(`Server returned status: ${response.status}`);
         }
         return response.json();
      })
      .then(data => {
         console.log("Response from server:", data);
      })
      .catch(error => {
         console.error("Error:", error);
      });
}

document.addEventListener("DOMContentLoaded", function () {
   const form = document.getElementById("modulation-form");
   form.addEventListener("submit", function (event) {
      event.preventDefault();

      const modulationOptions = document.querySelectorAll('input[name="modulation"]:checked');
      const startFreq = document.getElementById("start-freq").value;
      const freqStep = document.getElementById("freq-step").value;

      const jsonData = {
         modulation: modulationOptions.length > 0 ? modulationOptions[0].value : null,
         startFrequency: startFreq,
         step: freqStep
      };

      console.log(jsonData);
      globalScanData = jsonData;
      sendDataToServer('/post/scan/submit', jsonData)
   });
});
document.addEventListener("DOMContentLoaded", function () {
   const form = document.getElementById("tx-rx-config-form");
   form.addEventListener("submit", function (event) {
      event.preventDefault(); // Prevent form submission

      const modulation = document.getElementById("modulation").value;
      const basicFrequency = parseFloat(document.getElementById("basicFrequency").value);
      const frequencyDeviation = parseFloat(document.getElementById("frequencyDeviation").value);
      const channelNumber = parseInt(document.getElementById("channelNumber").value);
      const channelSpacing = parseFloat(document.getElementById("channelSpacing").value);
      const receiveBW = parseFloat(document.getElementById("receiveBW").value);
      const dataRate = parseFloat(document.getElementById("dataRate").value);
      const power = parseInt(document.getElementById("power").value);
      const syncMode = parseInt(document.getElementById("syncMode").value);
      const syncWordHigh = document.getElementById("syncWordHigh").value;
      const syncWordLow = document.getElementById("syncWordLow").value;
      const adrCheck = parseInt(document.getElementById("adrCheck").value);
      const address = document.getElementById("address").value;
      const whitening = parseInt(document.getElementById("whitening").value);
      const pktFormat = parseInt(document.getElementById("pktFormat").value);
      const lengthConfig = parseInt(document.getElementById("lengthConfig").value);
      const packetLength = parseInt(document.getElementById("packetLength").value);
      const crc = parseInt(document.getElementById("crc").value);
      const crcaf = parseInt(document.getElementById("crcaf").value);
      const dcfilteroff = parseInt(document.getElementById("dcfilteroff").value);
      const manchester = parseInt(document.getElementById("manchester").value);
      const fec = parseInt(document.getElementById("fec").value);
      const pre = parseInt(document.getElementById("pre").value);
      const pqt = parseInt(document.getElementById("pqt").value);
      const appendstatus = parseInt(document.getElementById("appendstatus").value);

      const jsonData = {
         modulation,
         basicFrequency,
         frequencyDeviation,
         channelNumber,
         channelSpacing,
         receiveBW,
         dataRate,
         power,
         syncMode,
         syncWordHigh,
         syncWordLow,
         adrCheck,
         address,
         whitening,
         pktFormat,
         lengthConfig,
         packetLength,
         crc,
         crcaf,
         dcfilteroff,
         manchester,
         fec,
         pre,
         pqt,
         appendstatus
      };
      globalRFData = jsonData;
      //   console.log(jsonData); // You can replace this with your JSON handling logic
      sendDataToServer('/post/txrx/submit', jsonData)
   });
});

const form = document.getElementById("modulation-form");
const modulationOptions = form.elements["modulation"];
const startFreqInput = document.getElementById("start-freq");
const freqStepInput = document.getElementById("freq-step");

function updateFormWithData(data) {
   // modulationOptions.value = data.modulation;
   // startFreqInput.value = data.startFrequency;
   // freqStepInput.value = data.step;
   canvas = document.getElementById('graphCanvas');
   ctx = canvas.getContext('2d');
   let freqArrayString = data.freqArray;
   data = JSON.parse(freqArrayString);
   barCount = data.length;
   barWidth = canvas.width / (barCount * 2); // Increase the denominator for more spacing
   maxValue = Math.max(...data);

   ctx.clearRect(0, 0, canvas.width, canvas.height);

   for (let i = 0; i < barCount; i++) {
      barHeight = (data[i] / maxValue) * canvas.height;
      x = i * (barWidth * 2); // Adjust the position based on barWidth
      y = canvas.height - barHeight;

      ctx.fillStyle = 'blue';
      ctx.fillRect(x, y, barWidth, barHeight);
   }
}
let on = 0;
function fetchDataAndUpdateForm() {
   fetch(`/get/scan?on=${on}`)
      .then(response => response.json())
      .then(data => {
         updateFormWithData(data);
         console.log('scan data from server: ', data);
      })
      .catch(error => {
         console.error("Error:", error);
      });
}

// Fetch and update form every second
let intervalId = null;
const scanButton = document.getElementById("scanButton");
scanButton.addEventListener("click", function (event) {
   on = 1 - on;
   scanButton.textContent = scanButton.textContent === "SCAN OFF" ? "SCAN ON" : "SCAN OFF";
   event.preventDefault();
   if (on === 1) {
      intervalId = setInterval(fetchDataAndUpdateForm, 1000);
   } else {
      clearInterval(intervalId);
      fetchDataAndUpdateForm();
   }
});

window.addEventListener('beforeunload', () => {
   clearInterval(intervalId);
});

const sendRFRequest = document.getElementById("sendRFButton");
sendRFRequest.addEventListener("click", function (event) {
   event.preventDefault();
   sendDataToServer('/post/send/frequency', globalRFData);
});

const receiveRFRequest = document.getElementById("receiveRFButton");
receiveRFRequest.addEventListener("click", function (event) {
   event.preventDefault();
   sendDataToServer('/post/receive/frequency', globalRFData);
});

let isJammingClick = false;
const JammingButton = document.getElementById('jammingButton');
JammingButton.addEventListener("click", function () {
   JammingButton.textContext = isJammingClick === false ? "STOP" : "START";
   isJammingClick = !isJammingClick;
   const freqInputs = document.querySelectorAll(".jamming-col input[type='number']");
   const frequencies = Array.from(freqInputs).map(input => parseInt(input.value));
   const requestData = {
      jamming_freqs: frequencies
   };
   sendDataToServer('/post/jamming', requestData);
})
