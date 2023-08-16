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
      .then(response => response.json())
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
   modulationOptions.value = data.modulation;
   startFreqInput.value = data.startFrequency;
   freqStepInput.value = data.step;
   canvas = document.getElementById('graphCanvas');
   const ctx = canvas.getContext('2d');
   const data = [91, 68, 5, 34, 15, 47, 58, 3, 56, 122,
      73, 48, 105, 20, 69, 8, 100, 16, 65, 92,
      79, 25, 54, 82, 44, 4, 95, 99, 13, 52,
      26, 74, 63, 98, 1, 66, 113, 86, 40, 9,
      116, 21, 117, 97, 19, 36, 83, 81, 39, 89,
      77, 64, 124, 51, 29, 94, 123, 60, 112, 18,
      127, 24, 71, 43, 59, 101, 53, 10, 76, 61,
      31, 50, 88, 67, 38, 75, 96, 6, 85, 84,
      110, 28, 32, 106, 102, 35, 45, 107, 78, 22,
      11, 72, 30, 41, 70, 114, 37, 109, 115, 33,
      49, 80, 42, 121, 93, 117, 111, 119, 87, 126,
      46, 55, 62, 120, 14, 117, 90, 103, 27, 117

   ];

   const barCount = data.length;
   const barWidth = canvas.width / (barCount * 2); // Increase the denominator for more spacing
   const maxValue = Math.max(...data);

   ctx.clearRect(0, 0, canvas.width, canvas.height);

   for (let i = 0; i < barCount; i++) {
      const barHeight = (data[i] / maxValue) * canvas.height;
      const x = i * (barWidth * 2); // Adjust the position based on barWidth
      const y = canvas.height - barHeight;

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

function onViewRF() {
   fetch('/on')
      .then(response => {
         if (!response.ok) {
            throw new Error('Network response was not ok');
         }
         return response.json()
      })
      .then(data => {
         console.log('response value onViewRF: ', data);
      })
      .catch(error => {
         console.error('Error:', error);
      });
}
document.getElementById('btnViewRF').addEventListener('click', onViewRF)


function onSendRF() {
   console.log("send RF")
   const dataToSend = {
      key: 'value'
   };

   fetch('/postDataRF', {
      method: 'POST',
      headers: {
         'Content-Type': 'application/json'
      },
      body: JSON.stringify(dataToSend)
   })
      .then(response => response.text())
      .then(data => {
         // Process the response data
         console.log('response value onSendRF: ', data);
      })
      .catch(error => {
         // Handle any errors that occurred during the fetch
         console.error('Error:', error);
      });
}
document.getElementById('btnSendRF').addEventListener('click', onSendRF)

function onJamming() {
   const dataToSend = {
      key: 'value'
   };

   fetch('http://your-esp8266-server-url/sendRF', {
      method: 'POST',
      headers: {
         'Content-Type': 'application/json'
      },
      body: JSON.stringify(dataToSend)
   })
      .then(response => response.json())
      .then(data => {
         // Process the response data
         console.log(data);
      })
      .catch(error => {
         // Handle any errors that occurred during the fetch
         console.error('Error:', error);
      });
}
function onSignal() {
   const dataToSend = {
      key: 'value'
   };

   fetch('http://your-esp8266-server-url/sendRF', {
      method: 'POST',
      headers: {
         'Content-Type': 'application/json'
      },
      body: JSON.stringify(dataToSend)
   })
      .then(response => response.json())
      .then(data => {
         // Process the response data
         console.log(data);
      })
      .catch(error => {
         // Handle any errors that occurred during the fetch
         console.error('Error:', error);
      });
}

// let intervalId; // Define a variable to store the interval ID
// function fetchDataFromServer() {
//    fetch('')
//       .then(response => response.json())
//       .then(data => {
//          // Process the received data
//          console.log(data);
//       })
//       .catch(error => {
//          // Handle any errors that occurred during the fetch
//          console.error('Error:', error);
//       });
// }
// // Start fetching data every 1 seconds
// intervalId = setInterval(fetchDataFromServer, 1000);

// //clean interval
// window.addEventListener('beforeunload', () => {
//    clearInterval(intervalId);
// });