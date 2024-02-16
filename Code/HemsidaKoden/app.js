let playerData = {};

  //Metod skapad av Bilal Ayubi
function savePlayerData() {
  const firstName = document.getElementById('firstName').value;
  const lastName = document.getElementById('lastName').value;

  playerData = {
    firstName: firstName,
    lastName: lastName,
    playerName: `${firstName} ${lastName}`,
    choice: null,
    highScore: 0 
  };

  localStorage.setItem('playerData', JSON.stringify(playerData));

  window.location.href = "player.html";


}
//Metod skapad av Bilal Ayubi
function sendPlayerNameToServer() {
  const playerData = JSON.parse(localStorage.getItem('playerData'));
  const firstName = playerData.firstName;
  const lastName = playerData.lastName;
  const playerChoice = playerData.choice;

  console.log("Sending player name and choice to server:", firstName, lastName, playerChoice);

  const xhr = new XMLHttpRequest();
  xhr.open("POST", "http://localhost:3000/player-name", true);
  xhr.setRequestHeader("Content-Type", "application/json");
  xhr.onreadystatechange = function () {
    if (this.readyState === XMLHttpRequest.DONE) {
      if (this.status === 200) {
        console.log("Player name and choice sent successfully");
        
      } else {
        console.error("Error sending player name and choice:", this.statusText);
      }
    }
  };
  xhr.send(JSON.stringify({ firstName: firstName, lastName: lastName, choice: playerChoice }));
}

let isPolling = false;


//Metod skapad av BIlal Ayubi
function waitForMessageFromServer() {
  if (isPolling) {
    return; // Ifall det redan är igång skippas detta
  }
  isPolling = true;
  
  const xhr = new XMLHttpRequest();
  xhr.open("GET", "http://localhost:3000/wait-for-message", true);
  xhr.onreadystatechange = function () {
    if (this.readyState === XMLHttpRequest.DONE) {
      if (this.status === 200) {
        console.log("Message received from server:", this.responseText);
        if (this.responseText && this.responseText.trim().length > 0) {
          alert(this.responseText); // Alert box med meddelande endast ifall den inehåller något meddelande
        }
        isPolling = false; // Återställ polling flag
        waitForMessageFromServer(); // Skicka en till polling request
      } else {
        console.error("Error waiting for message from server:", this.statusText);
        isPolling = false; // Återställ polling flag
        setTimeout(() => {
          waitForMessageFromServer(); //Försök igen efter delay
        }, 3000);
      }
    }
  };
  xhr.send();
  console.log("Sent long polling request to server");
}

//metod skapad av Adam Lahbil
function receiveTotalTries() {
  const xhr = new XMLHttpRequest();
  xhr.open("GET", "http://localhost:3000/total-tries", true);
  xhr.onreadystatechange = function () {
    if (this.readyState === XMLHttpRequest.DONE) {
      if (this.status === 200) {
        const responseJson = JSON.parse(this.responseText);
        const totalTries = responseJson.totalTries;
        console.log("Received total tries from server:", totalTries);

        
        updatePlayerData(totalTries);

        
      } else {
        console.error("Error receiving total tries from server:", this.statusText);
      }
    }
  };
  xhr.send();
}
//Metod skapad av Adam Lahbil
function savePlayerSelection() {
  const radios = document.getElementsByName('player');
  let playerChoice = null;
  for (let i = 0; i < radios.length; i++) {
    if (radios[i].checked) {
      playerChoice = radios[i].value;
      break;
    }
  }

  let playerData = JSON.parse(localStorage.getItem('playerData')); 
  playerData.choice = playerChoice;
  localStorage.setItem('playerData', JSON.stringify(playerData)); 

  sendPlayerNameToServer(); 

  window.location.href = "highscore.html"; 
}


//mMetod skapad av Adam Lahbil
function updateHighScoreList(message) {
  const highScoreList = document.getElementById("highScoreList");
  const listItem = document.createElement("li");
  listItem.textContent = message;
  highScoreList.appendChild(listItem);
}

const socket = io('http://localhost:3000');

socket.on('highScoreUpdate', (data) => {
  console.log('Received high score update:', data);

  const playerData = JSON.parse(localStorage.getItem('playerData'));
  
  if (playerData) {
    if (data.highScore > playerData.highScore) {
      playerData.highScore = data.highScore;

      localStorage.setItem('playerData', JSON.stringify(playerData));
      
      displayHighScoreList();
    }
  }
});
//Metod skapad av Fatima Kadum
function displayHighScoreList() {
  const playerData = JSON.parse(localStorage.getItem('playerData'));

  if (playerData) {
    const li = document.createElement('li');
    li.textContent = `${playerData.firstName} ${playerData.lastName} ${playerData.choice} - High Score: ${playerData.highScore}`;

    const highScoreList = document.getElementById('highScoreList');
    highScoreList.innerHTML = '';
    highScoreList.appendChild(li);
  }
}

//metod skapad av Fatima Kadum
function updatePlayerData(totalTries) {
  const playerData = JSON.parse(localStorage.getItem('playerData'));
  playerData.totalTries = totalTries;
  localStorage.setItem('playerData', JSON.stringify(playerData));
}


waitForMessageFromServer();
