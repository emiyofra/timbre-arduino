// Configuración de Firebase
const firebaseConfig = {
    apiKey: "YOUR_API_KEY",
    authDomain: "YOUR_PROJECT_ID.firebaseapp.com",
    databaseURL: "https://YOUR_PROJECT_ID.firebaseio.com",
    projectId: "YOUR_PROJECT_ID",
    storageBucket: "YOUR_PROJECT_ID.appspot.com",
    messagingSenderId: "YOUR_MESSAGING_SENDER_ID",
    appId: "YOUR_APP_ID"
  };
  
  // Inicializar Firebase
  firebase.initializeApp(firebaseConfig);
  const database = firebase.database();
  
  document.addEventListener('DOMContentLoaded', () => {
    const horarioForm = document.getElementById('horarioForm');
    const statusDiv = document.getElementById('status');
  
    // Verificar conexión a Firebase
    database.ref(".info/connected").on("value", function(snapshot) {
      if (snapshot.val() === true) {
        statusDiv.innerText = "Conectado a Firebase.";
      } else {
        statusDiv.innerText = "Desconectado de Firebase.";
      }
    });
  
    // Manejar el evento de envío del formulario
    horarioForm.addEventListener('submit', (e) => {
      e.preventDefault();
  
      // Obtener los valores de los horarios
      const time1 = document.getElementById('time1').value;
      const time2 = document.getElementById('time2').value;
      const time3 = document.getElementById('time3').value;
      const time4 = document.getElementById('time4').value;
      const time5 = document.getElementById('time5').value;
  
      // Guardar los horarios en Firebase
      database.ref('horarios/').set({
        horario1: time1,
        horario2: time2,
        horario3: time3,
        horario4: time4,
        horario5: time5
      }, (error) => {
        if (error) {
          statusDiv.innerText = 'Error al guardar los horarios: ' + error.message;
        } else {
          statusDiv.innerText = 'Horarios guardados exitosamente.';
        }
      });
    });
  });
  