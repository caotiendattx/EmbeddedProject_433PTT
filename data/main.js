const navItems = document.querySelectorAll("nav li");

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

   fetch('/postDataRF?key="value"', {
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
//    fetch('http://your-esp8266-server-url/fetchData')
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