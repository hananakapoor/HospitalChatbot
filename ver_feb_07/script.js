let mediaRecorder;
let audioChunks = [];
let isRecording = false;

// Load messages from localStorage
const loadMessages = () => {
    const messages = JSON.parse(localStorage.getItem('chatMessages') || '[]');
    messages.forEach(msg => {
        if (msg.type === 'voice') {
            appendVoiceMessage(`ver_feb_07/audio/${msg.filename}`, msg.timestamp, msg.isBot);
        } else {
            appendMessage(msg.sender, msg.text, msg.timestamp);
        }
    });
};

// Save message to localStorage
const saveMessage = (sender, text, timestamp, type = 'text', filename = null, isBot = false) => {
    const messages = JSON.parse(localStorage.getItem('chatMessages') || '[]');
    messages.push({ sender, text, timestamp, type, filename, isBot });
    localStorage.setItem('chatMessages', JSON.stringify(messages));
};

// Initialize voice recording
async function initializeVoiceRecording() {
    try {
        const stream = await navigator.mediaDevices.getUserMedia({ audio: true });
        mediaRecorder = new MediaRecorder(stream, { mimeType: 'audio/webm' });

        mediaRecorder.ondataavailable = (event) => {
            audioChunks.push(event.data);
        };

        mediaRecorder.onstop = async () => {
            const audioBlob = new Blob(audioChunks, { type: 'audio/webm' });
            const timestamp = new Date().toLocaleTimeString();
            const filename = 'audio1.webm';

            const formData = new FormData();
            formData.append('audio', audioBlob, filename);

            try {
                const response = await fetch('http://localhost:8080/upload-audio', {
                    method: 'POST',
                    body: formData,
                });

                if (response.ok) {
                    const result = await response.json();
                    const recognizedText = result.recognizedText;

                    // Display recognized text in the chat
                    appendMessage('user', recognizedText, timestamp);
                    saveMessage('user', recognizedText, timestamp);

                    // Send recognized text to the chatbot
                    const botResponse = await fetch(`http://localhost:8080/get_next_question/${encodeURIComponent(recognizedText)}`);
                    const botReply = await botResponse.text();

                    // Display bot response in the chat
                    appendMessage('bot', botReply, new Date().toLocaleTimeString());
                    saveMessage('bot', botReply, new Date().toLocaleTimeString());

                    // Autoplay bot's voice response
                    const botVoiceUrl = `audio/audio2.wav?timestamp=${new Date().getTime()}`;
                    playAudio(botVoiceUrl);
                }
            } catch (error) {
                console.error('Upload failed:', error);
            } finally {
                hideLoadingAnimation(); // Hide loading animation after processing
            }

            audioChunks = [];
        };
    } catch (error) {
        console.error('Microphone access error:', error);
    }
}

// Append text message to chat box
function appendMessage(sender, message, timestamp) {
    const chatBox = document.getElementById('chatBox');
    const messageDiv = document.createElement("div");
    messageDiv.className = `message ${sender}`;

    const initial = sender === 'user' ? 'YOU' : 'BOT';

    messageDiv.innerHTML = `
        <div class="message-avatar">${initial}</div>
        <div class="message-content">
            <div class="message-text">${message}</div>
            <div class="message-timestamp">${timestamp}</div>
        </div>
    `;

    chatBox.appendChild(messageDiv);
    chatBox.scrollTop = chatBox.scrollHeight;
}

// Append voice message to chat box
function appendVoiceMessage(audioUrl, timestamp, isBot) {
    const chatBox = document.getElementById('chatBox');
    const messageDiv = document.createElement("div");
    messageDiv.className = `message ${isBot ? 'bot' : 'user'}`;

    const initial = isBot ? 'BOT' : 'YOU';

    messageDiv.innerHTML = `
        <div class="message-avatar">${initial}</div>
        <div class="message-content">
            <div class="message-text">Voice message</div>
            <audio controls autoplay onended="deleteVoiceMessage(this)" onplay="highlightPlaying(this)" onpause="unhighlightPlaying(this)">
                <source src="${audioUrl}" type="audio/wav">
            </audio>
            <div class="message-timestamp">${timestamp}</div>
        </div>
    `;

    chatBox.appendChild(messageDiv);
    chatBox.scrollTop = chatBox.scrollHeight;
}

// Delete voice message after playback
function deleteVoiceMessage(audioElement) {
    const messageDiv = audioElement.closest('.message');
    if (messageDiv) {
        messageDiv.remove();
    }
}

// Play audio and delete it after playback
function playAudio(audioUrl) {
    const audio = new Audio(audioUrl);
    audio.autoplay = true;
    audio.onended = () => {
        const messageDiv = document.querySelector(`audio[src="${audioUrl}"]`)?.closest('.message');
        if (messageDiv) {
            messageDiv.remove();
        }
    };
}

// Start the chat by fetching the first question
async function startChat() {
    try {
        const response = await fetch('http://localhost:8080/first_question');
        const firstQuestion = await response.text();
        appendMessage("bot", firstQuestion, new Date().toLocaleTimeString());

        // Automatically play the voice response for the first question
        const botVoiceUrl = `audio/audio1.wav?timestamp=${new Date().getTime()}`;
        playAudio(botVoiceUrl);
        setTimeout(() => {
            const botFilename = 'audio1.wav';
            const botTimestamp = new Date().toLocaleTimeString();
            appendVoiceMessage(`ver_feb_07/audio/${botFilename}`, botTimestamp, true);
            saveMessage('bot', '', botTimestamp, 'voice', botFilename, true);
        }, 1000);
    } catch (error) {
        console.error('Error starting chat:', error);
    }
}

// Initialize the application
document.addEventListener('DOMContentLoaded', () => {
    initializeVoiceRecording();
    loadMessages();
    startChat();
});

// Button event listeners
document.getElementById('send-btn').addEventListener('click', sendMessage);
document.getElementById('userInput').addEventListener('keypress', (e) => {
    if (e.key === 'Enter') sendMessage();
});

const voiceButton = document.getElementById('voice-input-btn');

voiceButton.addEventListener('mousedown', startRecording);
voiceButton.addEventListener('mouseup', stopRecording);
voiceButton.addEventListener('mouseleave', stopRecording);

// Send a text message
function sendMessage() {
    const input = document.getElementById('userInput');
    const message = input.value.trim();
    if (message) {
        const timestamp = new Date().toLocaleTimeString();
        appendMessage('user', message, timestamp);
        saveMessage('user', message, timestamp);

        fetch(`http://localhost:8080/get_next_question/${encodeURIComponent(message)}`)
            .then(response => response.text())
            .then(botReply => {
                appendMessage('bot', botReply, new Date().toLocaleTimeString());
                saveMessage('bot', botReply, new Date().toLocaleTimeString());

                // Autoplay bot's voice response
                const botVoiceUrl = `audio/audio2.wav?timestamp=${new Date().getTime()}`;
                playAudio(botVoiceUrl);
            })
            .catch(error => console.error('Error sending message:', error));

        input.value = '';
    }
}

// Start recording
function startRecording() {
    if (mediaRecorder && mediaRecorder.state === 'inactive') {
        audioChunks = [];
        mediaRecorder.start();
        isRecording = true;
        voiceButton.classList.add('recording');
        showRecordingAnimation();
    }
}

// Stop recording
function stopRecording() {
    if (mediaRecorder && mediaRecorder.state === 'recording') {
        mediaRecorder.stop();
        isRecording = false;
        voiceButton.classList.remove('recording');
        hideRecordingAnimation();
        showLoadingAnimation(); // Show loading animation while processing audio
    }
}

// Show loading animation
function showLoadingAnimation() {
    const loadingAnimation = document.getElementById('loadingAnimation');
    loadingAnimation.style.display = 'block';
}

// Hide loading animation
function hideLoadingAnimation() {
    const loadingAnimation = document.getElementById('loadingAnimation');
    loadingAnimation.style.display = 'none';
}

// Show recording animation
function showRecordingAnimation() {
    const recordingAnimation = document.getElementById('recordingAnimation');
    recordingAnimation.style.display = 'block';
}

// Hide recording animation
function hideRecordingAnimation() {
    const recordingAnimation = document.getElementById('recordingAnimation');
    recordingAnimation.style.display = 'none';
}



const chatbotIcon = document.getElementById('chatbot-icon');
const chatbotWindow = document.getElementById('chatbot-window');
const closeBtn = document.getElementById('close-btn');
let isDragging = false;
let offset = { x: 0, y: 0 };

// Toggle Chat Window
chatbotIcon.addEventListener('click', () => {
    chatbotWindow.classList.toggle('open');
    if (!chatbotWindow.classList.contains('open')) {
        resetChatPosition();
    }
});

// Close Chat Window
closeBtn.addEventListener('click', () => {
    chatbotWindow.classList.remove('open');
    resetChatPosition();
});

// Draggable Functionality
chatbotWindow.addEventListener('mousedown', (e) => {
    if (e.target.closest('.chat-header')) {
        isDragging = true;
        const rect = chatbotWindow.getBoundingClientRect();
        offset = {
            x: e.clientX - rect.left,
            y: e.clientY - rect.top
        };
        chatbotWindow.style.cursor = 'grabbing';
    }
});

document.addEventListener('mousemove', (e) => {
    if (isDragging) {
        const x = e.clientX - offset.x;
        const y = e.clientY - offset.y;
        chatbotWindow.style.left = `${x}px`;
        chatbotWindow.style.top = `${y}px`;
    }
});

document.addEventListener('mouseup', () => {
    isDragging = false;
    chatbotWindow.style.cursor = 'default';
});

// Reset to initial position
function resetChatPosition() {
    chatbotWindow.style.left = 'calc(100% - 370px)';
    chatbotWindow.style.top = 'calc(100% - 590px)';
}

// Animation Control
function showLoading() {
    document.getElementById('loadingAnimation').style.display = 'block';
}

function hideLoading() {
    document.getElementById('loadingAnimation').style.display = 'none';
}

function showRecording() {
    document.getElementById('recordingAnimation').style.display = 'block';
}

function hideRecording() {
    document.getElementById('recordingAnimation').style.display = 'none';
}

// Initialize position
resetChatPosition();